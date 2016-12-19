/*
 * ContextAltimetryFailManagement.h
 *
 *  Created on: 19.12.2016
 *      Author: abs949
 */

#ifndef CONTEXTALTIMETRYFAILMANAGEMENT_H_
#define CONTEXTALTIMETRYFAILMANAGEMENT_H_


#include <iostream>
#include <unistd.h>
#include <iostream>
#include "Logger/Logger.h"
#include "Hal/HalBuilder.h"
#include "ContextTimeout.h"

struct Datacafm {
	Datacafm(ContextTimeout* cto1, ContextTimeout* cto2, ContextTimeout* cto3) : hb(), finished(false), puckAdded(false), cafmto1(cto1), cafmto2(cto2), cafmto3(cto3) {
	}
	HalBuilder hb;
	bool finished;
	bool puckAdded;
	ContextTimeout* cafmto1;
	ContextTimeout* cafmto2;
	ContextTimeout* cafmto3;
};

class ContextAltimetryFailManagement {
public:
	ContextAltimetryFailManagement(ContextTimeout*,ContextTimeout*,ContextTimeout*);
	virtual ~ContextAltimetryFailManagement();

	bool isContextimEnzustand() {
		if (cafm.finished){
			LOG_DEBUG << "Ich bin jetzt im Endzustand \n";
		}
		return cafm.finished;
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
	struct AltimetryFailManagement { //top-level state
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
			//TODO Delta decrement for PuckAdded
		}

		Datacafm* data;
	}*statePtr;   // a pointer to current state. Used for polymorphism.

	struct Puck1Recognized_AFM: public AltimetryFailManagement {
		virtual void signalLBAltimetryNotInterrupted() {
			//TODO Delta calculation
			if(0){//TODO Delta not ok
				data->puckAdded = true;
			}
			else{
				new (this) Puck2Ready_AFM;
			}
		}
	};

	struct Puck2Ready_AFM: public AltimetryFailManagement {
		virtual void signalAltimetryInterrupted(){
			data->cafmto2->stopTimerT0();
			//TODO th_2, Delta of t_02 and th_2
			data->cafmto2->startTimerTH();
			new (this) Puck2Recognized_AFM;
		}
	};

	struct Puck2Recognized_AFM: public AltimetryFailManagement{
		virtual void signalAltimetryNotInterrupted(){
			if(0){//TODO Delta not ok
				data->puckAdded = true;
			}
			else{
				new (this) Puck3Ready_AFM;
			}
		}
	};

	struct Puck3Ready_AFM: public AltimetryFailManagement{
		virtual void signalAltimetryInterrupted(){
			data->cafmto3->stopTimerT0();
			//TODO th_3, Delta of t_03 and th_3
			data->cafmto3->startTimerTH();
			new (this) Puck3Recognized_AFM;
		}
	};

	struct Puck3Recognized_AFM: public AltimetryFailManagement{
		virtual void signalAltimetryNotInterrupted(){
			if(0){//TODO Delta not ok
				data->puckAdded = true;
			}
			data->finished = true;
		}
	};

	Puck1Recognized_AFM stateMember; //The memory for the state is part of context object
	Datacafm cafm;
};

#endif /* CONTEXTALTIMETRYFAILMANAGEMENT_H_ */
