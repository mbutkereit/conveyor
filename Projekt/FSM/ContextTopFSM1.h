/*
 * ContextTopFSM.h
 *
 *  Created on: 07.12.2016
 *      Author: abs949
 */

#ifndef CONTEXTTOPFSM1_H_
#define CONTEXTTOPFSM1_H_

#include <iostream>
#include <ContextConveyor1.h>
#include "Logger/Logger.h"
#include "Hal/HalBuilder.h"
#include <vector>
#include <Puck.h>
#include "ContextMotor.h"
#include "Serializer/Serializer.h"
#include "Serializer/InfoMessage.h"
#include "Serializer/WorkpieceMessage.h"

extern HalBuilder hb; ///< Der HalBuilder um sicher und zentral auf die Hardware zuzugreifen.

struct TOPData {
	TOPData(int puckID, std::vector<Puck>* puckVector, int *skidcounter) :
			cc1(puckID, puckVector, skidcounter), cm(
					ContextMotor::getInstance()), hb(), im(){
	}
	ContextConveyor1 cc1;
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
class ContextTopFSM1 :public ContextI {
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
		virtual void signalTimerTick(){

		}

		TOPData* data; // pointer to data, which physically resides inside the context class (contextdata)
	}*statePtr;   // a pointer to current state. Used for polymorphism.

	struct MainState: public TOPFSM {
		virtual void signalLBBeginInterrupted() {

			data->cc1.signalLBBeginInterrupted();
		}
		virtual void signalLBBeginNotInterrupted() {

			data->cc1.signalLBBeginNotInterrupted();
		}
		virtual void signalLBEndInterrupted() {

			data->cc1.signalLBEndInterrupted();
		}
		virtual void signalLBEndNotInterrupted() {

			data->cc1.signalLBEndNotInterrupted();
		}
		virtual void signalLBAltimetryInterrupted() {

			data->cc1.signalLBAltimetryInterrupted();
		}
		virtual void signalLBAltimetryNotInterrupted() {

			data->cc1.signalLBAltimetryNotInterrupted();
		}

		virtual void signalLBSwitchInterrupted() {

			data->cc1.signalLBSwitchInterrupted();
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
				data->cc1.sensorMeasurementCompleted();
			}
		}
		virtual void signalLBSwitchNotInterrupted() { //ACTUALLY NOT EXECUTABLE BECAUSE OF signalLBSwitchInterrupted()

			data->cc1.signalLBSwitchNotInterrupted();
		}
		virtual void signalEStop() {

			data->cm->setSpeed(MOTOR_STOP);
			data->cm->transact();
			if (data->hb.getHardware()->getHMI()->isButtonEStopPressed()) {
				data->im.setESTOP();
			}
			LOG_DEBUG <<"E-STOP WURDE GEDRUECKT \n";
			cout<<"!!!!!!!!!!! E-STOP WURDE GEDRUECKT!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;


			new (this) E_Stopp;
		}

		virtual void signalStart() {

		}

		virtual void signalStop() {

			data->cc1.signalStop();
		}

		virtual void signalReset() {

			data->cc1.signalReset();
		}

		virtual void signalLBSkidInterrupted() {

			data->cc1.signalLBSkidInterrupted();
		}
		virtual void signalLBSkidNotInterrupted() {

			data->cc1.signalLBSkidNotInterrupted();
		}
		virtual void signalAltimetryCompleted() {

		}
		virtual void signalLBNextConveyor() {

			data->cc1.signalLBNextConveyor();
		}
		virtual void signalTimerTick(){
		    data->cc1.signalTimerTick();
		}
	};

	struct E_Stopp: public TOPFSM {
		virtual void signalReset() {
			LOG_DEBUG <<"State: E-Stopp \n";


			while (data->hb.getHardware()->getHMI()->isButtonEStopPressed()) {
			}
			data->im.removeESTOP();

			if (data->im.wurdeUeberallQuitiert()) {
				data->cm->resetSpeed(MOTOR_STOP);
				data->cm->transact();
				new (this) MainState;
			}

			else {
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
			data->cc1.sensorMeasurementCompleted();
			data->im.setBand1RutscheLeer();
			data->im.setBand2RutscheLeer();
			new (this) MainState;
		}
	};

	MainState stateMember;  //The memory for the state is part of context object
	TOPData contextdata;  //Data is also kept inside the context object

public:

	/**
	 *  Constructor des Contexts.
	 */
	ContextTopFSM1(int, std::vector<Puck>*, int*);

	/**
	 *  Destructor des Contexts.
	 */
	virtual ~ContextTopFSM1();

	/**
	 *
	 *return: gibt true zurück wenn der Context den Enzustand erreicht hat und false wenn Context noch nicht in einem Enzustand ist.
	 */
	bool isContextimEnzustand() {
		return contextdata.cc1.isContextimEnzustand();
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

	void signalTimerTick();

};

#endif /* CONTEXTTOPFSM1_H_ */
