/*
 * KSem.h
 *
 *  Created on: May 28, 2019
 *      Author: OS1
 */

#ifndef KSEM_H_
#define KSEM_H_
typedef unsigned int Time;

#include "SemList.h"
class KernelSem {
public:
	friend class PCB;
	friend class System;
	friend class SemList;

	KernelSem(int init);
	int wait(Time maxTimeToWait);
	int signal(int n);
	int val() const;
	virtual ~KernelSem();
	int value;
	KernelSem* next;
	SemList* lista; //oni koji cekaju na ovom semaforu
protected:

	static volatile KernelSem* first; //pokazivac na prvi semafor
	static volatile KernelSem* last; //pokazivac na poslednji semafor
};

#endif /* KSEM_H_ */
