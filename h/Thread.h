/*
 * Thread.h
 *
 *  Created on: May 21, 2019
 *      Author: OS1
 */
/*
 * thread
 *
 *  Created on: May 21, 2019
 *      Author: OS1
 */
//#include<stdio.h>

#ifndef THREAD_
#define THREAD_
typedef unsigned long StackSize;
const StackSize defaultStackSize = 4096;
typedef unsigned int Time;
const Time defaultTimeSlice = 2;
typedef int ID;

class PCB;

class Thread{

public:
	friend class Idle;
	friend class Main;
	friend class TestThread;
	friend class System;

	void start();
	void waitToComplete();
	virtual ~Thread();

	ID getId();
	static ID getRunningId();
	static Thread* getThreadById(ID id);

protected:
	friend class PCB; //mora da bude zbog wrapper metode TVOJ DODATAK
	Thread(StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice);
	virtual void run() {}

private:
	PCB* myPCB;

};

void dispatch();

#endif /* THREAD_ */
