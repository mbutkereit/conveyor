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
#include "Timer.h"

struct Datacm{
	Datacm(): hb(), timer(Timer::getInstance()), fastFlag(0), slowCounter(0), stopCounter(0){}
	    HalBuilder hb;
	    Timer* timer;
		int fastFlag;
		int slowCounter;
		int stopCounter;
};


class ContextMotor {


private:
	void transact() {
		statePtr->transact();
	} // context delegates signals to state

    struct MotorOfConveyor { //top-level state
        Datacm* data;
        virtual void transact() {
        }
    }*statePtr;   // a pointer to current state. Used for polymorphism.

    struct StateStart: public MotorOfConveyor {
        virtual void transact() {
        	LOG_DEBUG << "Statestart State\n";
            data->hb.getHardware()->getMotor()->stop();
            if (data->stopCounter > 0)
            {
            	LOG_DEBUG << "StateStart State: Switch to Stop\n";
                data->timer->setMode(TIMER_STOP);
                data->hb.getHardware()->getMotor()->stop();
                data->timer->startTimer();
                new (this) Stop;
            }
            else if (data->slowCounter > 0)
            {
            	LOG_DEBUG << "StateStart State: Switch to Slow\n";
                data->timer->setMode(TIMER_SLOW);
                data->hb.getHardware()->getMotor()->right();
                data->hb.getHardware()->getMotor()->slow();
                data->timer->startTimer();
                new (this) Slow;
            }
            else if (data->fastFlag)
            {
            	LOG_DEBUG << "StateStart State: Switch to Fast\n";
                data->timer->setMode(TIMER_FAST);
                data->fastFlag = 0;
                data->hb.getHardware()->getMotor()->right();
                data->hb.getHardware()->getMotor()->fast();
                data->timer->startTimer();
                new (this) Fast;
            }
        }
    };

    struct Fast: public MotorOfConveyor {
        virtual void transact() {
        	LOG_DEBUG << "Fast State\n";
            if (data->stopCounter > 0)
            {
            	LOG_DEBUG << "Fast State: Switch to Stop\n";
                data->timer->setMode(TIMER_STOP);
                data->timer->startTimer();
                data->hb.getHardware()->getMotor()->stop();
                new (this) Stop;
            }
            else if (data->slowCounter > 0)
            {
            	LOG_DEBUG << "Fast State: Switch to Slow\n";
                data->timer->setMode(TIMER_SLOW);
                data->timer->startTimer();
                data->hb.getHardware()->getMotor()->right();
                data->hb.getHardware()->getMotor()->slow();
                new (this) Slow;
            }
        }
    };

    struct Slow: public MotorOfConveyor {
        virtual void transact() {
        	LOG_DEBUG << "Slow State\n";
            if (data->stopCounter > 0)
            {
            	LOG_DEBUG << "Slow State: Switch to Stop\n";
                data->timer->setMode(TIMER_STOP);
                data->timer->stopTimer();
                data->hb.getHardware()->getMotor()->stop();
                new (this) Stop;
            }
            else if (data->slowCounter == 0)
            {
            	LOG_DEBUG << "Slow State: Switch to Fast\n";
                data->timer->setMode(TIMER_FAST);
                data->timer->startTimer();
                data->hb.getHardware()->getMotor()->right();
                data->hb.getHardware()->getMotor()->fast();
                new (this) Fast;
            }
        }
    };

    struct Stop: public MotorOfConveyor {
        virtual void transact() {
        	LOG_DEBUG << "Stop State\n";
            if (data->stopCounter == 0)
            {
                if (data->slowCounter > 0)
                {
                	LOG_DEBUG << "Stop State: Switch to Slow\n";
                    data->timer->setMode(TIMER_SLOW);
                    data->timer->continueTimer();
                    data->hb.getHardware()->getMotor()->right();
                    data->hb.getHardware()->getMotor()->slow();
                    new (this) Slow;
                }
                else
                {
                	LOG_DEBUG << "Stop State: Switch to Fast\n";
                    data->timer->setMode(TIMER_FAST);
                    data->timer->continueTimer();
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

    ContextMotor() :
            statePtr(&stateMember), cmdata()// assigning start state
    {
        statePtr->data = &cmdata;
    }
    ContextMotor(const ContextMotor& other);
    ContextMotor& operator=(const ContextMotor& other);

public:
	static ContextMotor* getInstance();

	int showStopCounter(){
		return cmdata.stopCounter;
	}

	void setSpeed(MotorOptions mo) {
		switch (mo) {
		case MOTOR_FAST:
			cmdata.fastFlag = 1;
			break;
		case MOTOR_SLOW:
			cmdata.slowCounter++;
			break;
		case MOTOR_STOP:
		default:
			cmdata.stopCounter++;
			break;
		}
		transact();
	}

	void resetSpeed(MotorOptions mo)
	{
		switch (mo) {
		case MOTOR_SLOW:
			cmdata.slowCounter--;
			break;
		case MOTOR_STOP:
		default:
			cmdata.stopCounter--;
			break;
		}
		transact();
	}

	virtual ~ContextMotor(){};
};

#endif /* CONTEXTMOTOR_H_ */
