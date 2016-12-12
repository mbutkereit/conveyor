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
#include <iostream>
#include "Logger/Logger.h"
#include "Hal/HalBuilder.h"
using namespace std;

extern HalBuilder hb;

struct Datactm {
	Datactm() :
			hb(), finishFlag(0), t1(0), t2(0), delta(0), deltaT0TH(0), deltaTHTW(0), deltaTWTE(0) {
	}
	HalBuilder hb;
	int finishFlag;
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

	int isFinished()
	{
		return ctmdata.finishFlag;
	}


private:
	struct Timereading { //top-level state
		Datactm* data;
		virtual void transact() {
		}
	}*statePtr;   // a pointer to current state. Used for polymorphism.

	struct StateStart: public Timereading {
		virtual void transact() {
			printf("StateStart\n");
			while(!(data->hb.getHardware()->getMT()->isItemRunningIn())){}
			new (this) PuckNotInEntry;
		}
	};

	struct PuckNotInEntry: public Timereading {
		virtual void transact() {
			printf("PuckNotInEntry\n");
			data->hb.getHardware()->getMotor()->stop();
			while(data->hb.getHardware()->getMT()->isItemRunningIn()){}
			new (this) MotorOn;
		}
	};

	struct MotorOn: public Timereading {
		virtual void transact() {
			printf("MotorOn\n");
			data->hb.getHardware()->getMotor()->right();
			data->hb.getHardware()->getMotor()->fast();
			while(!(data->hb.getHardware()->getMT()->isItemRunningIn())){}
			data->t1 = clock();
			new (this) TransportToHeightMeasurement;
		}
	};
	struct TransportToHeightMeasurement: public Timereading {
		virtual void transact() {
			printf("TransportToHeightMeasurement\n");
			while(data->hb.getHardware()->getMT()->isItemAltimetry()){}
			data->t2 = clock();
			data->deltaT0TH = data->t2 - data->t1;
			data->hb.getHardware()->getMotor()->slow();
			new (this) PuckInHeightMeasurement;
		}
	};
	struct PuckInHeightMeasurement: public Timereading {
		virtual void transact() {
			printf("PuckInHeightMeasurement\n");
			while(!(data->hb.getHardware()->getMT()->isItemAltimetry())){}
			data->hb.getHardware()->getMotor()->fast();
			new (this) TransPortToSkid;
		}
	};
	struct TransPortToSkid: public Timereading {
		virtual void transact() {
			printf("TransportToSkid\n");
			while(data->hb.getHardware()->getMT()->isItemSwitch()){}
			data->t1 = data->t2;
			data->t2 = clock();
			data->deltaTHTW = data->t2 - data->t1;
			new (this) PuckInSkid;
		}
	};
	struct PuckInSkid: public Timereading {
		virtual void transact() {
			printf("PuckInSkid\n");
			data->hb.getHardware()->getMotor()->openSwitch();
			while(data->hb.getHardware()->getMT()->isItemRunningOut()){}
			data->t1 = data->t2;
			data->t2 = clock();
			data->deltaTWTE = data->t2 - data->t1;
			new (this) PuckRunningOut;
		}
	};
	struct PuckRunningOut: public Timereading {
		virtual void transact() {
			data->hb.getHardware()->getMotor()->closedSwitch();
			data->hb.getHardware()->getMotor()->stop();
			printf("Delta t_0 und t_H: Ticks: %d, Seconds: %d\n", data->deltaT0TH, data->deltaT0TH/CLOCK_REALTIME);
			printf("Delta t_H und t_W: Ticks: %d, Seconds: %d\n", data->deltaTHTW, data->deltaTHTW/CLOCK_REALTIME);
			printf("Delta t_W und t_E: Ticks: %d, Seconds: %d\n", data->deltaTWTE, data->deltaTWTE/CLOCK_REALTIME);
			new (this) Finish;
		}
	};
	struct Finish: public Timereading {
		virtual void transact() {
			printf("Finish\n");
			data->hb.getHardware()->getMotor()->stop();
			new (this) StateStart;
		}
	};

	StateStart stateMember; //The memory for the state is part of context object
	Datactm ctmdata;
};
#endif /* CONTEXTTIMEMEASUREMENT_H_ */
