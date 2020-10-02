/*
 * Semaphor.cpp
 *
 *  Created on: May 28, 2019
 *      Author: OS1
 */

#include "Semaphor.h"
#include "KSem.h"
#include "System.h"

Semaphore::Semaphore(int init) {
	lock
	myImpl = 0;
	myImpl = new KernelSem(init);
	unlock
}


int Semaphore::wait(Time maxTimeToWait){
	lock
	//printf("maxTimeToWait je %d \n", maxTimeToWait);
	int ret = myImpl->wait(maxTimeToWait);
	unlock
	return ret;
}


int Semaphore::signal(int n){

	lock
	int ret = myImpl->signal(n);
	unlock
	return ret;
}

int Semaphore::val() const{

	return myImpl->val();
}
Semaphore::~Semaphore() {
	// TODO Auto-generated destructor stub
	lock
	delete myImpl;
	unlock
}

