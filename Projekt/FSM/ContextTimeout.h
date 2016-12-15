/*
 * ContextTimeout.h
 *
 *  Created on: 28.11.2016
 *      Author: abs949
 */

#ifndef CONTEXTTIMEOUT_H_
#define CONTEXTTIMEOUT_H_

#define DELTA_T0_TH 999
#define DELTA_TH_TW 999
#define DELTA_TW_TE 999

#include <iostream>
#include "TimeoutOptions.h"
using namespace std;

struct Datacto{
	Datacto():ticksPL(0), timeout(false){}
	int ticksPL;
	bool timeout;
};

class ContextTimeout {
public:
	ContextTimeout();
	virtual ~ContextTimeout();

	void timeout();

	void startTimerT0();

	void stopTimerT0();

	void startTimerTH();

	void stopTimerTH();

	void startTimerTW();

	void stopTimerTW();

	void signalTimerTick();

	bool timeoutOccured();
private:
    struct TimeOut { //top-level state
        Datacto* data;
    	virtual void timeout(){
    	}

    	virtual void startTimerT0(){
    	}

    	virtual void stopTimerT0(){
    	}

    	virtual void startTimerTH(){
    	}

    	virtual void stopTimerTH(){
    	}

    	virtual void startTimerTW(){
    	}

    	virtual void stopTimerTW(){
    	}

    	virtual void signalTimerTick(){
    	}

    }*statePtr;   // a pointer to current state. Used for polymorphism.

    struct StateStart: public TimeOut {
    	virtual void startTimerT0(){
    		data->ticksPL = DELTA_T0_TH;
    		new (this) StartT0_TH;
    	}

    	virtual void startTimerTH(){
    		data->ticksPL = DELTA_TH_TW;
    		new (this) StartTH_TW;
    	}

    	virtual void startTimerTW(){
    		data->ticksPL = DELTA_TW_TE;
    		new (this) StartTW_TE;
    	}

    };

    struct StartT0_TH: public TimeOut {
    	virtual void signalTimerTick(){
    		data->ticksPL--;
    		if(data->ticksPL == 0){
    			data->timeout = true;
    		}
    	}

    	virtual void stopTimerT0(){
    		new (this) StateStart;
    	}
    };

    struct StartTH_TW: public TimeOut {
    	virtual void signalTimerTick(){
    		data->ticksPL--;
    		if(data->ticksPL == 0){
    			data->timeout = true;
    		}
    	}

    	virtual void stopTimerTH(){
    		new (this) StateStart;
    	}
    };

    struct StartTW_TE: public TimeOut {
    	virtual void signalTimerTick(){
    		data->ticksPL--;
    		if(data->ticksPL == 0){
    			data->timeout = true;
    		}
    	}

    	virtual void stopTimerTW(){
    		new (this) StateStart;
    	}
    };

	StateStart stateMember;
	Datacto ctodata;
	ContextTimeout(const ContextTimeout& other);
	ContextTimeout& operator=(const ContextTimeout& other);
};

#endif /* CONTEXTTIMEOUT_H_ */
