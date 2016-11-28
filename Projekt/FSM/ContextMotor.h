/*
 * ContextMotor.h
 *
 *  Created on: 28.11.2016
 *      Author: abs949
 */

#ifndef CONTEXTMOTOR_H_
#define CONTEXTMOTOR_H_

#include <iostream>
using namespace std;

struct Datacm{
	Datacm(){}
};

class ContextMotor {
public:
	static ContextMotor* getInstance();
	virtual ~ContextMotor(){};
private:
	struct Motor { //top-level state
		Datacm* data;
		virtual void transact() {
		}
	}*statePtr;   // a pointer to current state. Used for polymorphism.

	struct StateStart: public Motor {
		virtual void transact() {
		}
	};

	struct Fast: public Motor{
		virtual void transact() {
		}
	};

	struct Slow: public Motor {
		virtual void transact() {
		}
	};

	struct Stop: public Motor {
		virtual void transact() {
		}
	};

	static ContextMotor* instance_;
	StateStart stateMember; //The memory for the state is part of context object
	Datacm cmdata;

	ContextMotor() :
			statePtr(&stateMember), cmdata() // assigning start state
	{
		statePtr->data = &cmdata;
	}
	ContextMotor(const ContextMotor& other);
	ContextMotor& operator=(const ContextMotor& other);
};

#endif /* CONTEXTMOTOR_H_ */
