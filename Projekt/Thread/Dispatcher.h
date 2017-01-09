/*
 * Dispatcher.h
 *
 *  Created on: 01.12.2016
 *      Author: abx827
 */

#ifndef DISPATCHER_H_
#define DISPATCHER_H_

#include <iostream>
#include <vector>
#include "SignalHandlerThread.h"
#include "ContextI.h"

// Events in the System. Each EVENT needs a corresponding Method in class Context
enum EVENTS { LBBEGININTERRUPTED, LBBEGINNOTINTERRUPTED, LBENDINTERRUPTED,LBENDNOTINTERRUPTED,
	LBALTIMETRYINTERRUPTED,LBALTIMETRYNOTINTERRUPTED,LBSKIDINTERRUPTED,LBSKIDNOTINTERRUPTED,
	LBSWITCHINTERRUPTED,LBSWITCHNOTINTERRUPTED,RESETSIGNAL,STOPSIGNAL,ESTOPSIGNAL,STARTSIGNAL,ALTEMETRYCOMPLETE,LBNEXTCONVEYOR,
	TIMINTR,NEVENTS };



// typedef: Method from Class "Transitions"
typedef void (ContextI::*method_t)(void);

class Dispatcher {
public:

	/**
	 * Konstruktor
	 */
	Dispatcher();

	/**
	 * Destruktor
	 */
	virtual ~Dispatcher();

	/**
	 * Call for every registered Listener
	 * @param event das Event
	 */
	virtual void callListeners(EVENTS event);

	/**
	 *  Remove Listener from a specific Event
	 * @param listener die State Maschine
	 * @param event das Event
	 */
	virtual void remListeners(ContextI *listener, EVENTS event);

	/**
	 * Add Listener to be called on a specific Event
	 *  @param listener die State Maschine
	 * @param event das Event
	 */
	virtual void addListener(ContextI *listener, EVENTS event);

private:
	method_t methods[NEVENTS];
	std::vector<ContextI *> listeners_[NEVENTS];

	Dispatcher(const Dispatcher &other);
	Dispatcher &operator=(const Dispatcher &other);

};

#endif /* DISPATCHER_H_ */
