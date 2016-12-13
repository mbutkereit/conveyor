/*
 * Dispatcher.cpp
 *
 *  Created on: 01.12.2016
 *      Author: abx827
*
 */
#include "Dispatcher.h"

	Dispatcher::Dispatcher() {
		// Add Method pointer to Call Method Array
		methods[LBBEGININTERRUPTED] = &ContextI::signalLBBeginInterrupted;
		methods[LBBEGINNOTINTERRUPTED] = &ContextI::signalLBBeginNotInterrupted;
		methods[LBENDINTERRUPTED] = &ContextI::signalLBEndInterrupted;
		methods[LBENDNOTINTERRUPTED] = &ContextI::signalLBEndNotInterrupted;
		methods[LBALTIMETRYINTERRUPTED] = &ContextI::signalLBAltimetryInterrupted;
		methods[LBALTIMETRYNOTINTERRUPTED] = &ContextI::signalLBAltimetryNotInterrupted;
		methods[LBSKIDINTERRUPTED] = &ContextI::signalLBSkidInterrupted;
		methods[LBSKIDNOTINTERRUPTED] = &ContextI::signalLBSkidNotInterrupted;
		methods[LBSWITCHINTERRUPTED] = &ContextI::signalLBSwitchInterrupted;
		methods[LBSWITCHNOTINTERRUPTED] = &ContextI::signalLBSwitchNotInterrupted;
		methods[RESETSIGNAL] = &ContextI::signalReset;
		methods[STOPSIGNAL] = &ContextI::signalStop;
		methods[ESTOPSIGNAL] = &ContextI::signalEStop;
		methods[STARTSIGNAL] = &ContextI::signalStart;
		methods[ALTEMETRYCOMPLETE] = &ContextI::signalAltimetryCompleted;
		methods[LBNEXTCONVEYOR] = &ContextI::signalLBNextConveyor;
	}

Dispatcher::~Dispatcher() {
	// TODO Auto-generated destructor stub
}

void Dispatcher::addListener(ContextI *listener, EVENTS event) {
	// Add Listener to be called on a specific Event
	listeners_[event].push_back(listener);
}

void Dispatcher::remListeners(ContextI *listener, EVENTS event) {
	// Remove Listener from a specific Event
	for (unsigned i = 0; i < listeners_[event].size(); ++i) {
		if (listeners_[event][i] == listener){
			listeners_[event].erase(listeners_[event].begin() + i);
		}
	}
}

 void Dispatcher::callListeners(EVENTS event) {
	// Call for every registered Listener
	// the Method that corresponds with event.
	for (unsigned i = 0; i < listeners_[event].size(); ++i) {
		(listeners_[event][i]->*methods[event])();
	}
}

