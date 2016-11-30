/*
 * ContextMotor.h
 *
 *  Created on: 29.11.2016
 *      Author: abs949
 */

#ifndef CONTEXTMOTOR_H_
#define CONTEXTMOTOR_H_

#include <iostream>
#include "Hal/HalBuilder.h"
#include "MotorOptions.h"
#include "TimerOptions.h"
#include "ContextTimer.h"

struct Datacm{
	Datacm(ContextTimer* ctimer): hb(), ct(ctimer), fastFlag(0), slowCounter(0), stopCounter(0){}
	    HalBuilder hb;
	    ContextTimer* ct;
		int fastFlag;
		int slowCounter;
		int stopCounter;
};


class ContextMotor {


private:
    struct MotorOfConveyor { //top-level state
        Datacm* data;
        virtual void transact() {
        }
    }*statePtr;   // a pointer to current state. Used for polymorphism.

    struct StateStart: public MotorOfConveyor {
        virtual void transact() {
            data->hb.getHardware()->getMotor()->stop();
            if (data->stopCounter > 0)
            {
                data->ct->setTimerSpeed(TIMER_STOP);
                data->ct->transact();
                data->hb.getHardware()->getMotor()->stop();
                new (this) Stop;
            }
            else if (data->slowCounter > 0)
            {
                data->ct->setTimerSpeed(TIMER_SLOW);
                data->ct->transact();
                data->hb.getHardware()->getMotor()->right();
                data->hb.getHardware()->getMotor()->slow();
                new (this) Slow;
            }
            else if (data->fastFlag)
            {
                data->ct->setTimerSpeed(TIMER_FAST);
                data->ct->transact();
                data->fastFlag = 0;
                data->hb.getHardware()->getMotor()->right();
                data->hb.getHardware()->getMotor()->fast();
                new (this) Fast;
            }
        }
    };

    struct Fast: public MotorOfConveyor {
        virtual void transact() {
            if (data->stopCounter > 0)
            {
                data->ct->setTimerSpeed(TIMER_STOP);
                data->ct->transact();
                data->hb.getHardware()->getMotor()->stop();
                new (this) Stop;
            }
            else if (data->slowCounter > 0)
            {
                data->ct->setTimerSpeed(TIMER_SLOW);
                data->ct->transact();
                data->hb.getHardware()->getMotor()->right();
                data->hb.getHardware()->getMotor()->slow();
                new (this) Slow;
            }
        }
    };

    struct Slow: public MotorOfConveyor {
        virtual void transact() {
            if (data->stopCounter > 0)
            {
                data->ct->setTimerSpeed(TIMER_STOP);
                data->ct->transact();
                data->hb.getHardware()->getMotor()->stop();
                new (this) Stop;
            }
            else if (data->slowCounter == 0)
            {
                data->ct->setTimerSpeed(TIMER_FAST);
                data->ct->transact();
                data->hb.getHardware()->getMotor()->right();
                data->hb.getHardware()->getMotor()->fast();
                new (this) Fast;
            }
        }
    };

    struct Stop: public MotorOfConveyor {
        virtual void transact() {
            if (data->stopCounter == 0)
            {
                if (data->slowCounter > 0)
                {
                    data->ct->setTimerSpeed(TIMER_SLOW);
                    data->ct->transact();
                    data->hb.getHardware()->getMotor()->right();
                    data->hb.getHardware()->getMotor()->slow();
                    new (this) Slow;
                }
                else if (data->slowCounter == 0)
                {
                    data->ct->setTimerSpeed(TIMER_FAST);
                    data->ct->transact();
                    data->hb.getHardware()->getMotor()->right();
                    data->hb.getHardware()->getMotor()->fast();
                    new (this) Fast;
                }
            }
        }
    };

    static ContextMotor* instance_;
    StateStart stateMember; //The memory for the state is part of context object
    Datacm cmdata;
    ContextTimer* ct; //<=== kann vermutlich weg

    ContextMotor(ContextTimer* ctimer) :
            statePtr(&stateMember), cmdata(ctimer), ct(ctimer) // assigning start state
    {
        statePtr->data = &cmdata;
    }
    ContextMotor(const ContextMotor& other);
    ContextMotor& operator=(const ContextMotor& other);

public:
	static ContextMotor* getInstance();

	void transact() {
		statePtr->transact();
	} // context delegates signals to state

	void setSpeed(MotorOptions mo) {
		switch (mo) {
		case MOTOR_FAST:
			cmdata.fastFlag = 1;
			break;
		case MOTOR_SLOW:
			cmdata.slowCounter++;
			break;
		default:
			cmdata.stopCounter++;
			break;
		}
	}

	void resetSpeed(MotorOptions mo)
	{
		switch (mo) {
		case MOTOR_SLOW:
			cmdata.slowCounter--;
			break;
		default:
			cmdata.stopCounter--;
			break;
		}
	}

	virtual ~ContextMotor(){};
};

#endif /* CONTEXTMOTOR_H_ */
