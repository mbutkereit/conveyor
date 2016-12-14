/*
 * ContextTopFSM.cpp
 *
 *  Created on: 07.12.2016
 *      Author: abs949
 */

#include "ContextTopFSM3.h"

ContextTopFSM3::ContextTopFSM3(int puckID, std::vector<Puck>* puckVector) :
statePtr(&stateMember), // assigning start state
contextdata( puckID,puckVector) // initializing data
{
statePtr->data = &contextdata; // connecting state->data with the context data
}


ContextTopFSM3::~ContextTopFSM3() {}

void ContextTopFSM3::signalLBBeginInterrupted(){
	statePtr->signalLBBeginInterrupted();
}

void ContextTopFSM3::signalLBBeginNotInterrupted(){
	statePtr->signalLBBeginNotInterrupted();
}

void ContextTopFSM3::signalLBEndInterrupted()  {
	statePtr->signalLBEndInterrupted();
}
void ContextTopFSM3::signalLBEndNotInterrupted()  {
	statePtr->signalLBEndNotInterrupted();
}

void ContextTopFSM3::signalLBAltimetryInterrupted(){
	statePtr->signalLBAltimetryInterrupted();
}
void ContextTopFSM3::signalLBAltimetryNotInterrupted(){
	statePtr->signalLBAltimetryNotInterrupted();
}


void ContextTopFSM3::signalLBSwitchInterrupted(){
	statePtr->signalLBSwitchInterrupted();
}
void ContextTopFSM3::signalLBSwitchNotInterrupted(){
	statePtr->signalLBSwitchNotInterrupted();
}

void ContextTopFSM3::signalEStop(){
	statePtr->signalEStop();
}
void ContextTopFSM3::signalStart(){
	statePtr->signalStart();
}
void ContextTopFSM3::signalStop(){
	statePtr->signalStop();
}
void ContextTopFSM3::signalReset(){
	statePtr->signalReset();
}

void ContextTopFSM3::signalLBSkidInterrupted(){
	statePtr->signalLBSkidInterrupted();
}
void ContextTopFSM3::signalLBSkidNotInterrupted(){
	statePtr->signalLBSkidNotInterrupted();
}

void ContextTopFSM3::signalAltimetryCompleted(){
	statePtr->signalAltimetryCompleted();
}

void ContextTopFSM3::signalLBNextConveyor(){
	statePtr->signalLBNextConveyor();
}

void ContextTopFSM3::signalTimerTick(){
    statePtr->signalTimerTick();
}

