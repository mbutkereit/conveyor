/*
 * ContextTimer.h
 *
 *  Created on: 28.11.2016
 *      Author: abs949
 */

#ifndef CONTEXTTIMER_H_
#define CONTEXTTIMER_H_

#define SLOW_FACTOR 2

#include <iostream>
#include <ctime>
#include "TimerOptions.h"

struct Datact{
	Datact(): currentSpeed(TIMER_STOP), pastTime(){
	}
	TimerOptions currentSpeed;
	clock_t pastTime;
};

class ContextTimer {
public:
	static ContextTimer* getInstance();
	virtual ~ContextTimer(){};
	void transact() {
		statePtr->transact();
	} // context delegates signals to state

	void setTimerSpeed(TimerOptions to)
	{
	    ctdata.currentSpeed = to;
	}

	clock_t giveTime()
	{
	    return ctdata.pastTime;
	}

private:
    struct TimerOfConveyor { //top-level state
        Datact* data;
        virtual void transact() {
        }
    }*statePtr;   // a pointer to current state. Used for polymorphism.

    struct StateStart: public TimerOfConveyor {
        virtual void transact() {
            if(ctdata.currentSpeed == TIMER_FAST)
            {
                ctdata.pastTime += clock();
                new (this) Fast;
            }else if(ctdata.currentSpeed == TIMER_SLOW)
            {
                ctdata.pastTime += (clock()/SLOW_FACTOR);
                new (this) Slow;
            }else if(ctdata.currentSpeed == TIMER_STOP)
            {
                new (this) Stop;
            }
        }
    };

    struct Fast: public TimerOfConveyor {
        virtual void transact() {
            ctdata.pastTime += clock();
            if (ctdata.currentSpeed == TIMER_SLOW)
            {
                new (this) Slow;
            } else if (ctdata.currentSpeed == TIMER_STOP)
            {
                new (this) Stop;
            }
        }
    };

    struct Slow: public TimerOfConveyor {
        virtual void transact() {
            ctdata.pastTime += (clock()/SLOW_FACTOR);
            if (ctdata.currentSpeed == TIMER_FAST)
            {
                new (this) Fast;
            } else if (ctdata.currentSpeed == TIMER_STOP)
            {
                new (this) Stop;
            } else
            {
                ctdata.pastTime += (clock()/SLOW_FACTOR);
            }
        }
    };

    struct Stop: public TimerOfConveyor {
        virtual void transact() {
            if (ctdata.currentSpeed == FAST)
            {
                new (this) FAST;
            } else if (ctdata.currentSpeed == SLOW)
            {
                new (this) SLOW;
            }
        }
    };

	static ContextTimer* instance_;
	StateStart stateMember; //The memory for the state is part of context object
	Datact ctdata;

	ContextTimer() :
			statePtr(&stateMember), ctdata() // assigning start state
	{
		statePtr->data = &ctdata;
	}
	ContextTimer(const ContextTimer& other);
	ContextTimer& operator=(const ContextTimer& other);
};

#endif /* CONTEXTTIMER_H_ */
