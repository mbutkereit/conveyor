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
		methods[LBBEGININTERRUPTED] = &Context::signalLBBeginInterrupted;
		methods[LBBEGINNOTINTERRUPTED] = &Context::signalLBBeginNotInterrupted;
		methods[LBENDINTERRUPTED] = &Context::signalLBEndInterrupted;
		methods[LBENDNOTINTERRUPTED] = &Context::signalLBEndNotInterrupted;
		methods[LBALTIMETRYINTERRUPTED] = &Context::signalLBAltimetryInterrupted;
		methods[LBALTIMETRYNOTINTERRUPTED] = &Context::signalLBAltimetryNotInterrupted;
		methods[LBSKIDINTERRUPTED] = &Context::signalLBSkidInterrupted;
		methods[LBSKIDNOTINTERRUPTED] = &Context::signalLBSkidNotInterrupted;
		methods[LBSWITCHINTERRUPTED] = &Context::signalLBSwitchInterrupted;
		methods[LBSWITCHNOTINTERRUPTED] = &Context::signalLBSwitchNotInterrupted;
		methods[RESETSIGNAL] = &Context::signalReset;
		methods[STOPSIGNAL] = &Context::signalStop;
		methods[ESTOPSIGNAL] = &Context::signalEStop;
		methods[STARTSIGNAL] = &Context::signalStart;
		methods[ALTEMETRYCOMPLETE] = &Context::signalAltimetryCompleted;
		methods[LBNEXTCONVEYOR] = &Context::signalLBNextConveyor;
	}

Dispatcher::~Dispatcher() {
	// TODO Auto-generated destructor stub
}

void Dispatcher::addListener(Context *listener, EVENTS event) {
	// Add Listener to be called on a specific Event
	listeners_[event].push_back(listener);
}

void Dispatcher::remListeners(Context *listener, EVENTS event) {
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

