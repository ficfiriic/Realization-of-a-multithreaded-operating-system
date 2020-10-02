/*
 * Event.cpp
 *
 *  Created on: Jun 21, 2019
 *      Author: OS1
 */

#include "Event.h"
#include "System.h"
#include "KernelEv.h"

Event::Event(IVTNo ivtNo) {

	myImpl = new KernelEv(ivtNo);

}
void Event::wait(){

	lock
	myImpl->wait();
	unlock
}

void Event::signal(){

	lock

	myImpl->signal();

	unlock
}

Event::~Event() {
	lock
	delete myImpl;
	unlock
}

