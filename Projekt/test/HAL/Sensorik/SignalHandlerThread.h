#ifndef SIGNALHANDLERTHREAD_H_
#define SIGNALHANDLERTHREAD_H_

#include <cstdlib>
#include <hw/inout.h>

#include "Logger/Logger.h"
#include "src/lib/HWaccess.h"
#include "src/lib/HAWThread.h"
#include "FSM/Context.h"
#include "Hal/ISR/InterruptHandler.h"
using namespace thread;

extern int coid;
extern struct sigevent isrtEvent_;
extern int isrtConnection_;

/**
 * @file
 * @section DESCRIPTION
 *
 * Eine Klasse um die Signale zu verarbeiten.
 */
class SignalHandlerThread : public HAWThread {

public:
	SignalHandlerThread();
	virtual ~SignalHandlerThread();
private:
	SignalHandlerThread(const SignalHandlerThread& b);
	virtual void execute(void*);
	virtual void shutdown();
};

#endif /* SIGNALHANDLERTHREAD_H_ */
