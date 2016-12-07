/*
 * ContextTopFSM.cpp
 *
 *  Created on: 07.12.2016
 *      Author: abs949
 */

#include "ContextTopFSM1.h"

ContextTopFSM1::ContextTopFSM1(int puckID, std::vector<Puck>* puckVector): statePtr(&stateMember), contextdata(puckID, puckVector)   {
	statePtr->data = &contextdata;
}

ContextTopFSM1::~ContextTopFSM1() {}

void ContextTopFSM1::signalLBBeginInterrupted(){
	statePtr->signalLBBeginInterrupted();
}

void ContextTopFSM1::signalLBBeginNotInterrupted(){
	statePtr->signalLBBeginNotInterrupted();
}

void ContextTopFSM1::signalLBEndInterrupted()  {
	statePtr->signalLBEndInterrupted();
}
void ContextTopFSM1::signalLBEndNotInterrupted()  {
	statePtr->signalLBEndNotInterrupted();
}

void ContextTopFSM1::signalLBAltimetryInterrupted(){
	statePtr->signalLBAltimetryInterrupted();
}
void ContextTopFSM1::signalLBAltimetryNotInterrupted(){
	statePtr->signalLBAltimetryNotInterrupted();
}


void ContextTopFSM1::signalLBSwitchInterrupted(){
	statePtr->signalLBSwitchInterrupted();
}
void ContextTopFSM1::signalLBSwitchNotInterrupted(){
	statePtr->signalLBSwitchNotInterrupted();
}

void ContextTopFSM1::signalEStop(){
	statePtr->signalEStop();
}
void ContextTopFSM1::signalStart(){
	statePtr->signalStart();
}
void ContextTopFSM1::signalStop(){
	statePtr->signalStop();
}
void ContextTopFSM1::signalReset(){
	statePtr->signalReset();
}

void ContextTopFSM1::signalLBSkidInterrupted(){
	statePtr->signalLBSkidInterrupted();
}
void ContextTopFSM1::signalLBSkidNotInterrupted(){
	statePtr->signalLBSkidNotInterrupted();
}

void ContextTopFSM1::signalAltimetryCompleted(){
	statePtr->signalAltimetryCompleted();
}

void ContextTopFSM1::signalLBNextConveyor(){
	statePtr->signalLBNextConveyor();
}

