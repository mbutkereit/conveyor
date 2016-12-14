/*
 * ContextTimeMeasurementFast.cpp
 *
 *  Created on: 14.12.2016
 *      Author: abq808
 */

#include "ContextTimeMeasurementFast.h"

ContextTimeMeasurementFast::ContextTimeMeasurementFast() :
		statePtr(&stateMember), // assigning start state
		contextdata()
// initializing data
{
	statePtr->data = &contextdata; // connecting state->data with the context data
}

ContextTimeMeasurementFast::~ContextTimeMeasurementFast() {
}

void ContextTimeMeasurementFast::signalLBBeginInterrupted() {
	statePtr->signalLBBeginInterrupted();
}

void ContextTimeMeasurementFast::signalLBBeginNotInterrupted() {
	statePtr->signalLBBeginNotInterrupted();
}

void ContextTimeMeasurementFast::signalLBEndInterrupted() {
	statePtr->signalLBEndInterrupted();
}

void ContextTimeMeasurementFast::signalLBAltimetryInterrupted() {
	statePtr->signalLBAltimetryInterrupted();
}

void ContextTimeMeasurementFast::signalLBSwitchInterrupted() {
	statePtr->signalLBSwitchInterrupted();
}

