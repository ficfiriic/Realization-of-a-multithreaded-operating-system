/*
 * PCB.h
 *
 *  Created on: May 24, 2019
 *      Author: OS1
 */

#ifndef PCB_H_
#define PCB_H_
#include "Thread.h"

enum Status {READY=0, NEW, DEAD,BLOCKED,WaitOnSem};

class PCB {

	friend class Idle;
	friend class KernelSem;
	friend class KernelEv;
	friend class SemList;
	friend class Thread;
	friend class Buffer;
	friend class System;
	friend class Main;

	unsigned *stack;
	unsigned int ss;
	unsigned int sp;
	unsigned int bp;
	Time timeToUse; //koliko se izvrsava
	Time timeLeft;//koliko mu je jos preostalo vremena da koristi CPU
	Time semWait; //koliko dugo ceka na semaforu

	Thread *myThread;

	int beskonacnost; //da li na semaforu treba da ceka beskonacno dugo
	int sizeI; //broj indeksa niza


	static int id;
protected:
	Status status;

public:
	int pid;
	Buffer* waitingForMe; //sve niti koje su pozvale mom waitToComplete i koje cekaju da se ja zavrsim
	static Buffer* allPCBs; //svi PCBovi za pretragu po IDu
	PCB(Thread* nit, StackSize velicinaSteka, Time vremeNiti);
	PCB();

	static void wrapper();
	int getID();
	virtual ~PCB();
};

#endif /* PCB_H_ */
