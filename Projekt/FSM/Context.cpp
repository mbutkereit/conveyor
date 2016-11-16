#include "Context.h"

Context::Context(): statePtr(&stateMember), contextdata(0)   {
	statePtr->data = &contextdata;
}

Context::~Context() {}

void Context::signalLBBeginInterrupted(){
	statePtr->signalLBBeginInterrupted();
}

void Context::signalLBBeginNotInterrupted(){
	statePtr->signalLBBeginNotInterrupted();
}

void Context::signalLBEndInterrupted()  {
	statePtr->signalLBEndInterrupted();
}
void Context::signalLBEndNotInterrupted()  {
	statePtr->signalLBEndNotInterrupted();
}

void Context::signalLBAltimetryInterrupted(){
	statePtr->signalLBAltimetryInterrupted();
}
void Context::signalLBAltimetryNotInterrupted(){
	statePtr->signalLBAltimetryNotInterrupted();
}


void Context::signalLBSwitchInterrupted(){
	statePtr->signalLBSwitchInterrupted();
}
void Context::signalLBSwitchNotInterrupted(){
	statePtr->signalLBSwitchNotInterrupted();
}

void Context::signalEStop(){
	statePtr->signalEStop();
}
void Context::signalStart(){
	statePtr->signalStart();
}
void Context::signalStop(){
	statePtr->signalStop();
}
void Context::signalReset(){
	statePtr->signalReset();
}

void Context::signalLBSkidInterrupted(){
	statePtr->signalLBSkidInterrupted();
}
void Context::signalLBSkidNotInterrupted(){
	statePtr->signalLBSkidNotInterrupted();
}

void Context::signalAltimetryCompleted(){
	statePtr->signalAltimetryCompleted();
}
