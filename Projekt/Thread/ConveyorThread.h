#ifndef CONVEYORTHREAD_H_
#define CONVEYORTHREAD_H_

#include <pthread.h>
#include "src/lib/HWaccess.h"
#include "src/lib/HAWThread.h"
#include "Hal/HalBuilder.h"

using namespace thread;

/**
 * @file
 * @section DESCRIPTION
 *
 * Eine Klasse um einen Thread fuer die Ample stuerung zu starten.
 */
class ConveyorThread: public HAWThread {
public:
	ConveyorThread();
	virtual ~ConveyorThread();
private:
	ConveyorThread(const ConveyorThread& b);      ///< Copy-Konstruktor. Privat, deshalb kann dieses Objekt nicht als "Call-by-value" uebergeben werden.
	ConveyorThread& operator=(ConveyorThread& b);
	virtual void execute(void*);
	virtual void shutdown();
};
#endif /* CONVEYORTHREAD_H_ */
