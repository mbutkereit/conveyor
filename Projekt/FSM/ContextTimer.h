/*
 * ContextTimer.h
 *
 *  Created on: 28.11.2016
 *      Author: abs949
 */

//#ifndef CONTEXTTIMER_H_
//#define CONTEXTTIMER_H_
//
//#define SLOW_FACTOR 2
//
//#include <iostream>
//#include <ctime>
//#include "TimerOptions.h"
//
//struct Datact{
//	Datact(): currentSpeed(TIMER_STOP), pastTime(){
//	}
//	TimerOptions currentSpeed;
//	clock_t pastTime;
//};
//
//class ContextTimer {
//public:
//	static ContextTimer* getInstance();
//	virtual ~ContextTimer(){};
//	void transact() {
//		statePtr->transact();
//	} // context delegates signals to state
//
//	void setTimerSpeed(TimerOptions to)
//	{
//	    ctdata.currentSpeed = to;
//	}
//
//	clock_t giveTime()
//	{
//	    return ctdata.pastTime;
//	}
//
//private:
//    struct TimerOfConveyor { //top-level state
//        Datact* data;
//        virtual void transact() {
//        }
//    }*statePtr;   // a pointer to current state. Used for polymorphism.
//
//    struct StateStart: public TimerOfConveyor {
//        virtual void transact() {
//            if(data->currentSpeed == TIMER_FAST)
//            {
//                data->pastTime += (clock() - data->pastTime);
//                new (this) Fast;
//            }else if(data->currentSpeed == TIMER_SLOW)
//            {
//                data->pastTime += ((clock() - data->pastTime)/SLOW_FACTOR);
//                new (this) Slow;
//            }else if(data->currentSpeed == TIMER_STOP)
//            {
//                new (this) Stop;
//            }
//        }
//    };
//
//    struct Fast: public TimerOfConveyor {
//        virtual void transact() {
//            data->pastTime += (clock() - data->pastTime);
//            if (data->currentSpeed == TIMER_SLOW)
//            {
//                new (this) Slow;
//            } else if (data->currentSpeed == TIMER_STOP)
//            {
//                new (this) Stop;
//            }
//        }
//    };
//
//    struct Slow: public TimerOfConveyor {
//        virtual void transact() {
//            data->pastTime += ((clock() - data->pastTime)/SLOW_FACTOR);
//            if (data->currentSpeed == TIMER_FAST)
//            {
//                new (this) Fast;
//            } else if (data->currentSpeed == TIMER_STOP)
//            {
//                new (this) Stop;
//            } else
//            {
//                data->pastTime += ((clock() - data->pastTime)/SLOW_FACTOR);
//            }
//        }
//    };
//
//    struct Stop: public TimerOfConveyor {
//        virtual void transact() {
//            if (data->currentSpeed == TIMER_FAST)
//            {
//                new (this) Fast;
//            } else if (data->currentSpeed == TIMER_SLOW)
//            {
//                new (this) Slow;
//            }
//        }
//    };
//
//	static ContextTimer* instance_;
//	StateStart stateMember; //The memory for the state is part of context object
//	Datact ctdata;
//
//	ContextTimer() :
//			statePtr(&stateMember), ctdata() // assigning start state
//	{
//		statePtr->data = &ctdata;
//	}
//	ContextTimer(const ContextTimer& other);
//	ContextTimer& operator=(const ContextTimer& other);
//};
//
//#endif /* CONTEXTTIMER_H_ */
