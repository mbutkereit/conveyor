/*
 * ContextTimeMeasurementSlow.cpp
 *
 *  Created on: 14.12.2016
 *      Author: abq808
 */

#include "ContextTimeMeasurementSlow.h"

ContextTimeMeasurementSlow::ContextTimeMeasurementSlow() :
            statePtr(&stateMember), ctmsdata() // assigning start state
    {
        statePtr->data = &ctmsdata;
    }

ContextTimeMeasurementSlow::~ContextTimeMeasurementSlow(){};

void ContextTimeMeasurementSlow::signalLBBeginInterrupted(){
    statePtr->signalLBBeginInterrupted();
}

void ContextTimeMeasurementSlow::signalLBBeginNotInterrupted(){
    statePtr->signalLBBeginNotInterrupted();
}

void ContextTimeMeasurementSlow::signalLBEndInterrupted()  {
    statePtr->signalLBEndInterrupted();
}
void ContextTimeMeasurementSlow::signalLBEndNotInterrupted()  {
    statePtr->signalLBEndNotInterrupted();
}

void ContextTimeMeasurementSlow::signalLBAltimetryInterrupted(){
    statePtr->signalLBAltimetryInterrupted();
}
void ContextTimeMeasurementSlow::signalLBAltimetryNotInterrupted(){
    statePtr->signalLBAltimetryNotInterrupted();
}

void ContextTimeMeasurementSlow::signalLBSwitchInterrupted(){
    statePtr->signalLBSwitchInterrupted();
}
void ContextTimeMeasurementSlow::signalLBSwitchNotInterrupted(){
    statePtr->signalLBSwitchNotInterrupted();
}

void ContextTimeMeasurementSlow::signalEStop(){
    statePtr->signalEStop();
}
void ContextTimeMeasurementSlow::signalStart(){
    statePtr->signalStart();
}

void ContextTimeMeasurementSlow::signalStop(){
    statePtr->signalStop();
}
void ContextTimeMeasurementSlow::signalReset(){
    statePtr->signalReset();
}

void ContextTimeMeasurementSlow::signalLBSkidInterrupted(){
    statePtr->signalLBSkidInterrupted();
}
void ContextTimeMeasurementSlow::signalLBSkidNotInterrupted(){
    statePtr->signalLBSkidNotInterrupted();
}

void ContextTimeMeasurementSlow::signalAltimetryCompleted(){
    statePtr->signalAltimetryCompleted();
}

void ContextTimeMeasurementSlow::signalLBNextConveyor(){
    statePtr->signalLBNextConveyor();
}

void ContextTimeMeasurementSlow::signalTimerTick(){
    statePtr->signalTimerTick();
}
