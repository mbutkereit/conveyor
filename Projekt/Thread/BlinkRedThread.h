#ifndef BLINKREDTHREAD_H_
#define BLINKREDTHREAD_H_

#include <pthread.h>
#include "src/lib/HWaccess.h"
#include "src/lib/HAWThread.h"
#include "Hal/HalBuilder.h"

using namespace thread;

class BlinkRedThread: public HAWThread {
public:
	BlinkRedThread();
    virtual ~BlinkRedThread();
private:
    BlinkRedThread(const BlinkRedThread& b);      ///< Copy-Konstruktor. Privat, deshalb kann dieses Objekt nicht als "Call-by-value" uebergeben werden.
    BlinkRedThread& operator=(BlinkRedThread& b);
	virtual void execute(void*);
	virtual void shutdown();
};

#endif
