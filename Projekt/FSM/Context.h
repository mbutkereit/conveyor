/*
 * Context.h
 *
 *  Created on: 08.11.2016
 *      Author: abx827
 */

#ifndef CONTEXT_H_
#define CONTEXT_H_

#include "Logger/Logger.h"
#include <iostream>
#include "Hal/HalBuilder.h"

extern HalBuilder hb;
class Context {
public:
	Context();
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

	virtual ~Context();


private:
	struct PuckOnConveyor1 { //top-level state
		virtual void signalEStop() {
		} //put code here for signalEStop in superstate
	}*statePtr;   // a pointer to current state. Used for polymorphism.

	struct Conveyor1Full: public State {
		virtual void signalConveyor1Free() {
			new (this) PuckNotInEntry;
		}
	};

	struct PuckNotInEntry: public State {
		virtual void signalLBBeginInterrupted() {
		    new (this) MotorOn;
		}
	};

	struct MotorOn: public State {
		virtual void signalLBBeginNotInterrupted() {
			new (this) TransportToHeightMeasurement;
		}
	};

	struct TransportToHeightMeasurement: public State {
		virtual void signalLBAltimetryInterrupted() {
			if (1){ // Delta von t_0 und t_H OK
				new (this) PuckInHeightMeasurement;
			}
			else if (11){ //Delta von t_0 und t_H zu klein
				new (this) PuckAdded;
			}
			else{ ////Delta von t_0 und t_H zu groß
				new (this) PuckLost;
			}
		}
	};

	struct PuckInHeightMeasurement: public State {
		virtual void signalLBAltimetryNotInterrupted() {
			if (data->data1 > 10) // choice point
				new (this) TransPortToSkid;
		}
	};

	struct TransPortToSkid: public State {
		virtual void signalLBSwitchInterrupted() {
			new (this) MetalDetection;
		}
	};

	struct MetalDetection: public State {
		virtual void signalNotDefined() { //TRANSITION IS NOT DEFINED
			if (hb.getHardware()->getMT()->isItemMetal()){
				//Code if puck is metal
			}
			if(1){ //Delta von t_h und t_w ok and both skids NOT full
				new (this) Sorting;
			}
			else if(11){ //Delta von t_h und t_w NOT ok

			}
			else{// Skid 1 and skid 2 full / turnGreenOff(), turnRedOn()
				new (this) BothSkidsFull;
			}
		}
	};

	struct Sorting: public State {
		virtual void signalA() { //TRANSITION IS NOT DEFINED
			if (data->data1 > 10)
				new (this) StateA;
			else
				new (this) StateB;
		}
	};

	struct TransportToDelivery: public State {
		virtual void signalLBEndInterrupted() {
			if (1) { //Delta von t_w und t_e ok
				new (this) DeliveryToConveyor2;
			} else if (11) { //Delta von t_w und t_e too high
                  new (this) PuckLost;
			} else { //Delta von t_w und t_e too low
				new (this) PuckAdded;
			}
		}
	};

	struct DeliveryToConveyor2: public State {
		virtual void signalNotDefined() { //TRANSITION NOT DEFINED
			new (this) TransportToConveyor2;
		}
	};

	struct TransportToConveyor2: public State {
		virtual void signalLBEndInNotInterrupted() {
			if (1){ // conveyor1NotEmpty
				new (this) NULL;
			}
			else if (0){ // conveyorEmpty
				hb.getHardware()->getMotor()->stop();
				new (this) NULL;
			}
		}
	};

		StateA stateMember;//The memory for the state is part of context object
		Data contextdata;  //Data is also kept inside the context object

		public:
		Context()
		: statePtr(&stateMember) // assigning start state
		, contextdata(0)         // initializing data
		{
			statePtr->data = &contextdata; // connecting state->data with the context data
		}

		void signalA(){statePtr->signalA();} // context delegates signals to state
};

#endif /* CONTEXT_H_ */
