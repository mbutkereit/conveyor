/*
 * ContextTopFSM.cpp
 *
 *  Created on: 07.12.2016
 *      Author: abs949
 */

#include "ContextTopFSM2.h"

ContextTopFSM2::ContextTopFSM2(int puckID, std::vector<Puck>* puckVector): statePtr(&stateMember), contextdata(puckID, puckVector)   {
	statePtr->data = &contextdata;
}

ContextTopFSM2::~ContextTopFSM2() {}

void ContextTopFSM2::signalLBBeginInterrupted(){
	statePtr->signalLBBeginInterrupted();
}

void ContextTopFSM2::signalLBBeginNotInterrupted(){
	statePtr->signalLBBeginNotInterrupted();
}

void ContextTopFSM2::signalLBEndInterrupted()  {
	statePtr->signalLBEndInterrupted();
}
void ContextTopFSM2::signalLBEndNotInterrupted()  {
	statePtr->signalLBEndNotInterrupted();
}

void ContextTopFSM2::signalLBAltimetryInterrupted(){
	statePtr->signalLBAltimetryInterrupted();
}
void ContextTopFSM2::signalLBAltimetryNotInterrupted(){
	statePtr->signalLBAltimetryNotInterrupted();
}


void ContextTopFSM2::signalLBSwitchInterrupted(){
	statePtr->signalLBSwitchInterrupted();
}
void ContextTopFSM2::signalLBSwitchNotInterrupted(){
	statePtr->signalLBSwitchNotInterrupted();
}

void ContextTopFSM2::signalEStop(){
	statePtr->signalEStop();
}
void ContextTopFSM2::signalStart(){
	statePtr->signalStart();
}
void ContextTopFSM2::signalStop(){
	statePtr->signalStop();
}
void ContextTopFSM2::signalReset(){
	statePtr->signalReset();
}

void ContextTopFSM2::signalLBSkidInterrupted(){
	statePtr->signalLBSkidInterrupted();
}
void ContextTopFSM2::signalLBSkidNotInterrupted(){
	statePtr->signalLBSkidNotInterrupted();
}

void ContextTopFSM2::signalAltimetryCompleted(){
	statePtr->signalAltimetryCompleted();
}

void ContextTopFSM2::signalLBNextConveyor(){
	statePtr->signalLBNextConveyor();
}

