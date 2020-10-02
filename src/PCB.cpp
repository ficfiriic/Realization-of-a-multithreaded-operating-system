/*
 * PCB.cpp
 *
 *  Created on: May 24, 2019
 *      Author: OS1
 */

#include "PCB.h"
#include "System.h"
#include "SCHEDULE.h"
#include "Buffer.h"

//#include <dos.h>//dohvatanje makroa segmenata i offseta
//#include <iostream.h>
//#include <stdlib.h>
ID PCB::id=0;
Buffer* PCB::allPCBs = new Buffer();

PCB::PCB(Thread *nit, StackSize velicinaSteka, Time vreme) {
	// TODO Auto-generated constructor stub
	lock

	waitingForMe = 0;
	myThread = nit;
	waitingForMe = new Buffer();

	timeToUse = vreme;
	timeLeft=vreme;
	semWait = 0;

	beskonacnost = 0;
	pid=++id;

	ss = 0; bp = 0; sp= 0;
	status = NEW;


	sizeI= velicinaSteka/sizeof(unsigned);
	stack = new unsigned[sizeI];

#ifndef BCC_BLOCK_IGNORE
	stack[sizeI - 1] = 0x200;
	stack[sizeI - 2] = FP_SEG(&(wrapper));
	stack[sizeI - 3] = FP_OFF(&(wrapper));

	this->ss = FP_SEG(stack + sizeI - 12);
	this->sp = FP_OFF(stack + sizeI - 12);
	this->bp = sp;
#endif


	if (this->myThread) PCB::allPCBs->add(this);

	unlock
}


void PCB::wrapper(){



	//printf("Nit koja treba da udje u svoj run je %d\n", System::running->pid);
	System::running->myThread->run();

	lock
	//printf("Nit koja je izasla iz svog runa je %d\n", System::running->pid);
	System::running->status = DEAD;

	while (System::running->waitingForMe->size>0) System::running->waitingForMe->removeFirst();

	dispatch();

	unlock
}


PCB::PCB(){

		waitingForMe = 0;
		myThread = 0;
		waitingForMe = new Buffer();

		timeToUse = 2;
		timeLeft=2;
		semWait = 0;

		beskonacnost = 0;
		pid=++id;

		ss = 0; bp = 0; sp= 0;
		status = NEW;


}
int PCB::getID(){return pid;}

PCB::~PCB() {

	lock

	delete waitingForMe;

	delete stack;

	if (this->myThread) PCB::allPCBs->removeByID(this->pid);

	unlock
}

