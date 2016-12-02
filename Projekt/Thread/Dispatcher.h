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
//#include "FSM/Context.h"

// Events in the System. Each EVENT needs a corresponding Method in class Context
enum EVENTS { LBBEGININTERRUPTED, LBBEGINNOTINTERRUPTED, LBENDINTERRUPTED,LBENDNOTINTERRUPTED,
	LBALTIMETRYINTERRUPTED,LBALTIMETRYNOTINTERRUPTED,LBSKIDINTERRUPTED,LBSKIDNOTINTERRUPTED,
	LBSWITCHINTERRUPTED,LBSWITCHNOTINTERRUPTED,RESETSIGNAL,STOPSIGNAL,ESTOPSIGNAL,STARTSIGNAL,ALTEMETRYCOMPLETE,
	NEVENTS };



// typedef: Method from Class "Transitions"
typedef void (Context::*method_t)(void);

class Dispatcher {
public:
	Dispatcher();
	virtual ~Dispatcher();


	virtual void callListeners(EVENTS event);
	virtual void remListeners(Context *listener, EVENTS event);
	virtual void addListener(Context *listener, EVENTS event);
private:
	method_t methods[NEVENTS];
	std::vector<Context *> listeners_[NEVENTS];

	Dispatcher(const Dispatcher &other);
	Dispatcher &operator=(const Dispatcher &other);

};

#endif /* DISPATCHER_H_ */
