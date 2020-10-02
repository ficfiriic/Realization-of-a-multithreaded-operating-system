/*
 * IVTEntry.h
 *
 *  Created on: Jun 21, 2019
 *      Author: OS1
 */

#ifndef IVTENTRY_H_
#define IVTENTRY_H_
#include "System.h"
#include "Event.h"

class IVTEntry {
public:
	IVTEntry(IVTNo ivtNo, InterruptRoutine novaRutina);
	virtual ~IVTEntry();
	IVTEntry* getEntry(IVTNo ivtNo);
	void callOldRoutine();
	static IVTEntry** entries;
	void setEvent(KernelEv* ev);
	void getEvent();
private:
	KernelEv* myEvent;
	IVTNo myEntryNum;
	InterruptRoutine myOldRoutine;



};

#endif /* IVTENTRY_H_ */
