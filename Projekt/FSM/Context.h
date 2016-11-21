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
#include <vector>
#include "FSM/Puck.h"
#include "Logger/Logger.h"
#include "Hal/HalBuilder.h"
using namespace std;

extern HalBuilder hb;
class Context {
public:
	Context(): statePtr(&stateMember)  // assigning start state
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
				if(puckvector.size() >= MAXIMUM_PUCKS)
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
			while(puckvector.size() >= MAXIMUM_PUCKS)
			{}
			new (this) PuckNotInEntry;
		}
	};

	struct PuckNotInEntry: public PuckOnConveyor1 {
		virtual void transact() {
			hb.getHardware()->getTL()->turnGreenOn();
			while(signalLBBeginNotInterrupted()){}
			new (this) MotorOn;
		}
	};

	struct MotorOn: public PuckOnConveyor1 {
		virtual void transact() {
			hb.getHardware()->getMotor()->right();
			hb.getHardware()->getMotor()->fast();
			Puck puck(id, TYPE404, -1, -1);
			puckvector.push_back(puck);
			while(signalLBBeginInterrupted()){}
			t1 = clock();
			new (this) TransportToHeightMeasurement;
		}
	};

	struct TransportToHeightMeasurement: public PuckOnConveyor1 {
		virtual void transact() {
			while(signalLBAltimetryNotInterrupted()){}
			hb.getHardware()->getMotor()->slow();
			t2 = clock();
			delta = t2 - t1;
		}
	};

	struct PuckInHeightMeasurement: public PuckOnConveyor1 {
		virtual void transact() {
			if (1) // choice point
				new (this) TransPortToSkid;
		}
	};

	struct TransPortToSkid: public PuckOnConveyor1 {
		virtual void transact() {
			new (this) MetalDetection;
		}
	};

	struct MetalDetection: public PuckOnConveyor1 {
		virtual void transact() { //TRANSITION IS NOT DEFINED
			if (hb.getHardware()->getMT()->isItemMetal()) {
				//Code if puck is metal
			}
			if (1) { //Delta von t_h und t_w ok and both skids NOT full
				new (this) Sorting;
			} else if (11) { //Delta von t_h und t_w NOT ok

			} else { // Skid 1 and skid 2 full / turnGreenOff(), turnRedOn()
				//new (this) BothSkidsFull;
			}
		}
	};

	struct Sorting: public PuckOnConveyor1 {
		virtual void transact() { //TRANSITION IS NOT DEFINED
			if (1){}
				//new (this) StateA;
			else
			{}
				//new (this) StateB;
		}
	};

	struct TransportToDelivery: public PuckOnConveyor1 {
		virtual void transact() {
			if (1) { //Delta von t_w und t_e ok
				new (this) DeliveryToConveyor2;
			} else if (11) { //Delta von t_w und t_e too high
				//new (this) PuckLost;
			} else { //Delta von t_w und t_e too low
				//new (this) PuckAdded;
			}
		}
	};

	struct DeliveryToConveyor2: public PuckOnConveyor1 {
		virtual void transact() { //TRANSITION NOT DEFINED
			new (this) TransportToConveyor2;
		}
	};

	struct TransportToConveyor2: public PuckOnConveyor1 {
		virtual void transact() {
			if (1) { // conveyor1NotEmpty
				//new (this) NULL;
			} else if (0) { // conveyorEmpty
				hb.getHardware()->getMotor()->stop();
				//new (this) NULL;
			}
		}
	};

	StateStart stateMember; //The memory for the state is part of context object
	HistoryState hs;
	vector<Puck> puckvector;
	static int id = 0;
	clock_t t1;
	clock_t t2;
	clock_t delta;
};

#endif /* CONTEXT_H_ */
