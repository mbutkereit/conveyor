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
#include "Thread/BlinkRedThread.h"
#include "Thread/BlinkYellowThread.h"
#include "ContextTimeout.h"

extern HalBuilder hb; ///< Der HalBuilder um sicher und zentral auf die Hardware zuzugreifen.

struct Data2 {
	Data2(int puckID, std::vector<Puck>* puckVector, int *skidcounter2) :
			puckID(puckID), hb(), cm(ContextMotor::getInstance()), cs(
					ContextSorting::getInstance()), cswitch(
					ContextSwitch::getInstance()), puck(puckID), puckVector(
					puckVector), finished(false), bothSkidsfull(false), posInVector(0), skidOfConveyor2Full(
					false), im(), sc2(skidcounter2), blinkRed(), blinkYellow(), wpm(), cto() {
	}
	int puckID;
	HalBuilder hb;
	ContextMotor* cm;
	ContextSorting* cs;
	ContextSwitch* cswitch;
	Puck puck;
	std::vector<Puck>* puckVector;
	bool finished;
	bool bothSkidsfull;
	int posInVector;
	bool skidOfConveyor2Full;
	InfoMessage im;
	int *sc2;
	BlinkRedThread blinkRed;
	BlinkYellowThread blinkYellow;
	WorkpieceMessage wpm;
	ContextTimeout cto;

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
		virtual void signalTimerTick() {
			data->cto.signalTimerTick();
			if (data->cto.timeoutOccured()) {
				data->hb.getHardware()->getTL()->turnGreenOff();
				data->blinkYellow.start(NULL);
				data->cm->setSpeed(MOTOR_STOP);
				data->cm->transact();
				cout << "TIMEOUT" << endl;
				data->puckVector->erase(
						data->puckVector->begin() + data->posInVector);
				new (this) PuckLost;
			}
		}

		Data2* data; // pointer to data, which physically resides inside the context class (contextdata)
	}*statePtr;   // a pointer to current state. Used for polymorphism.

	struct TransportToEntry: public PuckOnConveyor2 {
		//TRANSACTION/LEAVE
		virtual void signalLBBeginInterrupted() {
			LOG_DEBUG << "State: TransportToEntry Conveyor2\n";
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
			LOG_DEBUG << "State: MotorOn \n";

			struct workpiece_package_without_ch recieve =
					data->wpm.getWorkpieceInfo();

			switch (recieve.workpiece_type) {
			case 0:
				data->puck.setPuckType(DRILL_HOLE_UPSIDE);
				break;

			case 1:
				data->puck.setPuckType(DRILL_HOLE_UPSIDE_METAL);
				break;

			case 2:
				data->puck.setPuckType(DRILL_HOLE_UPSIDE_PLASTIC);
				break;

			case 3:
				data->puck.setPuckType(NO_DRILL_HOLE);
				break;

			case 4:
			default:
				data->puck.setPuckType(TYPE404PT);
				break;
			}
			data->puck.setId(recieve.id);
			data->puck.setHeightReading1(recieve.alimetry_value_one);
			data->puck.setHeightReading2(recieve.alimetry_value_two);
			data->puckVector->push_back(data->puck);
			data->posInVector = data->puckVector->size() - 1;
			//TODO t0 = GIVE TIME!
			data->cto.startTimerT0();
			new (this) TransportToHeightMeasurement;
		}
	};

	struct TransportToHeightMeasurement: public PuckOnConveyor2 {
		virtual void signalLBAltimetryInterrupted() {
			LOG_DEBUG << "State: TransportToHeightMeasurement \n";
			data->cm->setSpeed(MOTOR_SLOW);
			data->cm->transact();
			data->cto.stopTimerT0();
			data->cto.startTimerTH();
			if (1) {   //TODO DELTA t0 and tH OK
				data->hb.getHardware()->getAltimetry()->startAltimetry();
				usleep(20);
				data->puck.setHeightReading2(data->hb.getHardware()->getAltimetry()->getHeight());
				LOG_DEBUG << "Hoehenwert2: "
						<< (int) data->hb.getHardware()->getAltimetry()->getHeight()
						<< "\n";
				cout << "Hoehenwert2: " << data->puck.getHeightReading2() << endl;
				if (data->puck.getHeightReading2() > 3200) {
					LOG_DEBUG << "DRILL_HOLE_UPSIDE\n";
					data->puck.setPuckType(DRILL_HOLE_UPSIDE);
				} else {
					LOG_DEBUG << "NO_DRILL_HOLE\n";
					data->puck.setPuckType(NO_DRILL_HOLE);
				}
				new (this) PuckInHeightMeasurement;
			} else if (0) {   //TODO DELTA t0 AND tH TOO LOW
				data->hb.getHardware()->getTL()->turnGreenOff();
				data->blinkRed.start(NULL);
				data->cm->setSpeed(MOTOR_STOP);
				data->cm->transact();
				cout
						<< "FEHLER!!!!!!!!!!! PUCK WURDE HINZUGEF‹GT!!!!!!!!!!!!!!!!!!!!!!!!"
						<< endl;
				LOG_DEBUG << "Fehler: PUCK WURDE HINZUGEF‹GT \n";
				new (this) PuckAdded;
			}
		}
	};

	struct PuckInHeightMeasurement: public PuckOnConveyor2 {
		virtual void signalLBAltimetryNotInterrupted() {
			LOG_DEBUG << "State: PuckInHeightMeasurenment \n";
			data->cm->resetSpeed(MOTOR_SLOW);
			data->cm->transact();
			new (this) TransportToSwitch;
		}
	};

	struct TransportToSwitch: public PuckOnConveyor2 {
		virtual void signalLBSwitchInterrupted() {
			LOG_DEBUG << "State: TransoprtToSwitch \n";
			//TODO GIVE TIME tW, DELTA th AND tW CALCULATION
			data->cto.stopTimerTH();
			data->cto.startTimerTW();
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
			LOG_DEBUG << "State: Sorting\n";
			if (1) {   //TODO DELTA tH and tW OK
				data->cs->setCurrentPt(data->puck.getPuckType());
				data->cs->transact();
				if (data->cs->getSequenceOk()) {
					LOG_DEBUG << "Sequence OK \n";
					data->cswitch->setSwitchOpen();
					data->cswitch->transact();
					new (this) TransportToDelivery;
				} else {
					LOG_DEBUG << "Sequence not OK \n";
					if (data->hb.getHardware()->getMT()->isSkidFull()) {
						LOG_DEBUG << "Skid Not Full\n";
						new (this) SortOutThroughSkid;
					} else {
						LOG_DEBUG << "Rutsche voll: " << data->im.istBand1RutscheVoll() << "\n";
						if(data->im.istBand1RutscheVoll()){
							LOG_DEBUG << "Both Skids full\n";
							data->bothSkidsfull = true;
							new (this) EndOfTheEnd;
						}
						else{
						LOG_DEBUG << "Skid Full\n";
							data->hb.getHardware()->getTL()->turnGreenOff();
							data->hb.getHardware()->getTL()->turnYellowOn();
							data->cm->setSpeed(MOTOR_STOP);
							data->cm->transact();
							data->im.setBand2RutscheVoll();
						}
					}
				}
			} else if (0) {   //TODO DELTA tH AND tW TOO LOW
				data->hb.getHardware()->getTL()->turnGreenOff();
				data->blinkRed.start(NULL);
				data->cm->setSpeed(MOTOR_STOP);
				data->cm->transact();
				cout
						<< "FEHLER!!!!!!!!!!! PUCK WURDE HINZUGEF‹GT!!!!!!!!!!!!!!!!!!!!!!!!"
						<< endl;
				LOG_DEBUG << "Fehler: PUCK WURDE HINZUGEF‹GT \n";
				new (this) PuckAdded;
			}
		}

		virtual void skidOfConveyor2Cleared() {
			LOG_DEBUG << "State: skidOfConveyor2Cleared \n";
			new (this) SortOutThroughSkid;
		}
	};

	struct SortOutThroughSkid: public PuckOnConveyor2 {
		virtual void signalLBSkidInterrupted() {
			LOG_DEBUG << "State: SortOutThroughSkid \n";

			data->puckVector->erase(
					data->puckVector->begin() + data->posInVector);

			int temp = *data->sc2;
			temp++;
			*data->sc2 = temp;
			LOG_DEBUG << "Skidcounter: " << *data->sc2 << "\n";
			if (*data->sc2 > 3) {
				data->im.setBand2RutscheVoll();
				LOG_DEBUG << "Rutsche 2 voll\n";
			}
			if (data->puckVector->size() > 0) {
				LOG_DEBUG << "State: SortOutThroughSkid --> Band2 nicht leer \n";
				data->finished = true;
				new (this) EndOfTheEnd;
			} else {
				LOG_DEBUG << "State: SortOutThroughSkid --> Band2 leer \n";
				data->cm->setSpeed(MOTOR_STOP);
				LOG_DEBUG << "State: SortOutThroughSkid --> Band2 leer --> Motor gestoppt\n";
				data->cm->transact();
				new (this) Conveyor2Empty;
			}
		}
	};

	struct Conveyor2Empty: public PuckOnConveyor2 {
		virtual void signalLBBeginInterrupted() {
			LOG_DEBUG << "State: Conveyor2Empty \n";
			data->cm->resetSpeed(MOTOR_STOP);
			LOG_DEBUG << "State: Conveyor1Empty --> Motor reset stopp\n";
			data->cm->transact();
			data->im.setBand2Frei();
			data->finished = true;
			new (this) EndOfTheEnd;
		}
	};

	struct TransportToDelivery: public PuckOnConveyor2 {
		virtual void signalLBEndInterrupted() {
			LOG_DEBUG << "State: TransportToDelivery \n";
			//TODO tE = GIVE TIME, CALCULATE tW AND tE
			data->cto.stopTimerTW();
			data->cswitch->resetSwitchOpen();
			data->cswitch->transact();
			if (1) {   //TODO DELTA tW and tE OK
				cout << "ID: " <<

				cout << data->puck.getId() << endl;
				cout << "Height on Conveyor1: " <<

				cout << data->puck.getHeightReading1() << endl;
				cout << "Height on Conveyor2: " <<

				cout << data->puck.getHeightReading2() << endl;
				cout << "Puck Type: " <<

				cout << data->puck.getPuckType() << endl;

				new (this) DeliverToConveyor3;

			} else if (0) { //TODO DELTA tW AND tE TOO LOW
				data->hb.getHardware()->getTL()->turnGreenOff();
				data->blinkRed.start(NULL);
				data->cm->setSpeed(MOTOR_STOP);
				data->cm->transact();
				cout
						<< "FEHLER!!!!!!!!!!! PUCK WURDE HINZUGEF‹GT!!!!!!!!!!!!!!!!!!!!!!!!"
						<< endl;
				LOG_DEBUG << "Fehler: PUCK WURDE HINZUGEF‹GT \n";
				new (this) PuckAdded;
			}
		}
	};

	struct DeliverToConveyor3: public PuckOnConveyor2 { //TODO: signalLBNextConveyorNotInterrupted
		virtual void signalLBNextConveyor() {
			LOG_DEBUG << "State: DeliverToConveyor3 \n";
			int drillHoleUpside = 0;
			int drillHoleUpsideMetal = 1;
			int drillHoleUpsidePlastic = 2;
			int noDrillHole = 3;
			int type404Pt = 4;
			int sendPuckType;

			switch (data->puck.getPuckType()) {
			case DRILL_HOLE_UPSIDE:
				sendPuckType = drillHoleUpside;
				break;

			case DRILL_HOLE_UPSIDE_METAL:
				sendPuckType = drillHoleUpsideMetal;
				break;

			case DRILL_HOLE_UPSIDE_PLASTIC:
				sendPuckType = drillHoleUpsidePlastic;
				break;

			case NO_DRILL_HOLE:
				sendPuckType = noDrillHole;
				break;

			case TYPE404PT:
			default:
				sendPuckType = type404Pt;
				break;

			}
			data->wpm.send(data->puck.getHeightReading1(),
					data->puck.getHeightReading2(), sendPuckType,
					data->puck.getId());

			data->puckVector->erase(
					data->puckVector->begin() + data->posInVector);
			data->cm->setSpeed(MOTOR_STOP);
			data->cm->transact();
			new (this) Conveyor2Empty;

		}
	};

	struct PuckAdded: public PuckOnConveyor2 {
		virtual void signalReset() {
			LOG_DEBUG << "State: PuckAdded \n";
			data->blinkRed.stop();
			data->im.setBand2Frei();
			data->finished = true;
			new (this) EndOfTheEnd;
		}
	};

	struct PuckLost: public PuckOnConveyor2 {
		virtual void signalReset() {
			data->blinkYellow.stop();
			data->hb.getHardware()->getTL()->turnYellowOff();
			data->hb.getHardware()->getTL()->turnGreenOn();
			data->cm->resetSpeed(MOTOR_STOP);
			data->cm->transact();
			data->finished = true;
			new (this) EndOfTheEnd;
		}
	};

	struct EndOfTheEnd: public PuckOnConveyor2 {
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
		virtual void signalLBEndInNotInterrupted() {
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
		virtual void signalTimerTick() {
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
	 *return: gibt true zur√ºck wenn der Context den Enzustand erreicht hat und false wenn Context noch nicht in einem Enzustand ist.
	 */
	bool isContextimEnzustand() {
		if (contextdata.finished) {
			LOG_DEBUG << "Ich bin jetzt im Endzustand \n";
		}
		return contextdata.finished;
	}

	bool bothSkidsFull(){
		return contextdata.bothSkidsfull;
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

	void signalTimerTick();
};

#endif /* CONTEXTCONVEYOR2_H_ */
