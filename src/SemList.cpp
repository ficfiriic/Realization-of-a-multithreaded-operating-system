/*
 * SemList.cpp
 *
 *  Created on: May 28, 2019
 *      Author: OS1
 */

#include "KSem.h"
#include "SemList.h"
#include "PCB.h"
#include "Schedule.h"
#include "System.h"

//#include <stdio.h>
//#include <iostream.h>
int volatile SemList::izbacio = 0;

SemList::SemList() {

	head = 0;
	tail = 0;
	brojBeskonacno = 0;
	brojBlokiranih = 0;
}

void SemList::dodaj(PCB* p){

	if (p){
		Element* novi = new Element(p);
		//printf("Dodajem u listu cekanja na semaforu nit broj %d\n", p->pid);
		Element* tek = head;
		p->status=BLOCKED;
		Element* preth = 0;
		brojBlokiranih++;
		if (p->beskonacnost==1) {brojBeskonacno++; /*printf("Dodajem nit koja beskonacno ceka\n");*/};
		//else {printf("Dodajem nit koja ceka ogranicen broj \n");}

		while (tek && tek->info->semWait<novi->info->semWait){ //vrtis se u petlji dok god ima elemenata ciji semWait je manji od tvog
			novi->info->semWait-=tek->info->semWait; //mora ovako, jer ti cekajuci da oni zavrse i sebi vreme odbrojavas
			preth = tek;
			tek = tek->next;
		}

		if (tek){//ulazis ovde samo ako si nasla element ciji semWait je manji od tvog
			if (preth) {//negne u sredini, prespajas listu
				tek->info->semWait-=novi->info->semWait;//ti treba da budes iza novog zato -
				preth->next = novi;
				novi->next = tek;
			}else {//tekuci je prvi
				head = novi;
				novi->next = tek;
				tek->info->semWait-=novi->info->semWait;
			}
		}else{//ili nema nista u listi ili si stigla do kraja liste i svi imaju manji semWait od tebe tj. tek ==0
			if (!preth){
				//nema nista u listi pa dodajes prvog
				head = novi;
				tail = novi;
			}else {
				//dodajes na kraj liste
				preth->next = novi;
				tail = novi;
			}
		}
	}
}

PCB* SemList::odblokiraj(){ //zove samo signal i niko drugi

	int x=0;
	if (head){
		brojBlokiranih--;
		PCB* pom = 0;
		pom = head->info;
		x+=pom->semWait;
		if (pom->beskonacnost==1) brojBeskonacno--;
		Element* tek=head;
		head = head->next;
		//printf("U scheduler od signala ubacujem %d\n", pom->pid);
		pom->beskonacnost = 0;// za svaki slucaj
		if (!head) tail = 0;
		delete tek;
		if (head) head->info->semWait+=x;
		return pom;
	} return 0;

}

void SemList::dekrement(KernelSem* t){

	//this->stampaj();
	if (brojBlokiranih==brojBeskonacno || brojBlokiranih==0) return;
	Element* pom = head;
	Element* preth = 0;
	Element* tek = 0;
	while (pom->info->semWait == 0 && pom != 0) {preth = pom;pom = pom->next; /* printf("Preskacem\n");*/}
	if (pom==0) return;
	pom->info->semWait--;//prvi koji ne ceka beskonacno

	while ((pom->info->semWait==0) && pom){
		brojBlokiranih--;

		if (!preth) {head = head->next; if (!head) tail = 0;}//ako je prvi neki koji ne ceka beskonacno
		else {if (pom->next==0) {tail = preth; preth->next=0;}/*printf("Ovde sam montiram rep, moj rep pokazuje na nit broj %d sa cekanjem %d\n", preth->info->pid, preth->info->semWait);*///ako je poslednji neki koji ne ceka beskonacno
				else preth->next = pom->next; //neki u sredini
		}

		pom->info->status=READY;
		Scheduler::put((PCB*)pom->info);
		t->value++;
		//izbacio = 1;//oznacava da imam nesto u scheduleru
		//printf("Ubacujem u scheduler nit broj %d\n", pom->info->pid);
		Element* tek = pom;
		pom = pom->next;
		delete tek;

	}

}

SemList::~SemList() {
	// TODO Auto-generated destructor stub
	Element* pom = 0;
	brojBlokiranih = 0;
	brojBeskonacno = 0;
	while(head){
		pom = head;
		head = head->next;
		delete pom;

}
}

void SemList::updateSleepLists() {
	//printf ("INTERRUPT\n");
		KernelSem* pok = (KernelSem*)KernelSem::first;
		while (pok != 0) {
			pok->lista->dekrement((KernelSem*) pok);
			pok = pok->next;
		}

}

void SemList::stampaj(){
	int i=0;



	for(Element* tek=head;tek!=0;tek=tek->next){

      printf("%d ", tek->info->semWait);

	}
	printf("\n");
}
