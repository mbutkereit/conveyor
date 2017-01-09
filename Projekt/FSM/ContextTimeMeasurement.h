/*
 * ContextTimeMeasurement.h
 *
 *  Created on: 23.11.2016
 *      Author: abs949
 */

#ifndef CONTEXTTIMEMEASUREMENT_H_
#define CONTEXTTIMEMEASUREMENT_H_

#include <iostream>
#include <unistd.h>
#include <iostream>
#include "Logger/Logger.h"
#include "Hal/HalBuilder.h"
#include "ContextMotor.h"
#include "ContextSwitch.h"
#include "ContextI.h"

struct Datactm {
	Datactm() :
			hb(), cm(ContextMotor::getInstance()), cswitch(ContextSwitch::getInstance()), finished(false) {
	}
	HalBuilder hb;
	ContextMotor* cm;
	ContextSwitch* cswitch;
	bool finished;
};

class ContextTimeMeasurement :public ContextI {
public:
	ContextTimeMeasurement();
	virtual ~ContextTimeMeasurement();

	bool isContextimEnzustand() {
		if (ctmdata.finished){
			LOG_DEBUG << "Ich bin jetzt im Endzustand \n";
		}
		return ctmdata.finished;
	}

	void signalLBBeginInterrupted();

	void signalLBEndInterrupted();

	void signalLBAltimetryInterrupted();

	void signalLBSwitchInterrupted();

	void signalLBBeginNotInterrupted();

	void signalLBEndNotInterrupted();

	void signalLBAltimetryNotInterrupted();

	void signalLBSwitchNotInterrupted();

	void signalEStop();

	void signalStart();

	void signalStop();

	void signalReset();

	void signalLBSkidInterrupted();

	void signalLBSkidNotInterrupted();

	void signalAltimetryCompleted();

	void sensorMeasurementCompleted();

	void signalLBNextConveyor();

	void signalTimerTick();

private:
	struct Timereading { //top-level state
		virtual void signalLBBeginInterrupted() {
		}
		virtual void signalLBEndInterrupted() {
		}
		virtual void signalLBAltimetryInterrupted() {
		}
		virtual void signalLBSwitchInterrupted() {
		}
		virtual void signalLBBeginNotInterrupted() {
		}
		virtual void signalLBEndNotInterrupted() {
		}
		virtual void signalLBAltimetryNotInterrupted() {
		}
		virtual void signalLBSwitchNotInterrupted() {
		}
		virtual void signalEStop() {
		}
		virtual void signalStart() {
		}
		virtual void signalStop() {
		}
		virtual void signalReset() {
		}
		virtual void signalLBSkidInterrupted() {
		}
		virtual void signalLBSkidNotInterrupted() {
		}
		virtual void signalAltimetryCompleted() {
		}
		virtual void sensorMeasurementCompleted() {
		}
		virtual void signalLBNextConveyor() {
		}
		virtual void signalTimerTick(){
		}

		Datactm* data;
	}*statePtr;   // a pointer to current state. Used for polymorphism.

	struct StateStart: public Timereading {

	};

	StateStart stateMember; //The memory for the state is part of context object
	Datactm ctmdata;
};
#endif /* CONTEXTTIMEMEASUREMENT_H_ */
