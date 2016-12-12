#ifndef BLINKYELLOWTHREAD_H_
#define BLINKYELLOWTHREAD_H_

#include <pthread.h>
#include "src/lib/HWaccess.h"
#include "src/lib/HAWThread.h"
#include "Hal/HalBuilder.h"

using namespace thread;

class BlinkYellowThread: public HAWThread {
public:
	BlinkYellowThread();
    virtual ~BlinkYellowThread();
private:
    BlinkYellowThread(const BlinkYellowThread& b);      ///< Copy-Konstruktor. Privat, deshalb kann dieses Objekt nicht als "Call-by-value" uebergeben werden.
    BlinkYellowThread& operator=(BlinkYellowThread& b);
	virtual void execute(void*);
	virtual void shutdown();
};

#endif
