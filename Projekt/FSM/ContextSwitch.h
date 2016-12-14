/*
 * ContextSwitch.h
 *
 *  Created on: 28.11.2016
 *      Author: abs949
 */

#ifndef CONTEXTSWITCH_H_
#define CONTEXTSWITCH_H_

#include <iostream>
#include "Hal/HalBuilder.h"
#include "Switchtimer_Thread.h"

struct Datacsw{
    HalBuilder hb;
    int openCounter;
    Switchtimer_Thread swt;

	Datacsw(): hb(), openCounter(0), swt(60){
	}
};

class ContextSwitch {
public:
	static ContextSwitch* getInstance();
    virtual ~ContextSwitch(){};

    void transact() {
		statePtr->transact();
	} // context delegates signals to state
    
    void setSwitchOpen()
    {
        cswdata.openCounter++;
    }

    void resetSwitchOpen()
    {
        cswdata.openCounter--;
    }

private:
	struct SwitchOfConveyor { //top-level state
		Datacsw* data;
		virtual void transact() {
		}
	}*statePtr;   // a pointer to current state. Used for polymorphism.

    struct StateStart: public SwitchOfConveyor {
        virtual void transact() {
            LOG_DEBUG << "Statestart reached." << endl;
            data->hb.getHardware()->getMotor()->closedSwitch();
            if (data->openCounter > 0)
            {
                data->hb.getHardware()->getMotor()->openSwitch();
                data->swt.continueTimer();
                new (this) Open;
            }
            else
            {
                data->hb.getHardware()->getMotor()->closedSwitch();
                new (this) Close;
            }
        }
    };

	struct Close: public SwitchOfConveyor {
		virtual void transact() {
		    if(data->openCounter > 0)
		    {
		        data->hb.getHardware()->getMotor()->openSwitch();
		        data->swt.continueTimer();
		        new (this) Open;
		    }
		}
	};

	struct Open: public SwitchOfConveyor {
		virtual void transact() {
		    if(data->openCounter == 0)
		    {
		        data->hb.getHardware()->getMotor()->closedSwitch();
		        data->swt.stopTimer();
		        new (this) Close;
		    }
		}
	};

	static ContextSwitch* instance_;
	StateStart stateMember; //The memory for the state is part of context object
	Datacsw cswdata;

	ContextSwitch() :
			statePtr(&stateMember), cswdata() // assigning start state
	{
		statePtr->data = &cswdata;
		cswdata.swt.start(NULL);
	}
	ContextSwitch(const ContextSwitch& other);
	ContextSwitch& operator=(const ContextSwitch& other);
};

#endif /* CONTEXTSWITCH_H_ */
