/*
 * ContextSwitch.h
 *
 *  Created on: 28.11.2016
 *      Author: abs949
 */

#ifndef CONTEXTSWITCH_H_
#define CONTEXTSWITCH_H_

#include <iostream>
using namespace std;

struct Datacsw{
	Datacsw(){}
};

class ContextSwitch {
public:
	static ContextSwitch* getInstance();
    virtual ~ContextSwitch(){};
    
private:
	struct Switch { //top-level state
			Datacsw* data;
			virtual void transact() {
			}
		}*statePtr;   // a pointer to current state. Used for polymorphism.

		struct StateStart: public Switch {
			virtual void transact() {
			}
		};

	static ContextSwitch* instance_;
	StateStart stateMember; //The memory for the state is part of context object
	Datacsw cswdata;

	ContextSwitch() :
			statePtr(&stateMember), cswdata() // assigning start state
	{
		statePtr->data = &cswdata;
	}
	ContextSwitch(const ContextSwitch& other);
	ContextSwitch& operator=(const ContextSwitch& other);
};

#endif /* CONTEXTSWITCH_H_ */
