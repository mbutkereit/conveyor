/*
 * Context.h
 *
 *  Created on: 08.11.2016
 *      Author: abx827
 */

#ifndef CONTEXT_H_
#define CONTEXT_H_

#define DELTA_T0_TH 0
#define DELTA_TH_TW 0
#define DELTA_TW_TE 0
#define MAXIMUM_PUCKS 5

#include <iostream>
#include <ctime>
#include <queue>
#include <unistd.h>
#include "Puck.h"
#include "Logger/Logger.h"
#include "Hal/HalBuilder.h"
#include "ContextSorting.h"
using namespace std;

extern HalBuilder hb;
class Context {
public:
	Context(): statePtr(&stateMember)// assigning start state
    {
		id++;
    }

	void transact(){statePtr->transact();} // context delegates signals to state

	void signalLBBeginInterrupted();
	void signalLBEndInterrupted();
	void signalLBAltimetryInterrupted();
	void signalLBSwitchInterrupted();

	void signalLBBeginNotInterrupted();
	void signalLBEndInNotInterrupted();
	void signalLBAltimetryNotInterrupted();
	void signalLBSwitchNotInterrupted();

	void signalEStop();
	void signalStart();
	void signalStop();
	void signalReset();

	void signalLBSkidInterrupted();
	void signalLBSkidNotInterrupted();

	void signalAltimetryCompleted();

	//virtual ~Context();


private:
	struct PuckOnConveyor1 { //top-level state
		virtual void signalEStop() {
			hb.getHardware()->getMotor()->stop();
			//E-Stopp unlock missing
			while(1)
			{
				if(signalReset())
				{
					stateptr = &hs;
					break;
				}
			}

		} //put code here for signalEStop in superstate
		virtual void transact() {}
	}*statePtr;   // a pointer to current state. Used for polymorphism.

	struct StateStart : public PuckOnConveyor1 {
		virtual void transact(){
			hs = *statePtr;
			if(signalLBSkidNotInterrupted())
			{
				if(puckqueue.size() >= MAXIMUM_PUCKS)
				{
					new (this) ConveyorFull;
				}
				else
				{
					new (this) PuckNotInEntry;
				}
			}
		}
	};


	struct ConveyorFull: public PuckOnConveyor1 {
		virtual void transact() {
			hs = *statePtr;
			while(puckqueue.size() >= MAXIMUM_PUCKS){}
			new (this) PuckNotInEntry;
		}
	};

	struct PuckNotInEntry: public PuckOnConveyor1 {
		virtual void transact() {
			hs = *statePtr;
			hb.getHardware()->getTL()->turnGreenOn();
			while(signalLBBeginNotInterrupted()){}
			new (this) MotorOn;
		}
	};

	struct MotorOn: public PuckOnConveyor1 {
		virtual void transact() {
			hs = *statePtr;
			hb.getHardware()->getMotor()->right();
			hb.getHardware()->getMotor()->fast();
			puck(id);
			id++;
			puckqueue.push(puck);
			while(signalLBBeginInterrupted()){}
			t1 = clock();
			new (this) TransportToHeightMeasurement;
		}
	};

	struct TransportToHeightMeasurement: public PuckOnConveyor1 {
		virtual void transact() {
			hs = *statePtr;
			while(signalLBAltimetryNotInterrupted()){}
			//t_0 and t_H
			t2 = clock();
			delta = t2 - t1;
			hb.getHardware()->getMotor()->right();
			hb.getHardware()->getMotor()->slow();
			//Factor for CLOCK missing!
			if(delta < DELTA_T0_TH)
			{
				new (this) PuckAdded;
			}
			else if(delta > DELTA_T0_TH)
			{
				new (this) PuckLost;
			}
			else
			{
				new (this) PuckInHeightMeasurement;
			}
		}
	};

	struct PuckInHeightMeasurement: public PuckOnConveyor1 {
		virtual void transact() {
			hs = *statePtr;
			int height = hb.getHardware()->getAltimetry()->getHeight();
			puck.setHeightReading1(height);
			//Height measurementvalues missing!
			if(height > 1)
			{
				puck.setPuckdrillhole(NO_DRILL_HOLE);
			}
			else
			{
				puck.setPuckdrillhole(DRILL_HOLE_UPSIDE);
			}
			while(signalLBAltimetryInterrupted()){}
			hb.getHardware()->getMotor()->right();
			hb.getHardware()->getMotor()->fast();
			new (this) TransPortToSkid;
		}
	};

	struct TransPortToSkid: public PuckOnConveyor1 {
		virtual void transact() {
			hs = *statePtr;
			while(signalLBSwitchNotInterrupted()){}
			//t_h and t_w
			t1 = t2;
			t2 = clock();
			delta = t2 - t1;
			new (this) MetalDetection;
		}
	};

	struct MetalDetection: public PuckOnConveyor1 {
		virtual void transact() {
			hs = *statePtr;
			if(hb.getHardware()->getMT()->isItemMetal())
			{
				puck.setPuckmaterial(METAL);
			}
			else
			{
				puck.setPuckmaterial(PLASTIC);
			}
			if(signalLBSkidInterrupted())
			{
				new (this) BothSkidsFull;
			}
			else if(delta < DELTA_TH_TW)
			{
				new (this) PuckAdded;
			}
			else if(delta > DELTA_TH_TW)
			{
				new (this) PuckLost;
			}
			else
			{
				new (this) Sorting;
			}
		}
	};

	struct Sorting: public PuckOnConveyor1 {
		virtual void transact() { //TRANSITION IS NOT DEFINED
			cs.setCurrentPh(puck.getPuckdrillhole());
			cs.setCurrentPm(puck.getPuckmaterial());
			cs.transact();
			if (sequenceOK)
			{
				new (this) TransportToDelivery;
			}
			else
			{
				if(signalLBSkidNotInterrupted())
				{
					new (this) SortOutThroughSkid;
				}
				else
				{
					hb.getHardware()->getTL()->turnYellowOn();
					new (this) TransportToDelivery;
				}
			}

		}
	};

	struct TransportToDelivery: public PuckOnConveyor1 {
		virtual void transact() {
			hb.getHardware()->getMotor()->switchOpen();
			t1 = t2;
			while(signalLBEndInNotInterrupted()){}
			t2 = clock();
			delta = t2 - t1;
			if(delta > DELTA_TW_TE)
			{
				hb.getHardware()->getTL()->turnGreenOff();
				hb.getHardware()->getTL()->turnYellowOff();
				new (this) PuckLost;
			}
			else if(delta < DELTA_TW_TE)
			{
				hb.getHardware()->getTL()->turnGreenOff();
		        hb.getHardware()->getTL()->turnYellowOff();
		        new (this) PuckAdded;
			}
			else
			{
				new (this) DeliveryToConveyor2;
			}
		}
	};

	struct DeliveryToConveyor2: public PuckOnConveyor1 {
		virtual void transact() {
			//Check for conveyor2 isFree() missing!
			new (this) TransportToConveyor2;
		}
	};

	struct TransportToConveyor2: public PuckOnConveyor1 {
		virtual void transact() {
			while(signalLBEndInterrupted());
			usleep(4000000);
			puckqueue.pop();
			if(puckqueue.empty())
			{
				hb.getHardware()->getMotor()->stop();
			}
			new (this) NULL;
		}
	};

	struct BothSkidsFull : public PuckOnConveyor1 {
		virtual void transact()
		{

		}
	};

	struct PuckLost : public PuckOnConveyor1 {
		virtual void transact()
		{

		}
	};

	struct PuckAdded : public PuckOnConveyor1 {
		virtual void transact()
		{

		}
	};

	StateStart stateMember; //The memory for the state is part of context object
	ContextSorting cs;
	HistoryState hs;
	Puck puck;
	queue<Puck> puckqueue;
	static int id = 0;
	clock_t t1;
	clock_t t2;
	clock_t delta;
};

#endif /* CONTEXT_H_ */
