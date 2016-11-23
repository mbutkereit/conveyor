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
#define MAXIMUM_PUCKS 1

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

struct Datac {
	Datac() :
			hb(), cs(getInstance()), puck(-1), puckqueue(), es(), id(0), t1(0), t2(0), delta(0) {
	}
	HalBuilder hb;
	ContextSorting cs;
	Puck puck;
	queue<Puck> puckqueue;
	ErrorStates es;
	int id;
	clock_t t1;
	clock_t t2;
	clock_t delta;
};

static ContextSorting* getInstance();

ContextSorting* ContextSorting::instance_ = NULL;
ContextSorting* ContextSorting::getInstance() {
	if (instance_ == NULL) {
		instance_ = new ContextSorting();
	}
	return instance_;
}

class Context {
public:
	Context() :
			statePtr(&stateMember), cdata() // assigning start state
	{
		statePtr->data = &cdata;
	}

	~Context(){};

	void transact() {
		statePtr->transact();
	} // context delegates signals to state

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

private:
	struct PuckOnConveyor2 { //top-level state
		Datac* data;
		virtual void signalEStop() {
			//struct PuckOnConveyor1* hs;
			data->hb.getHardware()->getMotor()->stop();
			//E-Stopp unlock missing
			/*while (1) {
				if (0) {
					break;
				}
			}
			*/

		} //put code here for signalEStop in superstate
		virtual void transact() {
		}
	}*statePtr;   // a pointer to current state. Used for polymorphism.

	struct StateStart: public PuckOnConveyor2 {
		virtual void transact() {
			if (data->hb.getHardware()->getMT()->isSkidFull()) {
				if (data->puckqueue.size() >= MAXIMUM_PUCKS) {
					new (this) ConveyorFull;
				} else {
					new (this) PuckNotInEntry;
				}
			}
		}
	};

	struct ConveyorFull: public PuckOnConveyor2 {
		virtual void transact() {
			while (data->puckqueue.size() >= MAXIMUM_PUCKS) {
			}
			new (this) PuckNotInEntry;
		}
	};

	struct PuckNotInEntry: public PuckOnConveyor2 {
		virtual void transact() {
			data->hb.getHardware()->getTL()->turnGreenOn();
			while (data->hb.getHardware()->getMT()->isItemRunningIn()) {}
			new (this) MotorOn;
		}
	};

	struct MotorOn: public PuckOnConveyor2 {
		virtual void transact() {
			int id = (data->id);
			Puck puck(id);
			data->puck = puck;
			data->hb.getHardware()->getMotor()->right();
			data->hb.getHardware()->getMotor()->fast();
			data->id++;
			data->puckqueue.push(puck);
			while (!(data->hb.getHardware()->getMT()->isItemRunningIn())) {
			}
			data->t1 = clock();
			new (this) TransportToHeightMeasurement;
		}
	};

	struct TransportToHeightMeasurement: public PuckOnConveyor2 {
		virtual void transact() {
			while (data->hb.getHardware()->getMT()->isItemAltimetry()) {}
			//t_0 and t_H
			data->t2 = clock();
			data->delta = data->t2 - data->t1;
			data->hb.getHardware()->getMotor()->right();
			data->hb.getHardware()->getMotor()->slow();
			//Factor for CLOCK missing!
			if (data->delta < DELTA_T0_TH) {
				new (this) PuckAdded;
			} else if (data->delta > DELTA_T0_TH) {
				data->es.puckLost();
				new (this) PuckInHeightMeasurement;
			} else {
				new (this) PuckInHeightMeasurement;
			}
		}
	};

	struct PuckInHeightMeasurement: public PuckOnConveyor2 {
		virtual void transact() {
			int height = data->hb.getHardware()->getAltimetry()->getHeight();
			data->puck.setHeightReading1(height);
			if (data->hb.getHardware()->getMT()->isItemInAltimetryToleranceRange()) {
				data->puck.setPuckdrillhole(DRILL_HOLE_UPSIDE);
			} else {
				data->puck.setPuckdrillhole(NO_DRILL_HOLE);
			}
			while (!(data->hb.getHardware()->getMT()->isItemAltimetry())) {}
			data->hb.getHardware()->getMotor()->right();
			data->hb.getHardware()->getMotor()->fast();
			new (this) TransPortToSkid;
		}
	};

	struct TransPortToSkid: public PuckOnConveyor2 {
		virtual void transact() {
			while (data->hb.getHardware()->getMT()->isItemSwitch()) {}
			//t_h and t_w
			data->t1 = data->t2;
			data->t2 = clock();
			data->delta = data->t2 - data->t1;
			new (this) MetalDetection;
		}
	};

	struct MetalDetection: public PuckOnConveyor2 {
		virtual void transact() {
			if (data->hb.getHardware()->getMT()->isItemMetal()) {
				data->puck.setPuckmaterial(METAL);
			} else {
				data->puck.setPuckmaterial(PLASTIC);
			}
			if (!(data->hb.getHardware()->getMT()->isSkidFull())) {
				data->es.bothSkidsFull();
				new (this) Sorting;
				//new (this) BothSkidsFull;
			} else if (data->delta < DELTA_TH_TW) {
				new (this) PuckAdded;
			} else if (data->delta > DELTA_TH_TW) {
				data->es.puckLost();
				new (this) Sorting;
				//new (this) PuckLost;
			} else {
				new (this) Sorting;
			}
		}
	};

	struct Sorting: public PuckOnConveyor2 {
		virtual void transact() { //TRANSITION IS NOT DEFINED
			data->cs.setCurrentPh(data->puck.getPuckdrillhole());
			data->cs.setCurrentPm(data->puck.getPuckmaterial());
			data->cs.transact();
			if (data->cs.getSequenceOk()) {
				new (this) TransportToDelivery;
			} else {
				if (data->hb.getHardware()->getMT()->isSkidFull()) {
					new (this) SortOutThroughSkid;
				} else {
					data->hb.getHardware()->getTL()->turnYellowOn();
					new (this) TransportToDelivery;
				}
			}
		}
	};

	struct SortOutThroughSkid: public PuckOnConveyor2 {
	};

	struct TransportToDelivery: public PuckOnConveyor2 {
		virtual void transact() {
			data->hb.getHardware()->getMotor()->switchOpen();
			data->t1 = data->t2;
			while (data->hb.getHardware()->getMT()->isItemRunningOut()) {}
			data->t2 = clock();
			data->delta = data->t2 - data->t1;
			if (data->delta > DELTA_TW_TE) {
				data->hb.getHardware()->getTL()->turnGreenOff();
				data->hb.getHardware()->getTL()->turnYellowOff();
				data->es.puckLost();
				new (this) DeliveryToConveyor3;//delta checks evtl. erneut nötig?
				//new (this) PuckLost;
			} else if (data->delta < DELTA_TW_TE) {
				data->hb.getHardware()->getTL()->turnGreenOff();
				new (this) PuckAdded;
			} else {
				new (this) DeliveryToConveyor3;
			}
		}
	};

	struct DeliveryToConveyor3: public PuckOnConveyor2 {
		virtual void transact() {
			//Check for conveyor3 isFree() missing!
			new (this) TransportToConveyor3;
		}
	};

	struct TransportToConveyor3: public PuckOnConveyor2 {
		virtual void transact() {
			while (!(data->hb.getHardware()->getMT()->isItemRunningOut())){}
			usleep(4000000);
			data->puckqueue.pop();
			if (data->puckqueue.empty()) {
				data->hb.getHardware()->getMotor()->stop();
			}
			new (this) Finish;
		}
	};


	struct BothSkidsFull: public PuckOnConveyor2 {
		virtual void transact() {
			data->hb.getHardware()->getMotor()->stop();
			data->hb.getHardware()->getTL()->turnGreenOff();
			//Errormessage impelementation missing
			while(!(data->hb.getHardware()->getHMI()->isButtonStartPressed()))
			{
				data->hb.getHardware()->getTL()->turnRedOn();
				usleep(500000);
				data->hb.getHardware()->getTL()->turnRedOff();
				usleep(500000);
			}
			data->hb.getHardware()->getTL()->turnGreenOn();
			//new (this) history
		}
	};

	struct PuckLost: public PuckOnConveyor2 {
		virtual void transact() {
			data->hb.getHardware()->getMotor()->stop();
			//Errormessage implementation missing
			while(!(data->hb.getHardware()->getHMI()->isButtonStartPressed()))
			{
				data->hb.getHardware()->getTL()->turnYellowOn();
				usleep(500000);
				data->hb.getHardware()->getTL()->turnYellowOff();
				usleep(500000);
			}
			data->hb.getHardware()->getTL()->turnYellowOff();
			data->hb.getHardware()->getTL()->turnGreenOn();
			//new (this) history
		}
	};


	struct PuckAdded: public PuckOnConveyor2 {
		virtual void transact() {
			csdata->hb.getHardware()->getMotor()->stop();
			//Errormessage implementation missing
			while(!(data->hb.getHardware()->getHMI()->isButtonStartPressed()))
			{
				data->hb.getHardware()->getTL()->turnRedOn();
				usleep(500000);
				data->hb.getHardware()->getTL()->turnRedOff();
				usleep(500000);
			}
			data->hb.getHardware()->getTL()->turnRedOff();
			data->puckqueue.pop();
			new (this) StateStart;
		}
	};

	struct Finish: public PuckOnConveyor2 {
		virtual void transact() {

		}
	};

	StateStart stateMember; //The memory for the state is part of context object
	Datac cdata;
};

class ErrorStates{

public:
	ErrorStates();
	~ErrorStates();

	void puckLost() {
		hb.getHardware()->getMotor()->stop();
		//Errormessage implementation missing
		while (!(hb.getHardware()->getHMI()->isButtonStartPressed())) {
			hb.getHardware()->getTL()->turnYellowOn();
			usleep(500000);
			hb.getHardware()->getTL()->turnYellowOff();
			usleep(500000);
		}
		hb.getHardware()->getTL()->turnYellowOff();
		hb.getHardware()->getTL()->turnGreenOn();
	}

	void bothSkidsFull() {
		hb.getHardware()->getMotor()->stop();
		hb.getHardware()->getTL()->turnGreenOff();
		//Errormessage impelementation missing
		while (!(hb.getHardware()->getHMI()->isButtonStartPressed())) {
			hb.getHardware()->getTL()->turnRedOn();
			usleep(500000);
			hb.getHardware()->getTL()->turnRedOff();
			usleep(500000);
		}
		hb.getHardware()->getTL()->turnGreenOn();
	}
};

#endif /* CONTEXT_H_ */
