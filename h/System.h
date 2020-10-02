/*
 * System.h
 *
 *  Created on: May 24, 2019
 *      Author: OS1
 */

#ifndef SYSTEM_H_
#define SYSTEM_H_
#include "Thread.h"
//#include <dos.h>
typedef void interrupt (*InterruptRoutine)(...);

#define lock asm pushf; asm cli
#define unlock asm popf

class System {
public:
	friend class Thread;
	friend class PCB;

	static volatile PCB* running;
	static volatile int zahtevana_promena_konteksta;
	static volatile int lockFlag;
	static Thread* MainThread;
	static InterruptRoutine old;
	static Idle* idle;

	System();

	static void interrupt fun(...);
	static void tick() {}
	static void inic();
	static void restore();
	virtual ~System();
};

#endif /* SYSTEM_H_ */
