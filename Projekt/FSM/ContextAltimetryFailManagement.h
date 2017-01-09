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
	Datacafm(ContextTimeout* cto1, ContextTimeout* cto2, ContextTimeout* cto3, int* delta1, int* delta2, int* delta3) : hb(), finished(false), puckAdded(false), cafmto1(cto1), cafmto2(cto2), cafmto3(cto3), cafmdelta1(delta1), cafmdelta2(delta2), cafmdelta3(delta3) {
	}
	HalBuilder hb;
	bool finished;
	bool puckAdded;
	ContextTimeout* cafmto1;
	ContextTimeout* cafmto2;
	ContextTimeout* cafmto3;
	int* cafmdelta1;
	int* cafmdelta2;
	int* cafmdelta3;
};

class ContextAltimetryFailManagement {
public:
	ContextAltimetryFailManagement(ContextTimeout*,ContextTimeout*,ContextTimeout*,int*,int*,int*);
	virtual ~ContextAltimetryFailManagement();

	bool isContextimEnzustand() {
		if (cafm.finished){
			LOG_DEBUG << "Ich bin jetzt im Endzustand \n";
		}
		return cafm.finished;
	}

	bool puckAdded(){
	    return cafm.puckAdded;
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

	struct Puck2Ready_AFM: public AltimetryFailManagement {
		virtual void signalAltimetryInterrupted(){
			data->cafmto2->stopTimerT0();
			//if(*data->cafmdelta2 <= TOLERANCE){
			if(1){
				*data->cafmdelta2 = DELTA_TH_TE;
				data->cafmto2->startTimerTH();
				new (this) Puck3Ready_AFM;
			}else{
				data->puckAdded = true;
			}
		}
	};

	struct Puck3Ready_AFM: public AltimetryFailManagement{
		virtual void signalAltimetryInterrupted(){
			data->cafmto3->stopTimerT0();
			//if(*data->cafmdelta3 <= TOLERANCE){
			if(1){
				*data->cafmdelta3 = DELTA_TH_TE;
				data->cafmto3->startTimerTH();
				data->finished = true;
				new (this) EndOfTheEnd;
			}else{
				data->puckAdded = true;
			}
		}
	};

	struct EndOfTheEnd: public AltimetryFailManagement{
	};

	/**
	struct Puck3Recognized_AFM: public AltimetryFailManagement{
		virtual void signalAltimetryNotInterrupted(){
			if(0){//TODO Delta not ok
				data->puckAdded = true;
			}
			data->finished = true;
		}
	};
	*/

	Puck2Ready_AFM stateMember; //The memory for the state is part of context object
	Datacafm cafm;
};

#endif /* CONTEXTALTIMETRYFAILMANAGEMENT_H_ */
