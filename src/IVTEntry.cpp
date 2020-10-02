/*
 * IVTEntry.cpp
 *
 *  Created on: Jun 21, 2019
 *      Author: OS1
 */

#include "IVTEntry.h"
#include "KernelEv.h"
//#include <stdio.h>
//#include <dos.h>
IVTEntry** IVTEntry:: entries = new IVTEntry*[256];

IVTEntry::IVTEntry(IVTNo ivtNo, InterruptRoutine intRoutine) {

	this->myEntryNum = ivtNo;
	this->myEvent = 0;

#ifndef BCC_BLOCK_IGNORE
	this->myOldRoutine = getvect(ivtNo);
	setvect(ivtNo, intRoutine);
#endif

	this->entries[ivtNo] = this;
}


IVTEntry::~IVTEntry() {

	lock
#ifndef BCC_BLOCK_IGNORE
	setvect(myEntryNum, myOldRoutine);
#endif

	//printf("Destruktor *******\n");
	entries[myEntryNum] = 0;
	unlock
}

void IVTEntry::callOldRoutine(){

	lock
	(*myOldRoutine) ();
	unlock

}

void IVTEntry::setEvent(KernelEv* event){
	if (this->myEvent==0) this->myEvent=event;
}

IVTEntry* IVTEntry::getEntry(IVTNo ivtNo){

	return entries[ivtNo];
}

void IVTEntry::getEvent(){
	//printf("Uzimam event iz getEvent\n");
	if (this->myEvent!=0) {
		//printf("Signaliziram event iz getEventa\n");
				this->myEvent->signal();
	}
}
