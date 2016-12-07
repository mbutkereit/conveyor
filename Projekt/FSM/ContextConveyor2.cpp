/*
 * ContextConveyor2.cpp
 *
 *  Created on: 02.12.2016
 *      Author: abx827
 */

#include "ContextConveyor2.h"

ContextConveyor2::ContextConveyor2(int puckID, std::vector<Puck>* puckVector): statePtr(&stateMember), contextdata(puckID, puckVector)   {
    statePtr->data = &contextdata;
}

ContextConveyor2::~ContextConveyor2() {}

void ContextConveyor2::signalLBBeginInterrupted(){
	statePtr->signalLBBeginInterrupted();
}

void ContextConveyor2::signalLBBeginNotInterrupted(){
	statePtr->signalLBBeginNotInterrupted();
}

void ContextConveyor2::signalLBEndInterrupted()  {
	statePtr->signalLBEndInterrupted();
}
void ContextConveyor2::signalLBEndNotInterrupted()  {
	statePtr->signalLBEndNotInterrupted();
}

void ContextConveyor2::signalLBAltimetryInterrupted(){
	statePtr->signalLBAltimetryInterrupted();
}
void ContextConveyor2::signalLBAltimetryNotInterrupted(){
	statePtr->signalLBAltimetryNotInterrupted();
}


void ContextConveyor2::signalLBSwitchInterrupted(){
	statePtr->signalLBSwitchInterrupted();
}
void ContextConveyor2::signalLBSwitchNotInterrupted(){
	statePtr->signalLBSwitchNotInterrupted();
}

void ContextConveyor2::signalEStop(){
	statePtr->signalEStop();
}
void ContextConveyor2::signalStart(){
	statePtr->signalStart();
}
void ContextConveyor2::signalStop(){
	statePtr->signalStop();
}
void ContextConveyor2::signalReset(){
	statePtr->signalReset();
}

void ContextConveyor2::signalLBSkidInterrupted(){
	statePtr->signalLBSkidInterrupted();
}
void ContextConveyor2::signalLBSkidNotInterrupted(){
	statePtr->signalLBSkidNotInterrupted();
}

void ContextConveyor2::signalAltimetryCompleted(){
	statePtr->signalAltimetryCompleted();
}

void ContextConveyor2::signalLBNextConveyor(){
	statePtr->signalLBNextConveyor();
}
