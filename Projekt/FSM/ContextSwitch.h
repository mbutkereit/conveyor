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

struct Datacsw{
	Datacsw(): hb(), openCounter(0), closeCounter(0){}
	HalBuilder hb;
	int openCounter;
	int closeCounter;
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
            data->hb.getHardware()->getMotor()->switchClosed();
            if (data->openCounter > 0)
            {
                data->hb.getHardware()->getMotor()->switchOpen();
                new (this) Open;
            }
            else
            {
                data->hb.getHardware()->getMotor()->switchClosed();
                new (this) Close;
            }
        }
    };

	struct Close: public SwitchOfConveyor {
		virtual void transact() {
		    if(data->openCounter > 0)
		    {
		        data->hb.getHardware()->getMotor()->switchOpen();
		        data->hb.getHardware()->getMT()->isSwitchOpen();
		        new (this) Open;
		    }
		}
	};

	struct Open: public SwitchOfConveyor {
		virtual void transact() {
		    if(data->openCounter == 0)
		    {
		        data->hb.getHardware()->getMotor()->switchClosed();
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
	}
	ContextSwitch(const ContextSwitch& other);
	ContextSwitch& operator=(const ContextSwitch& other);
};

#endif /* CONTEXTSWITCH_H_ */
