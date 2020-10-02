/*
 * KernelEv.h
 *
 *  Created on: Jun 21, 2019
 *      Author: OS1
 */

#ifndef KERNELEV_H_
#define KERNELEV_H_
#include "PCB.h"
#include "System.h"
#include "Event.h"
#include "IVTEntry.h"
//#include "stdio.h"

#define PREPAREENTRY(brojRutine, stara)\
	void interrupt prekid##brojRutine(...);\
	IVTEntry newEntry##brojRutine(##brojRutine, prekid##brojRutine);\
		void interrupt prekid##brojRutine(...){\
		(IVTEntry::entries[##brojRutine])->getEvent();\
		if (stara) IVTEntry::entries[##brojRutine]->callOldRoutine();\
		dispatch();\
			}\



class KernelEv {
public:
	int value;
	PCB* myOwner;
	int blocking; //da li je zablokiran neko
	IVTEntry* entryForMe;


	KernelEv(IVTNo ivtNo);

	void wait();

	void signal();
	virtual ~KernelEv();
};

#endif /* KERNELEV_H_ */
