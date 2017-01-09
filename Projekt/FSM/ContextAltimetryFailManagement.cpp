/*
 * ContextAltimetryFailManagement.cpp
 *
 *  Created on: 19.12.2016
 *      Author: abs949
 */

#include "ContextAltimetryFailManagement.h"

ContextAltimetryFailManagement::ContextAltimetryFailManagement(ContextTimeout* cto1, ContextTimeout* cto2, ContextTimeout* cto3, int* delta1, int* delta2, int* delta3) :
			statePtr(&stateMember), cafm(cto1,cto2,cto3,delta1,delta2,delta3) // assigning start state
	{
		statePtr->data = &cafm;
	}

ContextAltimetryFailManagement::~ContextAltimetryFailManagement(){};

void ContextAltimetryFailManagement::signalLBBeginInterrupted(){
	statePtr->signalLBBeginInterrupted();
}

void ContextAltimetryFailManagement::signalLBBeginNotInterrupted(){
	statePtr->signalLBBeginNotInterrupted();
}

void ContextAltimetryFailManagement::signalLBEndInterrupted()  {
	statePtr->signalLBEndInterrupted();
}
void ContextAltimetryFailManagement::signalLBEndNotInterrupted()  {
	statePtr->signalLBEndNotInterrupted();
}

void ContextAltimetryFailManagement::signalLBAltimetryInterrupted(){
	statePtr->signalLBAltimetryInterrupted();
}
void ContextAltimetryFailManagement::signalLBAltimetryNotInterrupted(){
	statePtr->signalLBAltimetryNotInterrupted();
}

void ContextAltimetryFailManagement::signalLBSwitchInterrupted(){
	statePtr->signalLBSwitchInterrupted();
}
void ContextAltimetryFailManagement::signalLBSwitchNotInterrupted(){
	statePtr->signalLBSwitchNotInterrupted();
}

void ContextAltimetryFailManagement::signalEStop(){
	statePtr->signalEStop();
}
void ContextAltimetryFailManagement::signalStart(){
	statePtr->signalStart();
}

void ContextAltimetryFailManagement::signalStop(){
	statePtr->signalStop();
}
void ContextAltimetryFailManagement::signalReset(){
	statePtr->signalReset();
}

void ContextAltimetryFailManagement::signalLBSkidInterrupted(){
	statePtr->signalLBSkidInterrupted();
}
void ContextAltimetryFailManagement::signalLBSkidNotInterrupted(){
	statePtr->signalLBSkidNotInterrupted();
}

void ContextAltimetryFailManagement::signalAltimetryCompleted(){
	statePtr->signalAltimetryCompleted();
}

void ContextAltimetryFailManagement::signalLBNextConveyor(){
	statePtr->signalLBNextConveyor();
}

void ContextAltimetryFailManagement::signalTimerTick(){
    statePtr->signalTimerTick();
}
