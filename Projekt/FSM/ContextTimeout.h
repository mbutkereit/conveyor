/*
 * ContextTimeout.h
 *
 *  Created on: 28.11.2016
 *      Author: abs949
 */

#ifndef CONTEXTTIMEOUT_H_
#define CONTEXTTIMEOUT_H_

#include <iostream>
using namespace std;

struct Datacto{
	Datacto(){}
};

class ContextTimeout {
public:
	static ContextTimeout* getInstance();
	virtual ~ContextTimeout(){};
	void transact() {
		statePtr->transact();
	} // context delegates signals to state
private:
	struct TimeOut { //top-level state
			Datacto* data;
			virtual void transact() {
			}
		}*statePtr;   // a pointer to current state. Used for polymorphism.


		struct StateStart: public TimeOut {
			virtual void transact() {
			}
		};

	static ContextTimeout* instance_;
	StateStart stateMember; //The memory for the state is part of context object
	Datacto ctodata;

	ContextTimeout() :
			statePtr(&stateMember), ctodata() // assigning start state
	{
		statePtr->data = &ctodata;
	}
	ContextTimeout(const ContextTimeout& other);
	ContextTimeout& operator=(const ContextTimeout& other);
};

#endif /* CONTEXTTIMEOUT_H_ */
