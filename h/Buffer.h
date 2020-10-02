/*
 * Buffer.h
 *
 *  Created on: May 24, 2019
 *      Author: OS1
 */

#ifndef BUFFER_H_
#define BUFFER_H_
#include "Thread.h"

class Buffer {

	friend class PCB;
	friend class System;


	struct Elem{
		PCB* info;
		Elem* next;
		Elem (PCB* novi):info(novi),next(0){}
	};

	Elem* first;
	Elem* last;
	int size;
public:
	Buffer();
	void add(PCB* p); //dodaje na kraj liste
	void removeFirst(); //uzima prvi iz liste
	void removeByID(ID id); //skida onog sa odgovarajucim ID
	Thread* findThreadByID (ID id);
	virtual ~Buffer();
};

#endif /* BUFFER_H_ */
