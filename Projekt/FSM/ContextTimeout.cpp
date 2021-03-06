/*
 * ContextTimeout.cpp
 *
 *  Created on: 28.11.2016
 *      Author: abs949
 */

#include "ContextTimeout.h"

ContextTimeout::ContextTimeout() : statePtr(&stateMember), ctodata() // assigning start state
{
	statePtr->data = &ctodata;
}

ContextTimeout::~ContextTimeout(){}

void ContextTimeout::timeout(){
	statePtr->timeout();
}

void ContextTimeout::startTimerT0(){
	statePtr->startTimerT0();
}

void ContextTimeout::stopTimerT0(){
	statePtr->stopTimerT0();
}

void ContextTimeout::startTimerTH(){
	statePtr->startTimerTH();
}

void ContextTimeout::stopTimerTH(){
	statePtr->stopTimerTH();
}

void ContextTimeout::startTimerTW(){
	statePtr->startTimerTW();
}

void ContextTimeout::stopTimerTW(){
	statePtr->stopTimerTW();
}

void ContextTimeout::signalTimerTick(){
	statePtr->signalTimerTick();
}

bool ContextTimeout::timeoutOccured(){
	return ctodata.timeout;
}

void ContextTimeout::stopTimer(){
	statePtr->stopTimer();
}
