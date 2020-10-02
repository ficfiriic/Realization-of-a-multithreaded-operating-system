/*
 * System.cpp
 *
 *  Created on: May 24, 2019
 *      Author: OS1
 */

#include "System.h"
#include "PCB.h"
#include "SCHEDULE.h"
#include "Buffer.h"
#include "Idle.h"
#include "SemList.h"

//#include <dos.h>
//#include <stdio.h>
volatile PCB* System::running=0;
Thread* System::MainThread = 0;
volatile int System::zahtevana_promena_konteksta=0;
volatile int System::lockFlag=1;
InterruptRoutine System::old = 0;
Idle* System::idle = 0;

System::System() {}



void System::inic(){

			lock

			//novu rutinu stavljam na 8h, a onu koja je bila tu na 60h
#ifndef BCC_BLOCK_IGNORE
			old = getvect(0x8);
			setvect(0x8,&fun);
			setvect(0x60, old);
#endif

			MainThread = new Thread(4096,-1);
			System::running = MainThread->myPCB;
			System::running->status = READY;
			System::idle = new Idle();

			System::idle->setUpIdle();


			unlock

}

void System::restore(){
	lock

#ifndef BCC_BLOCK_IGNORE
	setvect(0x8, old);
#endif

	delete idle;
	delete MainThread;
	unlock
}


void interrupt System::fun(...){


	static unsigned tsp=0;
	static unsigned tss=0;
	static unsigned bpp=0;


	//printf ("U prekidnu rutinu ulazi nit %d\n", System::running->pid);

	if (zahtevana_promena_konteksta==0 /* && running->timeToUse!=0*/)	{	running->timeLeft--;
												//printf ("Smanjujem vreme \n");
													SemList::updateSleepLists();
													System::tick();
												//printf ("Pozivan staru rutinu %d\n", System::running->pid);
												asm int 60h;
											}
	if (zahtevana_promena_konteksta==1 || (System::running->timeLeft==0)){

		if (lockFlag){

		//uzmi kontekst
		asm {
			mov tsp, sp
			mov tss, ss
			mov bpp,bp
			}

			System::running->sp = tsp;
			System::running->ss = tss;
			System::running->bp = bpp;



			if (System::running->status == READY && System::running!=System::idle->myPCB) {
				//printf("U scheduler stavljam nit %d\n", System::running->pid);
				Scheduler::put((PCB*) System::running);
			}

			System::running = Scheduler::get();
			//printf("Iz schedulera uzimam nit %d\n", System::running->pid);

			if (System::running == 0) System::running= System::idle->myPCB;
			System::running->status = READY;

			System::running->timeLeft = System::running->timeToUse; //postavi vreme koliko dugo treba da se izvrsava

			zahtevana_promena_konteksta = 0;

			tsp = System:: running->sp;
			tss = System:: running->ss;
			bpp = System::running->bp;

			//stavi novi kontekst
			asm{
				mov sp, tsp
				mov ss, tss
				mov bp, bpp
			}

		//if od lockflaga
		}

	}


	//printf("Iz prekidne rutine izlazi nit %d\n", System::running->pid);
}


System::~System() {


}

