/*
 * ContextTimeMeasurement.h
 *
 *  Created on: 23.11.2016
 *      Author: abs949
 */

#ifndef CONTEXTTIMEMEASUREMENT_H_
#define CONTEXTTIMEMEASUREMENT_H_

#include <iostream>
#include <ctime>
#include <unistd.h>
#include "Logger/Logger.h"
#include "Hal/HalBuilder.h"
using namespace std;

extern HalBuilder hb;

struct Datactm {
	Datactm() :
			hb(), cs(getInstance()), puck(-1), puckqueue(), es(), id(0), t1(0), t2(0), delta(0), deltaT0TH(0), deltaTHTW(0), deltaTWTE(0) {
	}
	HalBuilder hb;
	clock_t t1;
	clock_t t2;
	clock_t delta;
	clock_t deltaT0TH;
	clock_t deltaTHTW;
	clock_t deltaTWTE;
};

class ContextTimeMeasurement {
public:
	ContextTimeMeasurement() :
			statePtr(&stateMember), ctmdata() // assigning start state
	{
		statePtr->data = &ctmdata;
	}
	~ContextTimeMeasurement(){};

	void transact() {
		statePtr->transact();
	} // context delegates signals to state


private:
	struct Timereading { //top-level state
		Datactm* data;
		virtual void transact() {
		}
	}*statePtr;   // a pointer to current state. Used for polymorphism.

	struct StateStart: public Timereading {
		virtual void transact() {
			while(!(data->hb.getHardware()->getMT()->isItemRunningIn())){}
			new (this) PuckNotInEntry;
		}
	};

	struct PuckNotInEntry: public Timereading {
		virtual void transact() {
			data->hb.getHardware()->getMotor()->stop();
			while(data->hb.getHardware()->getMT()->isItemRunningIn()){}
			new (this) MotorOn;
		}
	};

	struct MotorOn: public Timereading {
		virtual void transact() {
			data->hb.getHardware()->getMotor()->right();
			data->hb.getHardware()->getMotor()->fast();
			while(!(data->hb.getHardware()->getMT()->isItemRunningIn())){}
			data->t1 = clock();
			new (this) TransportToHeightMeasurement;
		}
	};
	struct TransportToHeightMeasurement: public Timereading {
		virtual void transact() {
			while(data->hb.getHardware()->getMT()->isItemAltimetry()){}
			data->t2 = clock();
			data->deltaT0TH = data->t2 - data->t1;
			data->hb.getHardware()->getMotor()->slow();
			new (this) PuckInHeightMeasurement;
		}
	};
	struct PuckInHeightMeasurement: public Timereading {
		virtual void transact() {
			while(!(data->hb.getHardware()->getMT()->isItemAltimetry())){}
			data->hb.getHardware()->getMotor()->fast();
		}
	};
	struct TransPortToSkid: public Timereading {
		virtual void transact() {
			while(data->hb.getHardware()->getMT()->isItemSwitch()){}
			data->t1 = data->t2;
			data->t2 = clock();
			data->deltaTHTW = data->t2 - data->t1;
			new (this) PuckInSkid;
		}
	};
	struct PuckInSkid: public Timereading {
		virtual void transact() {
			data->hb.getHardware()->getMotor()->switchOpen();
			while(data->hb.getHardware()->getMT()->isItemRunningOut()){}
			data->t1 = data->t2;
			data->t2 = clock();
			data->deltaTWTE = data->t2 - data->t1;
			new (this) PuckRunningOut;
		}
	};
	struct PuckRunningOut: public Timereading {
		virtual void transact() {
			data->hb.getHardware()->getMotor()->switchClosed();
			data->hb.getHardware()->getMotor()->stop();
			printf("Delta t_0 und t_H: %d", data->deltaT0TH);
			printf("Delta t_H und t_W: %d", data->deltaTHTW);
			printf("Delta t_W und t_E: %d", data->deltaTWTE);
			new (this) Finish;
		}
	};
	struct PuckRunningOut: public Timereading {
		virtual void transact() {
		}
	};


	StateStart stateMember; //The memory for the state is part of context object
	Datactm ctmdata;
};
#endif /* CONTEXTTIMEMEASUREMENT_H_ */
