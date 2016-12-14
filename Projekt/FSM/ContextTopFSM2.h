/*
 * ContextTopFSM.h
 *
 *  Created on: 07.12.2016
 *      Author: abs949
 */

#ifndef CONTEXTTOPFSM2_H_
#define CONTEXTTOPFSM2_H_

#include <iostream>
#include <ContextConveyor2.h>
#include "Logger/Logger.h"
#include "Hal/HalBuilder.h"
#include <vector>
#include <Puck.h>
#include "ContextMotor.h"
#include "Serializer/Serializer.h"
#include "Serializer/InfoMessage.h"
#include "Serializer/WorkpieceMessage.h"

extern HalBuilder hb; ///< Der HalBuilder um sicher und zentral auf die Hardware zuzugreifen.

struct TOPData2 {
	TOPData2(int puckID, std::vector<Puck>* puckVector, int *skidcounter2) :
			cc2(puckID, puckVector, skidcounter2), cm(ContextMotor::getInstance()), hb(), im() {
	}
	ContextConveyor2 cc2;
	ContextMotor *cm;
	HalBuilder hb;
	InfoMessage im;
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
class ContextTopFSM2: public  ContextI {
private:
	struct TOPFSM { //top-level state
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

		virtual void signalLBNextConveyor() {
		}

		TOPData2* data; // pointer to data, which physically resides inside the context class (contextdata)
	}*statePtr;   // a pointer to current state. Used for polymorphism.

	struct MainState: public TOPFSM {

		void signalLBBeginInterrupted() {
			LOG_DEBUG <<"State: MainState \n";

			data->cc2.signalLBBeginInterrupted();
		}
		void signalLBBeginNotInterrupted() {
			LOG_DEBUG <<"State: MainState \n";

			data->cc2.signalLBBeginNotInterrupted();
		}
		void signalLBEndInterrupted() {
			LOG_DEBUG <<"State: MainState \n";

			data->cc2.signalLBEndInterrupted();
		}
		void signalLBEndNotInterrupted() {
			LOG_DEBUG <<"State: MainState \n";

			data->cc2.signalLBEndNotInterrupted();
		}
		void signalLBAltimetryInterrupted() {
			LOG_DEBUG <<"State: MainState \n";

			data->cc2.signalLBAltimetryInterrupted();
		}
		void signalLBAltimetryNotInterrupted() {
			LOG_DEBUG <<"State: MainState \n";

			data->cc2.signalLBAltimetryNotInterrupted();
		}

		void signalLBSwitchInterrupted() {
			LOG_DEBUG <<"State: MainState \n";

			data->cc2.signalLBSwitchInterrupted();
			if (data->im.istBand1RutscheVoll()
					&& data->im.istBand2RutscheVoll()) {
				data->hb.getHardware()->getTL()->turnGreenOff();
				data->hb.getHardware()->getTL()->turnRedOn();
				data->cm->setSpeed(MOTOR_STOP);
				data->cm->transact();
				LOG_DEBUG <<"Fehler: BEIDE RUTSCHEN SIND VOLL \n";
				cout<<"FEHLER!!!!!!!!!!! BEIDE RUTSCHEN SIND VOLL!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;

				new (this) BothSkidsFull;
			} else {
				data->cc2.sensorMeasurementCompleted();
				if (data->im.istBand2RutscheVoll()) {
					LOG_DEBUG <<"Fehler:RUTSCHE 2 IST VOLL \n";
					cout<<"FEHLER!!!!!!!!!!! RUTSCHE 2 IST VOLL!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;
					new (this) SkidOfConveyor2Full;
				}
			}
		}
		void signalLBSwitchNotInterrupted() {
			LOG_DEBUG <<"State: MainState \n";
														//ACTUALLY NOT EXECUTABLE BECAUSE OF signalLBSwitchInterrupted()
			data->cc2.signalLBSwitchNotInterrupted();
		}
		void signalEStop() {
			LOG_DEBUG <<"State: MainState \n";

			data->cm->setSpeed(MOTOR_STOP);
			data->cm->transact();
			if (data->hb.getHardware()->getHMI()->isButtonEStopPressed()) {
				data->im.setESTOP();
			}
			LOG_DEBUG <<"E-STOP WURDE GEDRUECKT \n";
			cout<<"!!!!!!!!!!! E-STOP WURDE GEDRUECKT!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;

			new (this) E_Stopp;
		}

		void signalStart() {
			LOG_DEBUG <<"State: MainState \n";

		}

		void signalStop() {
			LOG_DEBUG <<"State: MainState \n";

			data->cc2.signalStop();
		}

		void signalReset() {
			LOG_DEBUG <<"State: MainState \n";

			data->cc2.signalReset();
		}

		void signalLBSkidInterrupted() {
			LOG_DEBUG <<"State: MainState \n";

			data->cc2.signalLBSkidInterrupted();
		}
		void signalLBSkidNotInterrupted() {
			LOG_DEBUG <<"State: MainState \n";

			data->cc2.signalLBSkidNotInterrupted();
		}
		void signalAltimetryCompleted() {
			LOG_DEBUG <<"State: MainState \n";

		}
		void signalLBNextConveyor() {
			LOG_DEBUG <<"State: MainState \n";

			data->cc2.signalLBNextConveyor();
		}
	};

	struct E_Stopp: public TOPFSM {
		void signalReset() {
			LOG_DEBUG <<"State: E_Stopp \n";

			while (data->hb.getHardware()->getHMI()->isButtonEStopPressed()) {
			}
			data->im.removeESTOP();

			if (data->im.wurdeUeberallQuitiert()) {
				data->cm->resetSpeed(MOTOR_STOP);
				data->cm->transact();
				new (this) MainState;
			} else {
				new (this) E_Stopp;
			}
		}
	};

	struct BothSkidsFull: public TOPFSM {
		void signalReset() {
			LOG_DEBUG <<"State: BothSkidsFull \n";
			data->hb.getHardware()->getTL()->turnRedOff();
			data->hb.getHardware()->getTL()->turnGreenOn();
			data->cm->resetSpeed(MOTOR_STOP);
			data->cm->transact();
			data->cc2.sensorMeasurementCompleted();
			data->im.setBand1RutscheLeer();
			data->im.setBand2RutscheLeer();
			new (this) MainState;

		}
	};

	struct SkidOfConveyor2Full: public TOPFSM {
		virtual void signalReset() {
			LOG_DEBUG <<"State: SkidOfConveyor2Full \n";
			data->hb.getHardware()->getTL()->turnYellowOff();
			data->hb.getHardware()->getTL()->turnGreenOn();
			data->cc2.skidOfConveyor2Cleared();
			data->im.setBand2RutscheLeer();
			data->cm->resetSpeed(MOTOR_STOP);
			data->cm->transact();
		}
	};

	MainState stateMember;  //The memory for the state is part of context object
	TOPData2 contextdata;  //Data is also kept inside the context object

public:

	/**
	 *  Constructor des Contexts.
	 */
	ContextTopFSM2(int, std::vector<Puck>*, int*);

	/**
	 *  Destructor des Contexts.
	 */
	virtual ~ContextTopFSM2();

	/**
	 *
	 *return: gibt true zurück wenn der Context den Enzustand erreicht hat und false wenn Context noch nicht in einem Enzustand ist.
	 */
	bool isContextimEnzustand() {
		return contextdata.cc2.isContextimEnzustand();
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

	void signalLBNextConveyor();

};

#endif /* CONTEXTTOPFSM1_H_ */
