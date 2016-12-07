#ifndef SERIALMESSAGERECVTHREAD_H_
#define SERIALMESSAGERECVTHREAD_H_

#include <cstdlib>
#include <hw/inout.h>

#include "Logger/Logger.h"
#include "src/lib/HWaccess.h"
#include "src/lib/HAWThread.h"
#include "FSM/Context.h"
#include "FSM/ContextSorting.h"
#include "FSM/ContextMotor.h"
#include "Hal/ISR/InterruptHandler.h"
#include "Dispatcher.h"
#include "Serializer/Serializer.h"
#include "Serializer/InfoMessage.h"
#include "Serializer/WorkpieceMessage.h"

using namespace thread;

/**
 * @file
 * @section DESCRIPTION
 *
 * Eine Klasse um die Signale zu verarbeiten.
 */
class SerialMessageRecvThread : public HAWThread {

public:
	SerialMessageRecvThread();
	virtual ~SerialMessageRecvThread();
private:
	SerialMessageRecvThread(const SerialMessageRecvThread& b);
	virtual void execute(void*);
	virtual void shutdown();
};

#endif /* SIGNALHANDLERTHREAD_H_ */
