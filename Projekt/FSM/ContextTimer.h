/*
 * ContextTimer.h
 *
 *  Created on: 28.11.2016
 *      Author: abs949
 */

#ifndef CONTEXTTIMER_H_
#define CONTEXTTIMER_H_

#include <iostream>
using namespace std;

struct Datact{
	Datact(){}
};

class ContextTimer {
public:
	static ContextTimer* getInstance();
	virtual ~ContextTimer(){};
	void transact() {
		statePtr->transact();
	} // context delegates signals to state
private:
	struct Timer { //top-level state
			Datact* data;
			virtual void transact() {
			}
		}*statePtr;   // a pointer to current state. Used for polymorphism.

		struct StateStart: public Timer {
			virtual void transact() {
			}
		};

	static ContextTimer* instance_;
	StateStart stateMember; //The memory for the state is part of context object
	Datact ctdata;

	ContextTimer() :
			statePtr(&stateMember), ctdata() // assigning start state
	{
		statePtr->data = &ctdata;
	}
	ContextTimer(const ContextTimer& other);
	ContextTimer& operator=(const ContextTimer& other);
};

#endif /* CONTEXTTIMER_H_ */
