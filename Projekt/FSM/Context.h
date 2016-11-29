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
#define HM1_HM2_TOLERANCE 0

#include <iostream>
#include <ctime>
#include <map>
#include <unistd.h>
#include "Puck.h"
#include "Logger/Logger.h"
#include "Hal/HalBuilder.h"
#include "ContextSorting.h"
#include "ErrorStates.h"
#include "FSM/ContextMotor.h"
#include "FSM/MotorOptions.h"

using namespace std;
extern HalBuilder hb;

struct Datac {
	Datac(ContextSorting* csorting, ContextMotor* cmotor) :
			hb(), cs(csorting), cm(cmotor), puck(-1), puckmap(), es(), id(0), height(-1), t1(0), t2(0), delta(0) {
	}
	HalBuilder hb;
	ContextSorting* cs;
	ContextMotor* cm;
	Puck puck;
	map<int,Puck> puckmap;
	ErrorStates es;
	int id;
	int height;
	clock_t t1;
	clock_t t2;
	clock_t delta;
};

class Context {
private:
	Datac cdata;

	struct PuckOnConveyor2 { //top-level state
		Datac* data;
		virtual void signalEStop() {
			//struct PuckOnConveyor1* hs;
			data->cm->setSpeed(STOP);
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
			if (data->hb.getHardware()->getMT()->isSkidFull()) { //Rutsche 1 voll Abfrage fehlt!
				data->es.bothSkidsFull();
				new (this) TransportToEntry;
			} else {
				new (this) TransportToEntry;
			}
		}
	};

	StateStart stateMember; //The memory for the state is part of context object

	struct TransportToEntry: public PuckOnConveyor2 {
		virtual void transact() {
			data->hb.getHardware()->getTL()->turnGreenOn();
			while (data->hb.getHardware()->getMT()->isItemRunningIn()) {
			} //LS_E BAND 1 müsste stattdessen geprüft werden!
			new (this) MotorOn;
		}
	};

	struct MotorOn: public PuckOnConveyor2 { //ID wird von Band1 vergeben
		virtual void transact() {
			int id = (data->id);
			Puck puck(id);
			data->puck = puck;
			data->hb.getHardware()->getMotor()->right();
			data->hb.getHardware()->getMotor()->fast();
			data->puckmap.insert(pair<int, Puck>(id, puck));
			while (!(data->hb.getHardware()->getMT()->isItemRunningIn())) {
			}
			data->t1 = clock();
			new (this) TransportToHeightMeasurement;
		}
	};

	struct TransportToHeightMeasurement: public PuckOnConveyor2 {
		virtual void transact() {
			while (data->hb.getHardware()->getMT()->isItemAltimetry()) {
			}
			//t_0 and t_H
			data->t2 = clock();
			data->delta = data->t2 - data->t1;
			//data->hb.getHardware()->getMotor()->right();
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
			while (!(data->hb.getHardware()->getMT()->isItemAltimetry())) {
			}
			data->hb.getHardware()->getMotor()->right();
			data->hb.getHardware()->getMotor()->fast();
			new (this) TransPortToSkid;
		}
	};

	struct TransPortToSkid: public PuckOnConveyor2 {
		virtual void transact() {
			while (data->hb.getHardware()->getMT()->isItemSwitch()) {
			}
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
				data->puckmap.erase(data->puck.getId());
				data->es.puckLost();
				new (this) StateStart;
				//new (this) PuckLost;
			} else {
				new (this) Sorting;
			}
		}
	};

	struct Sorting: public PuckOnConveyor2 {
		virtual void transact() { //TRANSITION IS NOT DEFINED
			data->cs->setCurrentPh(data->puck.getPuckdrillhole());
			data->cs->setCurrentPm(data->puck.getPuckmaterial());
			data->cs->transact();
			if (data->cs->getSequenceOk()) {
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
			while (data->hb.getHardware()->getMT()->isItemRunningOut()) {
			}
			data->t2 = clock();
			data->delta = data->t2 - data->t1;
			if (data->delta > DELTA_TW_TE) {
				data->hb.getHardware()->getTL()->turnGreenOff();
				data->hb.getHardware()->getTL()->turnYellowOff();
				data->es.puckLost();
				new (this) DeliveryToConveyor3; //delta checks evtl. erneut nötig?
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
			while (!(data->hb.getHardware()->getMT()->isItemRunningOut())) {
			}
			usleep(4000000);
			data->puckmap.erase(data->puck.getId());
			if (data->puckmap.empty()) {
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
			while (!(data->hb.getHardware()->getHMI()->isButtonStartPressed())) {
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
			while (!(data->hb.getHardware()->getHMI()->isButtonStartPressed())) {
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

			data->hb.getHardware()->getMotor()->stop();
			//Errormessage implementation missing
			while (!(data->hb.getHardware()->getHMI()->isButtonStartPressed())) {
				data->hb.getHardware()->getTL()->turnRedOn();
				usleep(500000);
				data->hb.getHardware()->getTL()->turnRedOff();
				usleep(500000);
			}
			data->hb.getHardware()->getTL()->turnRedOff();
			data->puckmap.erase(data->puck.getId());
			new (this) StateStart;
		}
	};

	struct Finish: public PuckOnConveyor2 {
		virtual void transact() {

		}
	};

public:

	Context(ContextSorting* csorting, ContextMotor* cmotor) :
			statePtr(&stateMember), cdata(csorting, cmotor) // assigning start state
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
};

#endif /* CONTEXT_H_ */
