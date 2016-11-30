/*
 * ContextTimeout.h
 *
 *  Created on: 28.11.2016
 *      Author: abs949
 */

#ifndef CONTEXTTIMEOUT_H_
#define CONTEXTTIMEOUT_H_

#define DELTA_T0_TH 0
#define DELTA_TH_TW 0
#define DELTA_TW_TE 0

#include <iostream>
#include "TimeoutOptions.h"
#include "DeltaTimes.h"
#include "ContextTimer.h"
using namespace std;

struct Datacto{
	Datacto(): to(TIMEOUT_STOP), dt(T0_TH), ct(ContextTimer::getInstance()), startTime(0){}
	TimeoutOptions to;
	DeltaTimes dt;
	ContextTimer* ct;
	clock_t startTime;
};

class ContextTimeout {
public:
	static ContextTimeout* getInstance();
	virtual ~ContextTimeout(){};
	void transact() {
		statePtr->transact();
	} // context delegates signals to state

	void setTimer(TimeoutOptions to, DeltaTimes dt)
	{
	    ctodata.to = to;
	    ctodata.dt = dt;
	}

private:
    struct TimeOut { //top-level state
        Datacto* data;
        virtual void transact() {
        }
    }*statePtr;   // a pointer to current state. Used for polymorphism.

    struct StateStart: public TimeOut {
        virtual void transact() {
            if (data->to == TIMEOUT_START)
            {
                data->startTime = data->ct->giveTime();
                switch (data->dt)
                {
                case T0_TH:
                    new (this) StartT0_TH;
                    break;
                case TH_TW:
                    new (this) StartTH_TW;
                    break;
                default:
                    new (this) StartTW_TE;
                    break;
                }
            }
        }
    };

    struct StartT0_TH: public TimeOut {
        virtual void transact(){
        }
    };

    struct StartTH_TW: public TimeOut {
        virtual void transact(){

        }
    };

    struct StartTW_TE: public TimeOut {
        virtual void transact(){

        }
    };

	static ContextTimeout* instance_;
	StateStart stateMember; //The memory for the state is part of context object
	Datacto ctodata;

	ContextTimeout() :
			statePtr(&stateMember), ctodata()// assigning start state
	{
		statePtr->data = &ctodata;
	}
	ContextTimeout(const ContextTimeout& other);
	ContextTimeout& operator=(const ContextTimeout& other);
};

#endif /* CONTEXTTIMEOUT_H_ */
