/*
 * Idle.h
 *
 *  Created on: Jun 30, 2019
 *      Author: OS1
 */

#ifndef IDLE_H_
#define IDLE_H_

#include "Thread.h"

class Idle:public Thread {
public:
	Idle(StackSize stackSize=defaultStackSize, Time timeSlice=defaultTimeSlice);
	void run();
	void setUpIdle();
	virtual ~Idle();
};

#endif /* IDLE_H_ */
