/*
 * SignalHandlerThread.h
 *
 *  Created on: 08.11.2016
 *      Author: abx827
 */

#ifndef SIGNALHANDLERTHREAD_H_
#define SIGNALHANDLERTHREAD_H_
#include <pthread.h>
#include <cstdlib>
#include <iostream>
#include <hw/inout.h>
#include "Logger/Logger.h"
#include "src/lib/HWaccess.h"
#include "src/lib/HAWThread.h"
#include "FSM/Context.h"
#include "FSM/ContextSorting.h"
using namespace thread;

extern int coid;
extern struct sigevent isrtEvent_;
extern int isrtConnection_;

class SignalHandlerThread : public HAWThread {

public:
	SignalHandlerThread();
	virtual ~SignalHandlerThread();
private:
	SignalHandlerThread(const SignalHandlerThread& b);
	virtual void execute(void*);
	virtual void shutdown();
};


#define ESTOP 0x400
#define RESET 0x800
#define START 0x1000
#define STOP 0x2000

#define ALTIMETRY_COMPLETED 0x4000

#define LIGHT_BARRIER_BEGIN_INTERRUPTED 0x1
#define LIGHT_BARRIER_END_INTERRUPTED 0x100
#define LIGHT_BARRIER_SWITCH_INTERRUPTED 0x10
#define LIGHT_BARRIER_ALTIMETRY_INTERRUPTED 0x4
#define LIGHT_BARRIER_SKID_INTERRUPTED 0x40

#define LIGHT_BARRIER_BEGIN_NOT_INTERRUPTED 0x2
#define LIGHT_BARRIER_END_NOT_INTERRUPTED 0x200
#define LIGHT_BARRIER_SWITCH_NOT_INTERRUPTED 0x20
#define LIGHT_BARRIER_ALTIMETRY_NOT_INTERRUPTED 0x8
#define LIGHT_BARRIER_SKID_NOT_INTERRUPTED 0x80


#endif /* SIGNALHANDLERTHREAD_H_ */
