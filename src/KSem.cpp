/*
 * KSem.cpp
 *
 *  Created on: May 28, 2019
 *      Author: OS1
 */

#include "KSem.h"
#include "System.h"
#include "PCB.h"
#include "SCHEDULE.h"
#include "SemList.h"
volatile KernelSem* KernelSem::first = 0;
volatile KernelSem* KernelSem::last = 0;

KernelSem::KernelSem(int init) {
	//lock

	this->value = init;
	this->next = 0;
	lista =  new SemList();
	if (first == 0) {
					first = this;
					last = first;
						}
	else {
		last->next = this;
		last = this;
	}

	//unlock

}

int KernelSem::wait(Time maxTimeToWait){



	//printf("MaxTimeToWait je %d\n", maxTimeToWait);

	this->value--; //ako ja smanjim nesto i ono nakon smanjivanja bude 0, to znaci da ja smem
	//da prodjem ali niko posle mene ne sme
	//zato si stavila uslov this.value<0, ako je jednako 0 ti si poslednja koja prolazi dalje

	if (this->value < 0){
		int ret = 0; //oznacava povratnu vrednost, ako se waitu prosledi 0, znaci da sigurno nece biti deblokirana nit
		//jer joj je isteklo vreme(treba da ceka neograniceno), u svakom drugom slucaju se nit odblokira nakon isteka vremena

	if (maxTimeToWait==0) {System::running->beskonacnost = 1;  ret = 1;}
	else System::running->beskonacnost = 0;

	System::running->status = BLOCKED;
	System::running->semWait = maxTimeToWait;
	//printf("System::running->semWait je %d\n",System::running->semWait);
	//printf("Id niti koju ubacujem u listu za cekanje na semaforu je %d\n", PCB::running->id);
	//printf("Vrednost semafora posle waita je %d\n",value);
	lista->dodaj((PCB *)System::running);

	dispatch();

	if (ret) return 1;
	else return 0; //nulu vracam samo ako ne ceka neko beskonacno
	}

	return 1;

}

int KernelSem::signal(int n){

	if (n<0) return n;
	//printf("Vrednost semafora pre signala je %d\n", value);
	if (n==0) value++;
	else value = value+n;
	//printf("Vrednost semafora posle signala je %d\n", value);
	 //ako dobijem 0 povecam samo za jedan, ako dobijem vise povecam za vise
	if ((lista->brojBlokiranih>0) && (n==0)) { //znaci da samo jednog treba da odblokiram
											PCB* odblokiran = lista->odblokiraj();
											odblokiran->status = READY;
											Scheduler::put(odblokiran);
											return 0;}
	else
	if (lista->brojBlokiranih>0 && n>0) {
											int ret = lista->brojBlokiranih - n;
											if (ret<0) ret = lista->brojBlokiranih;
											else ret = n;
											int cnt = ret;
											while (cnt>0){
												PCB* odblokiran = lista->odblokiraj();
												odblokiran->status = READY;
												Scheduler::put(odblokiran);
												cnt--;
											};

											return ret;}

}

int KernelSem::val() const {

	return this->value;
}

KernelSem::~KernelSem() {
	// TODO Auto-generated destructor stub
	delete lista;
}

