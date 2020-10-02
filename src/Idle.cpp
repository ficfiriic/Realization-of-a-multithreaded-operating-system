/*
 * Idle.cpp
 *
 *  Created on: Jun 30, 2019
 *      Author: OS1
 */

#include "Idle.h"
#include "System.h"
#include "PCB.h"
Idle::Idle(StackSize stack, Time timeSlice) {
	// TODO Auto-generated constructor stub

}

void Idle::setUpIdle(){
	lock
	this->myPCB->status = READY;
	this->myPCB->timeLeft = 1;
	this->myPCB->timeToUse = 1;
	unlock
}

void Idle::run(){

	//printf("Radi idle\n");
	while (1) {}
}
Idle::~Idle() {
	// TODO Auto-generated des
}

