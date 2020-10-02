/*
 * Thread.cpp
 *
 *  Created on: May 21, 2019
 *      Author: OS1
 */

#include "Thread.h"
#include "PCB.h"
#include "System.h"
#include "Buffer.h"
#include "SCHEDULE.h"
#include "Idle.h"
//#include <dos.h>


Thread::Thread(StackSize stackSize, Time timeSlice){

	lock
	myPCB = new PCB(this, stackSize, timeSlice);
	unlock
}

ID Thread::getRunningId(){
	ID id = 0;
	id = System::running->pid;
	return id;

}

ID Thread::getId(){
	return myPCB->pid;
}

Thread* Thread::getThreadById(ID id) {

	lock

	Thread* toRet = PCB::allPCBs->findThreadByID(id);

	unlock

	return toRet;
}

void Thread::start(){

	lock
		myPCB->status=READY;
		Scheduler::put(myPCB);
		//printf("Ubacio u Scheduler nit broj %d \n", this->getId() );
	unlock

}

 void Thread::waitToComplete(){

	 lock

	 //printf ("WaitToComplete niti broj %d\n", this->myPCB->pid);
	 if ((this->myPCB->status!=DEAD) && (System::running != (volatile PCB*)this->myPCB) && (this != System::idle)){
		 System::running->status = BLOCKED;
		 this->myPCB->waitingForMe->add((PCB*)System::running);
		 dispatch();
	 }

	 unlock
}


 Thread::~Thread(){
	 lock
	 waitToComplete();
	 delete myPCB;
	 unlock

 }


 void dispatch(){

	 lock

	 //printf("Pozvan dispatch niti %d\n", System::running->pid);
	 System::zahtevana_promena_konteksta = 1;
	 System::fun();

	 unlock

 }


