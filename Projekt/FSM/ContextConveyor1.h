/*
 * ContextConveyor1.h
 *
 *  Created on: 02.12.2016
 *      Author: abx827
 */

#ifndef CONTEXTCONVEYOR1_H_
#define CONTEXTCONVEYOR1_H_
#define DELTA_T0_TH 0
#define DELTA_TH_TW 0
#define DELTA_TW_TE 0
#define HEIGHT_DRILL 0
#define HEIGHT_NO_DRILL 0

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
#include "Thread/BlinkRedThread.h"
#include "Thread/BlinkYellowThread.h"
#include "ContextI.h"

extern HalBuilder hb; ///< Der HalBuilder um sicher und zentral auf die Hardware zuzugreifen.

struct Data {
	Data(int puckID, std::vector<Puck>* puckVector, int *skidcounter) :
			puckID(puckID), hb(), cm(ContextMotor::getInstance()), cs(
					ContextSorting::getInstance()), cswitch(
					ContextSwitch::getInstance()), puck(puckID), puckVector(
					puckVector), finished(false), posInVector(0), im(), sc(
					skidcounter), blinkRed(), blinkYellow(), wpm(), tickX(10) {
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
	InfoMessage im;
	int *sc;
	BlinkRedThread blinkRed;
	BlinkYellowThread blinkYellow;
	WorkpieceMessage wpm;
	int tickX;

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
class ContextConveyor1 {
private:
	struct PuckOnConveyor1 { //top-level state
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
		virtual void signalLBNextConveyor() {
		}
		virtual void signalTimerTick(){
            data->tickX--;
            cout << data->tickX << endl;
            if(data->tickX == 0){
                data->hb.getHardware()->getTL()->turnGreenOff();
                data->blinkYellow.start(NULL);
                data->cm->setSpeed(MOTOR_STOP);
                data->cm->transact();
                cout << "TIMEOUT" << endl;
                data->puckVector->erase(data->puckVector->begin() + data->posInVector);
                new (this) PuckLost;
            }
		}

		Data* data; // pointer to data, which physically resides inside the context class (contextdata)
	}*statePtr;   // a pointer to current state. Used for polymorphism.

	struct TransportToEntry: public PuckOnConveyor1 {
		//TRANSACTION/LEAVE
		void signalLBBeginInterrupted() {
			LOG_DEBUG << "State: TransportToEntry \n";

			data->hb.getHardware()->getTL()->turnGreenOn();

			data->cm->setSpeed(MOTOR_FAST);

			data->cm->transact();
			new (this) MotorOn;
		}
	};

	struct MotorOn: public PuckOnConveyor1 {
		//LEAVE
		void signalLBBeginNotInterrupted() {
			LOG_DEBUG << "State: MotorOn\n";
			//TODO t0 = GIVE TIME, START TIMER(t0)!
			data->puckVector->push_back(data->puck);
			data->posInVector = data->puckVector->size() - 1;
			new (this) TransportToHeightMeasurement;
		}
	};

	struct TransportToHeightMeasurement: public PuckOnConveyor1 {
		virtual void signalLBAltimetryInterrupted() {
			LOG_DEBUG << "State: TransportToHeightMeasurement \n";
			data->cm->setSpeed(MOTOR_SLOW);
			data->cm->transact();
			if (1) {   //TODO DELTA t0 and tH OK
				data->hb.getHardware()->getAltimetry()->startAltimetry();
				usleep(20);
				data->puck.setHeightReading1(data->hb.getHardware()->getAltimetry()->getHeight());
				LOG_DEBUG
						<< "Heohenwert1: "
						<< (int) data->hb.getHardware()->getAltimetry()->getHeight()
						<< "\n";
				if (!(hb.getHardware()->getMT()->isItemInAltimetryToleranceRange())) {
					LOG_DEBUG << "DRILL_HOLE_UPSIDE\n";
					data->puck.setPuckType(DRILL_HOLE_UPSIDE);

				} else {
					LOG_DEBUG << "NO_DRILL_HOLE\n";
					data->puck.setPuckType(NO_DRILL_HOLE);
				}

				new (this) PuckInHeightMeasurement;
			} else if (0) { //TODO DELTA t0 AND tH TOO LOW
				data->hb.getHardware()->getTL()->turnGreenOff();

				data->blinkRed.start(NULL);

				data->cm->setSpeed(MOTOR_STOP);
				data->cm->transact();
				cout
						<< "FEHLER!!!!!!!!!!! PUCK WURDE HINZUGEF�GT!!!!!!!!!!!!!!!!!!!!!!!!"
						<< endl;
				LOG_DEBUG << "Fehler: PUCK WURDE HINZUGEF�GT \n";
				new (this) PuckAdded;
			}
		}
	};

	struct PuckInHeightMeasurement: public PuckOnConveyor1 {
		virtual void signalLBAltimetryNotInterrupted() {
			LOG_DEBUG << "State: PuckInHeightMeasurement \n";
			data->cm->resetSpeed(MOTOR_SLOW);
			data->cm->transact();
			new (this) TransportToSwitch;
		}
	};

	struct TransportToSwitch: public PuckOnConveyor1 {
		virtual void signalLBSwitchInterrupted() {
			LOG_DEBUG << "State: TransportToSwitch \n";
			//TODO STOP TIMER tH, GIVE TIME tW, START TIMER tW, DELTA th AND tW CALCULATION
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

	struct Sorting: public PuckOnConveyor1 {
		virtual void sensorMeasurementCompleted() {
			LOG_DEBUG << "State: Sorting \n";
			if (1) { //TODO DELTA tH and tW OK

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
						LOG_DEBUG << "Skid Full\n";
						data->hb.getHardware()->getTL()->turnYellowOn();
						data->cswitch->setSwitchOpen();
						data->cswitch->transact();
						new (this) TransportToDelivery;
					}
				}
			} else if (0) { //TODO DELTA tH AND tW TOO LOW
				data->hb.getHardware()->getTL()->turnGreenOff();

				data->blinkRed.start(NULL);

				data->cm->setSpeed(MOTOR_STOP);
				data->cm->transact();
				cout
						<< "FEHLER!!!!!!!!!!! PUCK WURDE HINZUGEF�GT!!!!!!!!!!!!!!!!!!!!!!!!"
						<< endl;
				LOG_DEBUG << "Fehler: PUCK WURDE HINZUGEF�GT \n";
				new (this) PuckAdded;
			}
		}
	};

	struct SortOutThroughSkid: public PuckOnConveyor1 {
		virtual void signalLBSkidInterrupted() {
			LOG_DEBUG << "State: SortOutThroughSkid \n";
			int temp = *data->sc;
			temp++;
			*data->sc = temp;
			LOG_DEBUG << "Skidcounter: "<< *data->sc<< "\n";
			if (*data->sc > 3) {
				data->im.setBand1RutscheVoll();
				LOG_DEBUG << "Rutsche 1 voll\n";

			}

			if (data->puckVector->size() > 0) {
				data->finished = true;
			} else {
				data->cm->setSpeed(MOTOR_STOP);
				data->cm->transact();
				new (this) Conveyor1Empty;
			}
		}
	};

	struct Conveyor1Empty: public PuckOnConveyor1 {
		virtual void signalLBBeginInterrupted() {
			LOG_DEBUG << "State: Conveyor1Empty \n";
			data->cm->resetSpeed(MOTOR_STOP);
			data->cm->transact();
			data->finished = true;

		}
	};

	struct TransportToDelivery: public PuckOnConveyor1 {
		virtual void signalLBEndInterrupted() {
			LOG_DEBUG << "State: TransportToDelivery \n";
			//TODO STOP TIME(tW), tE = GIVE TIME, CALCULATE tW AND tE
			data->cswitch->resetSwitchOpen();
			data->cswitch->transact();
			if (1) { //TODO DELTA tW and tE OK
				data->cm->setSpeed(MOTOR_STOP);
				data->cm->transact();
				while (!(data->im.istBand2Frei())) {
				}
				data->cm->resetSpeed(MOTOR_STOP);
				data->cm->transact();
				new (this) TransportToConveyor2;
			} else if (0) { //TODO DELTA tW AND tE TOO LOW
				data->hb.getHardware()->getTL()->turnGreenOff();

				data->blinkRed.start(NULL);

				data->cm->setSpeed(MOTOR_STOP);
				data->cm->transact();
				cout
						<< "FEHLER!!!!!!!!!!! PUCK WURDE HINZUGEF�GT!!!!!!!!!!!!!!!!!!!!!!!!"
						<< endl;
				LOG_DEBUG << "Fehler: PUCK WURDE HINZUGEF�GT \n";
				new (this) PuckAdded;
			}
		}
	};

	struct TransportToConveyor2: public PuckOnConveyor1 {
		virtual void signalLBNextConveyor() {
			LOG_DEBUG << "State: TransportToConveyor2 \n";
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
			LOG_DEBUG << "Daten gesendet \n";

			data->puckVector->erase(
					data->puckVector->begin() + data->posInVector);

			if (data->puckVector->size() > 0) {

				data->finished = true;
			} else {

				data->cm->setSpeed(MOTOR_STOP);
				data->cm->transact();
				new (this) Conveyor1Empty;
			}

		}
	};

	struct PuckAdded: public PuckOnConveyor1 {
		virtual void signalReset() {
			LOG_DEBUG << "State: PuckAdded \n";
			data->blinkRed.stop();
			data->finished = true;
		}
	};

	struct PuckLost: public PuckOnConveyor1 {
	    virtual void signalReset(){
	        data->blinkYellow.stop();
	        data->hb.getHardware()->getTL()->turnYellowOff();
	        data->hb.getHardware()->getTL()->turnGreenOn();
	        data->cm->resetSpeed(MOTOR_STOP);
	        data->cm->transact();
	        data->finished = true;
	    }
	};

	TransportToEntry stateMember; //The memory for the state is part of context object
	Data contextdata;  //Data is also kept inside the context object

public:

	/**
	 *  Constructor des Contexts.
	 */
	ContextConveyor1(int, std::vector<Puck>*, int*);

	/**
	 *  Destructor des Contexts.
	 */
	virtual ~ContextConveyor1();

	/**
	 *
	 *return: gibt true zurück wenn der Context den Enzustand erreicht hat und false wenn Context noch nicht in einem Enzustand ist.
	 */
	bool isContextimEnzustand() {
		if (contextdata.finished){
			LOG_DEBUG << "Ich bin jetzt im Endzustand \n";
		}
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

	void signalLBNextConveyor();

	void signalTimerTick();
};

#endif /* CONTEXTCONVEYOR1_H_ */
