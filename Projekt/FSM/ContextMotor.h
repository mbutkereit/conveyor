/*
 * ContextMotor.h
 *
 *  Created on: 28.11.2016
 *      Author: abs949
 */

#ifndef CONTEXTMOTOR_H_
#define CONTEXTMOTOR_H_

#include <iostream>
#include <queue>
#include <pthread.h>
#include "Hal/HalBuilder.h"
#include "FSM/ContextTimer.h"
#include "FSM/MotorOptions.h"
#include "FSM/Puck.h"
using namespace std;

extern HalBuilder hb;

struct Datacm{
	Datacm(ContextTimer ctimer): hb(), ct(ctimer), highestPuckPriorityID(-1), dissolveID(-1), fastQueue(), slowQueue(), stopQueue(){
	}
		HalBuilder hb;
		ContextTimer ct;
		int highestPuckPriorityID;
		int dissolveID;
		queue<int, MotorOptions> fastQueue;
		queue<int, MotorOptions> slowQueue;
		queue<int, MotorOptions> stopQueue;
};

class ContextMotor {
public:
	static ContextMotor* getInstance();
	virtual ~ContextMotor(){};

	void setSpeed(int puckID, MotorOptions mo)
	{
		switch (mo) {
		case MotorOptions::FAST:
			cmdata.fastQueue.push(make_pair(puckID,puck));
			fast();
			break;
		case MotorOptions::SLOW:
			cmdata.slowQueue.push(make_pair(puckID,puck));
			slow();
			break;
		default:
			cmdata.stopQueue.push(make_pair(puckID,puck));
			stop();
			break;
		}
	}

	void dissolve(int dissolveID)
	{
		cmdata.dissolveID = dissolveID;
	}

	void transact() {
		statePtr->transact();
	} // context delegates signals to state

private:
	struct Motor { //top-level state
		Datacm* data;
		virtual void transact() {
		}
	}*statePtr;   // a pointer to current state. Used for polymorphism.

	struct StateStart: public Motor {
		virtual void transact() {
			if(data->stopMap.size() > 0)
			{
				hb.getHardware()->getMotor()->stop();
				new (this) Stop;
			}
			else if (data->slowMap.size() > 0)
			{
				hb.getHardware()->getMotor()->right();
				hb.getHardware()->getMotor()->slow();
				new (this) Slow;
			}
			else if (data->fastMap.size() > 0)
			{
				hb.getHardware()->getMotor()->right();
				hb.getHardware()->getMotor()->fast();
				new (this) Fast;
			}
		}
	};

	struct Fast: public Motor{
		virtual void transact() {
			if(data->stopMap.size() > 0)
			{
				hb.getHardware()->getMotor()->stop();
				new (this) Stop;
			}
			else if (data->slowMap.size() > 0)
			{
				hb.getHardware()->getMotor()->right();
				hb.getHardware()->getMotor()->slow();
				new (this) Slow;
			}
			else if (data->fastMap.size() > 0)
			{
				hb.getHardware()->getMotor()->right();
				hb.getHardware()->getMotor()->fast();
				new (this) Fast;
			}
		}
	};

	struct Slow: public Motor {
		virtual void transact() {
			if(data->stopMap.size() > 0)
			{
				hb.getHardware()->getMotor()->stop();
				new (this) Stop;
			}
			else if (data->slowMap.size() > 0)
			{
				hb.getHardware()->getMotor()->right();
				hb.getHardware()->getMotor()->slow();
				new (this) Slow;
			}
			else if (data->fastMap.size() > 0)
			{
				hb.getHardware()->getMotor()->right();
				hb.getHardware()->getMotor()->fast();
				new (this) Fast;
			}
		}
	};

	struct Stop: public Motor {
		virtual void transact() {

			if(data->stopMap.size() > 0)
			{
				hb.getHardware()->getMotor()->stop();
				new (this) Stop;
			}
			else if (data->slowMap.size() > 0)
			{
				hb.getHardware()->getMotor()->right();
				hb.getHardware()->getMotor()->slow();
				new (this) Slow;
			}
			else if (data->fastMap.size() > 0)
			{
				hb.getHardware()->getMotor()->right();
				hb.getHardware()->getMotor()->fast();
				new (this) Fast;
			}
		}
	};

	static ContextMotor* instance_;
	StateStart stateMember; //The memory for the state is part of context object
	Datacm cmdata;

	ContextMotor() :
			statePtr(&stateMember), cmdata(ContextTimer::getInstance()) // assigning start state
	{
		statePtr->data = &cmdata;
	}

	ContextMotor(const ContextMotor& other);
	ContextMotor& operator=(const ContextMotor& other);
};
#endif /* CONTEXTMOTOR_H_ */
