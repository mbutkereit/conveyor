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

extern HalBuilder hb; ///< Der HalBuilder um sicher und zentral auf die Hardware zuzugreifen.

struct TOPData {
	TOPData(int puckID, std::vector<Puck>* puckVector) : cc1(puckID, puckVector), cm(ContextMotor::getInstance()) {
	}
	ContextConveyor1 cc1;
	ContextMotor cm;
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
class ContextTopFSM1 {
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

		TOPData* data; // pointer to data, which physically resides inside the context class (contextdata)
	}*statePtr;   // a pointer to current state. Used for polymorphism.

	struct MainState: public TOPFSM {
		void signalLBBeginInterrupted() {
			data->cc1.signalLBBeginInterrupted();
		}
		void signalLBBeginNotInterrupted() {
			data->cc1.signalLBBeginNotInterrupted();
		}
		void signalLBEndInterrupted() {
			data->cc1.signalLBEndInterrupted();
		}
		void signalLBEndNotInterrupted() {
			data->cc1.signalLBEndNotInterrupted();
		}
		void signalLBAltimetryInterrupted() {
			data->cc1.signalLBAltimetryInterrupted();
		}
		void signalLBAltimetryNotInterrupted() {
			data->cc1.signalLBAltimetryNotInterrupted();
		}

		void signalLBSwitchInterrupted() {
			data->cc1.signalLBSwitchInterrupted();
		}
		void signalLBSwitchNotInterrupted() {
			data->cc1.signalLBSwitchNotInterrupted();
		}
		void signalEStop() {
		    data->cm->setSpeed(MOTOR_STOP);
		    data->cm->transact();
		    new (this) E_Stopp;
		}

		void signalStart() {
		}

		void signalStop() {
			data->cc1.signalStop();
		}

		void signalReset() {
			data->cc1.signalReset();
		}

		void signalLBSkidInterrupted() {
			data->cc1.signalLBSkidInterrupted();
		}
		void signalLBSkidNotInterrupted() {
			data->cc1.signalLBSkidNotInterrupted();
		}
		void signalAltimetryCompleted() {
		}
		void signalLBNextConveyor() {
			data->cc1.signalLBNextConveyor();
		}
	};

    struct E_Stopp: public TOPFSM{
        void signalReset(){//TODO ALL CONVEYOR UNLOCK MISSING
        	while(data->hb.getHardware()->getHMI()->isButtonEStopPressed()){}
        	data->cm->resetSpeed(MOTOR_STOP);
        	data->cm->transact();
            //TODO UNLOCK CHECK FOR OTHER CONVEYOR
        	new (this) MainState;
        }
    };

	MainState stateMember;   //The memory for the state is part of context object
	TOPData contextdata;  //Data is also kept inside the context object

public:

	/**
	 *  Constructor des Contexts.
	 */
	ContextTopFSM1(int, std::vector<Puck>*);

	/**
	 *  Destructor des Contexts.
	 */
	virtual ~ContextTopFSM1();

	/**
	*
	*return: gibt true zurück wenn der Context den Enzustand erreicht hat und false wenn Context noch nicht in einem Enzustand ist.
	*/
	bool isContextimEnzustand(){return false;}

	/**
	 * @todo Ausstehende implementierung Dokumentieren.
	 */
	void signalLBBeginInterrupted();

	/**
	 * @todo Ausstehende implementierung Dokumentieren.
	 */
	void signalLBEndInterrupted();

	/**
	 * @todo Ausstehende implementierung Dokumentieren.
	 */
	void signalLBAltimetryInterrupted();

	/**
	 * @todo Ausstehende implementierung Dokumentieren.
	 */
	void signalLBSwitchInterrupted();

	/**
	 * @todo Ausstehende implementierung Dokumentieren.
	 */
	void signalLBBeginNotInterrupted();

	/**
	 * @todo Ausstehende implementierung Dokumentieren.
	 */
	void signalLBEndNotInterrupted();

	/**
	 *  Constructor des Adapters.
	 *
	 *  @param baseaddress Die Baseaddress die verwenden werden soll.
	 */
	void signalLBAltimetryNotInterrupted();

	/**
	 * @todo Ausstehende implementierung Dokumentieren.
	 */
	void signalLBSwitchNotInterrupted();

	/**
	 * @todo Ausstehende implementierung Dokumentieren.
	 */
	void signalEStop();

	/**
	 * @todo Ausstehende implementierung Dokumentieren.
	 */
	void signalStart();

	/**
	 * @todo Ausstehende implementierung Dokumentieren.
	 */
	void signalStop();

	/**
	 * @todo Ausstehende implementierung Dokumentieren.
	 */
	void signalReset();

	/**
	 * @todo Ausstehende implementierung Dokumentieren.
	 */
	void signalLBSkidInterrupted();

	/**
	 * @todo Ausstehende implementierung Dokumentieren.
	 */
	void signalLBSkidNotInterrupted();

	/**
	 * @todo Ausstehende implementierung Dokumentieren.
	 */
	void signalAltimetryCompleted();

	/**
	 * @todo Ausstehende implementierung Dokumentieren.
	 */
	void signalLBNextConveyor();

};

#endif /* CONTEXTTOPFSM1_H_ */
