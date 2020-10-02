/*
 * Buffer.cpp
 *
 *  Created on: May 24, 2019
 *      Author: OS1
 */

#include "Buffer.h"
#include "System.h"
#include "Schedule.h"
//#include <stdio.h>
#include "PCB.h"

Buffer::Buffer() {
	first = 0;
	last = 0;
	size = 0;

}

void Buffer::add(PCB* p){
	//lock

	size++;
	Elem* novi = new Elem(p);
	//printf("Ubacena nit u blokiranje je %d i njen status je %d\n", p->pid, p->status);
	if (first == 0) {last = novi; first = novi;}
	else{
	last->next = novi;
	last = novi;
	}

	//unlock
}

void Buffer::removeFirst(){
	//lock

	if (first){
		//printf("Velicina liste je %d\n", size);
		//printf("ID niti koja je u listi %d \n", first->info->pid);
		first->info->status = READY;

		Scheduler::put((PCB*)first->info);

		//printf("ID niti koju sam stavila u scheduler je %d \n", first->info->pid);
		size--;
		Elem* tek = 0;
		tek = first;

		first = first->next;
		tek->next = 0;
		if (!first) last = 0;
		delete tek;
		}

	//unlock
}
void Buffer::removeByID(ID id){
	Elem* term = first;
	Elem* preth = 0;

	while (term) {

		if (term->info->pid != id){
			preth = term;
			term = term->next;
		}else {
			Elem* old = term;
			term = term->next;

			if (!preth) first = term;
			else preth->next = term;

			old->next = 0;
			delete old;
		}
	}


}

Thread* Buffer::findThreadByID(ID forFinding) {

	Elem* temp = first;

		while(temp && temp->info->pid != forFinding){
			temp = temp->next;
		}

		if(temp) return temp->info->myThread;
		else return 0;
}

Buffer::~Buffer() {

	Elem* pom = 0;
	size = 0;
	while(first){
		pom = first;
		first = first->next;
		pom->next = 0;
		delete pom;
	}


}

