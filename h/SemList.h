/*
 * SemList.h
 *
 *  Created on: Jul 2, 2019
 *      Author: OS1
 */
/*
 * SemList.h
 *
 *  Created on: May 28, 2019
 *      Author: OS1
 */

#ifndef SEMLIST_H_
#define SEMLIST_H_
#include "PCB.h"

typedef unsigned int Time;
class SemList {

	struct Element {
	PCB* info; //koliko postoji polje u PCBudugo treba da ceka na semaforu
	Element* next;
	Element(PCB* p): info(p), next(0){}
	};

public:
	friend class KernelSem;

	SemList();
	void dodaj(PCB* p);
	PCB* odblokiraj();
	void stampaj();
	void dekrement(KernelSem* t);
	virtual ~SemList();
	static volatile int izbacio;
	static void updateSleepLists();
protected:
	Element* head;
	Element* tail;
	int brojBeskonacno;
	int brojBlokiranih;
};

#endif /* SEMLIST_H_ */
