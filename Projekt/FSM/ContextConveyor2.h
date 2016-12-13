/*
 * ContextConveyor2.h
 *
 *  Created on: 02.12.2016
 *      Author: abx827
 */

#ifndef CONTEXTCONVEYOR2_H_
#define CONTEXTCONVEYOR2_H_

#include <iostream>
#include "Logger/Logger.h"
#include "Hal/HalBuilder.h"
#include "ContextMotor.h"
#include "Puck.h"
#include <vector>
#include "ContextSorting.h"
#include "ContextSwitch.h"
#include "Serializer/Serializer.h"
#include "Serializer/InfoMessage.h"
#include "Serializer/WorkpieceMessage.h"
#include "ContextI.h"

extern HalBuilder hb; ///< Der HalBuilder um sicher und zentral auf die Hardware zuzugreifen.

struct Data2 {
	Data2(int puckID, std::vector<Puck>* puckVector, int *skidcounter2) :
			puckID(puckID), hb(), cm(ContextMotor::getInstance()), cs(
					ContextSorting::getInstance()), cswitch(
					ContextSwitch::getInstance()), puck(puckID), puckVector(
					puckVector), finished(false), posInVector(0), skidOfConveyor2Full(
					false), im(), sc2(skidcounter2) {
	}
	int puckID;
	HalBuilder hb;
	ContextMotor* cm;
	ContextSorting* cs;
	ContextSwitch* cswitch;
	Puck puck;
	std::vector<Puck>* puckVector;
	bool finished;
	int posInVector;
	bool skidOfConveyor2Full;
	InfoMessage im;
	int *sc2;
};

/**
 * @file
 * @section DESCRIPTION
 *
 * Eine Klasse die einen Context aus dem State Pattern darstellt.
 * (Soll verwendet werden um den ersten Automaten zu Implementieren.)
 *
 * Example: http://prg.phoenix.uberspace.de/2016/04/19/state-machine/
 */
class ContextConveyor2 {
private:
	struct PuckOnConveyor2 { //top-level state
		virtual void signalLBBeginInterrupted() {
		}
		virtual void signalLBEndInterrupted() {
		}
		virtual void signalLBAltimetryInterrupted() {
		}
		virtual void signalLBSwitchInterrupted() {
		}
		virtual void signalLBBeginNotInterrupted() {
		}
		virtual void signalLBEndNotInterrupted() {
		}
		virtual void signalLBAltimetryNotInterrupted() {
		}
		virtual void signalLBSwitchNotInterrupted() {
		}
		virtual void signalEStop() {
		}
		virtual void signalStart() {
		}
		virtual void signalStop() {
		}
		virtual void signalReset() {
		}
		virtual void signalLBSkidInterrupted() {
		}
		virtual void signalLBSkidNotInterrupted() {
		}
		virtual void signalAltimetryCompleted() {
		}
		virtual void sensorMeasurementCompleted() {
		}
		virtual void skidOfConveyor2Cleared() {
		}
		virtual void signalLBNextConveyor() {
		}

		//TODO SIGNALS THAT ARE MISSING
		virtual void signalTimeout() {
		}

		Data2* data; // pointer to data, which physically resides inside the context class (contextdata)
	}*statePtr;   // a pointer to current state. Used for polymorphism.

	struct TransportToEntry: public PuckOnConveyor2 {
		//TRANSACTION/LEAVE
		virtual void signalLBBeginInterrupted() {
			data->im.setBand2NichtFrei();
			data->hb.getHardware()->getTL()->turnGreenOn();
			data->cm->setSpeed(MOTOR_FAST);
			data->cm->transact();
			new (this) MotorOn;
		}
	};

	struct MotorOn: public PuckOnConveyor2 {
		//LEAVE
		virtual void signalLBBeginNotInterrupted() {
			data->puckVector->push_back(data->puck);
			data->posInVector = data->puckVector->size() - 1;
			//TODO t0 = GIVE TIME, START TIMER(t0)!
			new (this) TransportToHeightMeasurement;
		}
	};

	struct TransportToHeightMeasurement: public PuckOnConveyor2 {
		virtual void signalLBAltimetryInterrupted() {
			data->cm->setSpeed(MOTOR_SLOW);
			data->cm->transact();
			if (1) {   //TODO DELTA t0 and tH OK
				data->puck.setHeightReading1(
						data->hb.getHardware()->getAltimetry()->getHeight());
				if (data->hb.getHardware()->getMT()->isItemInAltimetryToleranceRange()) {
					data->puck.setPuckType(DRILL_HOLE_UPSIDE);
				} else {
					data->puck.setPuckType(NO_DRILL_HOLE);
				}
				new (this) PuckInHeightMeasurement;
			} else if (0) {   //TODO DELTA t0 AND tH TOO LOW
				data->hb.getHardware()->getTL()->turnGreenOff();
				data->hb.getHardware()->getTL()->turnRedOn(); //TODO ACTUALLY SHOULD BLINK!
				data->cm->setSpeed(MOTOR_STOP);
				data->cm->transact();
				new (this) PuckAdded;
			}
		}
	};

	struct PuckInHeightMeasurement: public PuckOnConveyor2 {
		virtual void signalLBAltimetryNotInterrupted() {
			data->cm->resetSpeed(MOTOR_SLOW);
			data->cm->transact();
			new (this) TransportToSwitch;
		}
	};

	struct TransportToSwitch: public PuckOnConveyor2 {
		virtual void signalLBSwitchInterrupted() {
			//TODO STOP TIMER tH, GIVE TIME tW, DELTA th AND tW CALCULATION
			if (data->puck.getPuckType() == DRILL_HOLE_UPSIDE) {
				if (data->hb.getHardware()->getMT()->isItemMetal()) {

					data->puck.setPuckType(DRILL_HOLE_UPSIDE_METAL);
				} else {

					data->puck.setPuckType(DRILL_HOLE_UPSIDE_PLASTIC);
				}
			}
			new (this) Sorting;
		}
	};

	struct Sorting: public PuckOnConveyor2 {
		virtual void sensorMeasurementCompleted() {
			if (1) {   //TODO DELTA tH and tW OK
				data->cs->setCurrentPt(data->puck.getPuckType());

				data->cs->transact();
				if (data->cs->getSequenceOk()) {
					data->cswitch->setSwitchOpen();
					data->cswitch->transact();
					new (this) TransportToDelivery;
				} else {
					if (data->hb.getHardware()->getMT()->isSkidFull()) {
						new (this) SortOutThroughSkid;
					} else {
						data->hb.getHardware()->getTL()->turnYellowOn();
						data->cm->setSpeed(MOTOR_STOP);
						data->cm->transact();
						data->im.setBand2RutscheVoll();
					}
				}
			} else if (0) {   //TODO DELTA tH AND tW TOO LOW
				data->hb.getHardware()->getTL()->turnGreenOff();
				data->hb.getHardware()->getTL()->turnRedOn(); //TODO ACTUALLY SHOULD BLINK!
				data->cm->setSpeed(MOTOR_STOP);
				data->cm->transact();
				new (this) PuckAdded;
			}
		}

		virtual void skidOfConveyor2Cleared() {
			new (this) SortOutThroughSkid;
		}
	};

	struct SortOutThroughSkid: public PuckOnConveyor2 {
		virtual void signalLBSkidInterrupted() {
			int temp = *data->sc2;
			temp++;
			*data->sc2 = temp;
			if (*data->sc2 > 3) {
				data->im.setBand2RutscheVoll();

			}
			if (data->puckVector->size() > 0) {
				data->finished = true;
			} else {
				data->cm->setSpeed(MOTOR_STOP);
				data->cm->transact();
				new (this) Conveyor2Empty;
			}
		}
	};

	struct Conveyor2Empty: public PuckOnConveyor2 {
		virtual void signalLBBeginInterrupted() {
			data->cm->resetSpeed(MOTOR_STOP);
			data->cm->transact();
			data->im.setBand2Frei();
			data->finished = true;
		}
	};

	struct TransportToDelivery: public PuckOnConveyor2 {
		virtual void signalLBEndInterrupted() {
			//TODO STOP TIME(tW), tE = GIVE TIME, CALCULATE tW AND tE
			data->cswitch->resetSwitchOpen();
			data->cswitch->transact();
			if (1) { //TODO DELTA tW and tE OK
				data->cm->setSpeed(MOTOR_STOP);
				data->cm->transact();
				new (this) DeliverToConveyor3;
			} else if (0) { //TODO DELTA tW AND tE TOO LOW
				data->hb.getHardware()->getTL()->turnGreenOff();
				data->hb.getHardware()->getTL()->turnRedOn(); //TODO ACTUALLY SHOULD BLINK!
				data->cm->setSpeed(MOTOR_STOP);
				data->cm->transact();
				new (this) PuckAdded;
			}
		}
	};

	struct DeliverToConveyor3: public PuckOnConveyor2 {
		virtual void signalConveyor3isFree() {
			data->cm->resetSpeed(MOTOR_STOP);
			data->cm->transact();
			new (this) TransportToConveyor3;
		}
	};

	struct TransportToConveyor3: public PuckOnConveyor2 {
		virtual void signalLBNextConveyor() {
			//TODO SEND PUCKINFORMATION
			data->puckVector->erase(
					data->puckVector->begin() + data->posInVector);
			data->cm->setSpeed(MOTOR_STOP);
			data->cm->transact();
			new (this) Conveyor2Empty;

		}
	};

	struct PuckAdded: public PuckOnConveyor2 {
		virtual void signalReset() {
			data->hb.getHardware()->getTL()->turnRedOff();
			data->im.setBand2Frei();
			data->finished = true;
		}
	};

	TransportToEntry stateMember; //The memory for the state is part of context object
	Data2 contextdata;  //Data is also kept inside the context object

public:

	/**
	 *  Constructor des Contexts.
	 */
	ContextConveyor2(int, std::vector<Puck>*, int*);

	/**
	 *  Destructor des Contexts.
	 */
	virtual ~ContextConveyor2();

	/**
	 *
	 *return: gibt true zurück wenn der Context den Enzustand erreicht hat und false wenn Context noch nicht in einem Enzustand ist.
	 */
	bool isContextimEnzustand() {
		return contextdata.finished;
	}

	void signalLBBeginInterrupted();

	void signalLBEndInterrupted();

	void signalLBAltimetryInterrupted();

	void signalLBSwitchInterrupted();

	void signalLBBeginNotInterrupted();

	void signalLBEndNotInterrupted();

	void signalLBAltimetryNotInterrupted();

	void signalLBSwitchNotInterrupted();

	void signalEStop();

	void signalStart();

	void signalStop();

	void signalReset();

	void signalLBSkidInterrupted();

	void signalLBSkidNotInterrupted();

	void signalAltimetryCompleted();

	void sensorMeasurementCompleted();

	virtual void skidOfConveyor2Cleared();

	void signalLBNextConveyor();
};

#endif /* CONTEXTCONVEYOR2_H_ */
