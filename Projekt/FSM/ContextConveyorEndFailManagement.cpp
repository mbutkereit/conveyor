/*
 * ContextConveyorEndFailManagement.cpp
 *
 *  Created on: 19.12.2016
 *      Author: abs949
 */

#include "ContextConveyorEndFailManagement.h"

ContextConveyorEndFailManagement::ContextConveyorEndFailManagement(ContextTimeout* cto1, ContextTimeout* cto2, ContextTimeout* cto3,  int* delta1, int* delta2, int* delta3) :
			statePtr(&stateMember), ccefm(cto1, cto2, cto3, delta1, delta2, delta3) // assigning start state
	{
		statePtr->data = &ccefm;
	}

ContextConveyorEndFailManagement::~ContextConveyorEndFailManagement(){};

void ContextConveyorEndFailManagement::signalLBBeginInterrupted(){
	statePtr->signalLBBeginInterrupted();
}

void ContextConveyorEndFailManagement::signalLBBeginNotInterrupted(){
	statePtr->signalLBBeginNotInterrupted();
}

void ContextConveyorEndFailManagement::signalLBEndInterrupted()  {
	statePtr->signalLBEndInterrupted();
}
void ContextConveyorEndFailManagement::signalLBEndNotInterrupted()  {
	statePtr->signalLBEndNotInterrupted();
}

void ContextConveyorEndFailManagement::signalLBAltimetryInterrupted(){
	statePtr->signalLBAltimetryInterrupted();
}
void ContextConveyorEndFailManagement::signalLBAltimetryNotInterrupted(){
	statePtr->signalLBAltimetryNotInterrupted();
}

void ContextConveyorEndFailManagement::signalLBSwitchInterrupted(){
	statePtr->signalLBSwitchInterrupted();
}
void ContextConveyorEndFailManagement::signalLBSwitchNotInterrupted(){
	statePtr->signalLBSwitchNotInterrupted();
}

void ContextConveyorEndFailManagement::signalEStop(){
	statePtr->signalEStop();
}
void ContextConveyorEndFailManagement::signalStart(){
	statePtr->signalStart();
}

void ContextConveyorEndFailManagement::signalStop(){
	statePtr->signalStop();
}
void ContextConveyorEndFailManagement::signalReset(){
	statePtr->signalReset();
}

void ContextConveyorEndFailManagement::signalLBSkidInterrupted(){
	statePtr->signalLBSkidInterrupted();
}
void ContextConveyorEndFailManagement::signalLBSkidNotInterrupted(){
	statePtr->signalLBSkidNotInterrupted();
}

void ContextConveyorEndFailManagement::signalAltimetryCompleted(){
	statePtr->signalAltimetryCompleted();
}

void ContextConveyorEndFailManagement::signalLBNextConveyor(){
	statePtr->signalLBNextConveyor();
}

void ContextConveyorEndFailManagement::signalTimerTick(){
    statePtr->signalTimerTick();
}
