/*
 * ContextConveyorEndFailManagement.h
 *
 *  Created on: 19.12.2016
 *      Author: abs949
 */

#ifndef CONTEXTCONVEYORENDFAILMANAGEMENT_H_
#define CONTEXTCONVEYORENDFAILMANAGEMENT_H_

#include <iostream>
#include <unistd.h>
#include <iostream>
#include "Logger/Logger.h"
#include "Hal/HalBuilder.h"
#include "ContextMotor.h"
#include "ContextSwitch.h"
#include "ContextTimeout.h"

struct Dataccefm {
	Dataccefm(ContextTimeout* cto1, ContextTimeout* cto2, ContextTimeout* cto3) :
			hb(), cswitch(ContextSwitch::getInstance()), finished(false), puckAdded(false), ccefmto1(cto1), ccefmto2(cto2), ccefmto3(cto3) {
	}
	HalBuilder hb;
	ContextSwitch* cswitch;
	bool finished;
	bool puckAdded;
	ContextTimeout* ccefmto1;
	ContextTimeout* ccefmto2;
	ContextTimeout* ccefmto3;
};

class ContextConveyorEndFailManagement{
public:
	ContextConveyorEndFailManagement(ContextTimeout*,ContextTimeout*,ContextTimeout*);
	virtual ~ContextConveyorEndFailManagement();

	bool isContextimEnzustand() {
		if (ccefm.finished){
			LOG_DEBUG << "Ich bin jetzt im Endzustand \n";
		}
		return ccefm.finished;
	}

	bool puckAdded(){
	    return ccefm.puckAdded;
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
	struct ConveyorEndFailManagement { //top-level state
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

		Dataccefm* data;
	}*statePtr;   // a pointer to current state. Used for polymorphism.

	struct Puck1Recognized_CEFM: public ConveyorEndFailManagement {
		virtual void signalLBEndNotInterrupted() {
			if(0){//TODO Delta not ok
				data->puckAdded = true;
			}
			else{
				new (this) Puck2Ready_CEFM;
			}
		}
	};

	struct Puck2Ready_CEFM: public ConveyorEndFailManagement {
		virtual void signalLBEndInterrupted(){
			data->ccefmto2->stopTimerTH();
			//TODO t_E2, Delta of t_H2 and t_E2
			new (this) Puck2Recognized_CEFM;
		}
	};

	struct Puck2Recognized_CEFM: public ConveyorEndFailManagement {
		virtual void signalLBEndNotInterrupted(){
			if(0){//TODO Delta not ok
				data->puckAdded = true;
			}
			else{
				new (this) Puck3Ready_CEFM;
			}
		}
	};

	struct Puck3Ready_CEFM: public ConveyorEndFailManagement {
		virtual void signalLBEndInterrupted(){
			data->ccefmto3->stopTimerTH();
			//TODO t_E3, Delta of t_H3 and t_E3
			new (this) Puck3Recognized_CEFM;
		}
	};

	struct Puck3Recognized_CEFM: public ConveyorEndFailManagement {
		virtual void signalLBEndNotInterrupted(){
			if(0){//TODO Delta not ok
				data->puckAdded = true;
			}
			data->finished = true;
		}
	};

	Puck1Recognized_CEFM stateMember; //The memory for the state is part of context object
	Dataccefm ccefm;
};
#endif /* CONTEXTCONVEYORENDFAILMANAGEMENT_H_ */
