/*
 * ContextTimeout.h
 *
 *  Created on: 28.11.2016
 *      Author: abs949
 */

#ifndef CONTEXTTIMEOUT_H_
#define CONTEXTTIMEOUT_H_

#include <iostream>
#include "TimeoutOptions.h"
#include "Logger/Logger.h"
using namespace std;

#define DEFAULT
#ifdef DEFAULT

    #define DELTA_T0_TH 25
    #define DELTA_TH_TW 12
    #define DELTA_TW_TE 22

    #define DELTA_TH_TE 1000
#else
    #define DELTA_T0_TH 1000

#endif

#define TOLERANCE 10

struct Datacto{
	Datacto():ticksPL(NULL), timeout(false), delta_t0_tH(DELTA_T0_TH), delta_tH_tW(DELTA_TH_TW), delta_tW_tE(DELTA_TW_TE){}
	int *ticksPL;
	bool timeout;
	int delta_t0_tH;
	int delta_tH_tW;
	int delta_tW_tE;
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

	void stopTimer();

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

    	virtual void stopTimer(){
    		new (this) StateStart;
    	}

    }*statePtr;   // a pointer to current state. Used for polymorphism.

    struct StateStart: public TimeOut {
    	virtual void startTimerT0(){
    		data->ticksPL = &data->delta_t0_tH;
    		new (this) StartT0_TH;
    	}

    	virtual void startTimerTH(){
    		data->ticksPL = &data->delta_tH_tW;
    		new (this) StartTH_TW;
    	}

    	virtual void startTimerTW(){
    		data->ticksPL = &data->delta_tW_tE;
    		new (this) StartTW_TE;
    	}

    };

    struct StartT0_TH: public TimeOut {
    	virtual void signalTimerTick(){
    		*data->ticksPL -= 1;
    		if(*data->ticksPL+TOLERANCE == 0){
    			LOG_DEBUG << "TIMEOUT AT T0_TH " << *data->ticksPL << "\n";
    			data->timeout = true;
    		}
    	}

    	virtual void stopTimerT0(){
    		new (this) StateStart;
    	}
    };

    struct StartTH_TW: public TimeOut {
    	virtual void signalTimerTick(){
    		*data->ticksPL -= 1;
    		if(*data->ticksPL+TOLERANCE == 0){
    			LOG_DEBUG << "TIMEOUT AT TH_TW: " << *data->ticksPL << "\n";
    			data->timeout = true;
    		}
    	}

    	virtual void stopTimerTH(){
    		new (this) StateStart;
    	}
    };

    struct StartTW_TE: public TimeOut {
    	virtual void signalTimerTick(){
    		*data->ticksPL-= 1;
    		if(*data->ticksPL+TOLERANCE == 0){
    			LOG_DEBUG << "TIMEOUT AT TW_TE: " << *data->ticksPL << "\n";
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
