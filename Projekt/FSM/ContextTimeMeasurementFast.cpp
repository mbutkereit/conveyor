/*
 * ContextTimeMeasurementFast.cpp
 *
 *  Created on: 14.12.2016
 *      Author: abq808
 */

#include "ContextTimeMeasurementFast.h"

ContextTimeMeasurementFast::ContextTimeMeasurementFast() :
            statePtr(&stateMember), ctmfdata() // assigning start state
    {
        statePtr->data = &ctmfdata;
    }

ContextTimeMeasurementFast::~ContextTimeMeasurementFast(){};

void ContextTimeMeasurementFast::signalLBBeginInterrupted(){
    statePtr->signalLBBeginInterrupted();
}

void ContextTimeMeasurementFast::signalLBBeginNotInterrupted(){
    statePtr->signalLBBeginNotInterrupted();
}

void ContextTimeMeasurementFast::signalLBEndInterrupted()  {
    statePtr->signalLBEndInterrupted();
}
void ContextTimeMeasurementFast::signalLBEndNotInterrupted()  {
    statePtr->signalLBEndNotInterrupted();
}

void ContextTimeMeasurementFast::signalLBAltimetryInterrupted(){
    statePtr->signalLBAltimetryInterrupted();
}
void ContextTimeMeasurementFast::signalLBAltimetryNotInterrupted(){
    statePtr->signalLBAltimetryNotInterrupted();
}

void ContextTimeMeasurementFast::signalLBSwitchInterrupted(){
    statePtr->signalLBSwitchInterrupted();
}
void ContextTimeMeasurementFast::signalLBSwitchNotInterrupted(){
    statePtr->signalLBSwitchNotInterrupted();
}

void ContextTimeMeasurementFast::signalEStop(){
    statePtr->signalEStop();
}
void ContextTimeMeasurementFast::signalStart(){
    statePtr->signalStart();
}

void ContextTimeMeasurementFast::signalStop(){
    statePtr->signalStop();
}
void ContextTimeMeasurementFast::signalReset(){
    statePtr->signalReset();
}

void ContextTimeMeasurementFast::signalLBSkidInterrupted(){
    statePtr->signalLBSkidInterrupted();
}
void ContextTimeMeasurementFast::signalLBSkidNotInterrupted(){
    statePtr->signalLBSkidNotInterrupted();
}

void ContextTimeMeasurementFast::signalAltimetryCompleted(){
    statePtr->signalAltimetryCompleted();
}

void ContextTimeMeasurementFast::signalLBNextConveyor(){
    statePtr->signalLBNextConveyor();
}

void ContextTimeMeasurementFast::signalTimerTick(){
    statePtr->signalTimerTick();
}
