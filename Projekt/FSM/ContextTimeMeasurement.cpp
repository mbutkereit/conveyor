/*
 * ContextTimemeasurement.cpp
 *
 *  Created on: 15.12.2016
 *      Author: abs949
 */
#include "ContextTimeMeasurement.h"

ContextTimeMeasurement::ContextTimeMeasurement() :
			statePtr(&stateMember), ctmdata() // assigning start state
	{
		statePtr->data = &ctmdata;
	}

ContextTimeMeasurement::~ContextTimeMeasurement(){};

void ContextTimeMeasurement::signalLBBeginInterrupted(){
	statePtr->signalLBBeginInterrupted();
}

void ContextTimeMeasurement::signalLBBeginNotInterrupted(){
	statePtr->signalLBBeginNotInterrupted();
}

void ContextTimeMeasurement::signalLBEndInterrupted()  {
	statePtr->signalLBEndInterrupted();
}
void ContextTimeMeasurement::signalLBEndNotInterrupted()  {
	statePtr->signalLBEndNotInterrupted();
}

void ContextTimeMeasurement::signalLBAltimetryInterrupted(){
	statePtr->signalLBAltimetryInterrupted();
}
void ContextTimeMeasurement::signalLBAltimetryNotInterrupted(){
	statePtr->signalLBAltimetryNotInterrupted();
}

void ContextTimeMeasurement::signalLBSwitchInterrupted(){
	statePtr->signalLBSwitchInterrupted();
}
void ContextTimeMeasurement::signalLBSwitchNotInterrupted(){
	statePtr->signalLBSwitchNotInterrupted();
}

void ContextTimeMeasurement::signalEStop(){
	statePtr->signalEStop();
}
void ContextTimeMeasurement::signalStart(){
	statePtr->signalStart();
}

void ContextTimeMeasurement::signalStop(){
	statePtr->signalStop();
}
void ContextTimeMeasurement::signalReset(){
	statePtr->signalReset();
}

void ContextTimeMeasurement::signalLBSkidInterrupted(){
	statePtr->signalLBSkidInterrupted();
}
void ContextTimeMeasurement::signalLBSkidNotInterrupted(){
	statePtr->signalLBSkidNotInterrupted();
}

void ContextTimeMeasurement::signalAltimetryCompleted(){
	statePtr->signalAltimetryCompleted();
}

void ContextTimeMeasurement::signalLBNextConveyor(){
	statePtr->signalLBNextConveyor();
}

void ContextTimeMeasurement::signalTimerTick(){
    statePtr->signalTimerTick();
}
