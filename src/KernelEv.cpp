/*
 * KernelEv.cpp
 *
 *  Created on: Jun 21, 2019
 *      Author: OS1
 */

#include "KernelEv.h"
#include "Schedule.h"
#include "System.h"

KernelEv::KernelEv(IVTNo ivtNo) {
	// TODO Auto-generated constructor stub

	entryForMe = IVTEntry::entries[ivtNo];

	this->myOwner = (PCB*) System::running;
	this->blocking=0;
	this->value=0;
	IVTEntry::entries[ivtNo]->setEvent(this);
}

void KernelEv::wait(){

	//printf("Usao u wait eventa\n");

	if (this->myOwner != System::running) return; //da ne moze da pozove neko ko nije napravio ovaj dogadjaj

	//printf("Kernel Event\n");
	if (this->value==1)
		{this->value = 0;
		//printf("Bla bla u pogresnu grana wait eventa\n");
		return;}
	if (this->value == 0) {

		this->value=-1;
		this->blocking = 1;
		//printf("Usao sam u pravu grana wait eventa\n");
		//this->value = 0;
		myOwner->status = BLOCKED;

		//printf("Blokirao sam vlasnika eventa\n");
		dispatch();

	}

	}




void KernelEv::signal(){


	//printf("Usao u signal eventa\n");
	if (this->value != -1)
		{this->value = 1; /*printf("Signal ne radim nista\n");*/ return;}

	this->blocking = 0;
	this->value=0;
	myOwner->status = READY;
	//printf("Odblokirao sam vlasnika eventa\n");
	Scheduler::put(this->myOwner);

}

KernelEv::~KernelEv() {

	//delete myOwner;
	delete entryForMe;
}

