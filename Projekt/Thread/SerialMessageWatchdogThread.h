#ifndef SERIALMESSAGEWATCHDOGTHREAD_H_
#define SERIALMESSAGEWATCHDOGTHREAD_H_

#include <cstdlib>
#include <hw/inout.h>
#include <sys/neutrino.h>

#include "Logger/Logger.h"
#include "src/lib/HWaccess.h"
#include "src/lib/HAWThread.h"
//#include "FSM/Context.h"
#include "FSM/ContextSorting.h"
#include "FSM/ContextMotor.h"
#include "Hal/ISR/InterruptHandler.h"
#include "Dispatcher.h"
#include "Serializer/Serializer.h"

using namespace thread;

#define WATCHDOG_SLEEP 100
#define WATCHDOG_PULSE_CODE 0x0F
//@todo Keine Globalisierung, in Europa.
/**
 * @file
 * @section DESCRIPTION
 *
 * Eine Klasse um die Signale zu verarbeiten.
 */
class SerialMessageWatchdogThread : public HAWThread {
public:
	SerialMessageWatchdogThread();
	virtual ~SerialMessageWatchdogThread();
	static void notify();
private:
	SerialMessageWatchdogThread(const SerialMessageWatchdogThread& b);
	virtual void execute(void*);
	virtual void shutdown();
};

#endif /* SIGNALHANDLERTHREAD_H_ */
