/*
 * ContextConveyor1.h
 *
 *  Created on: 02.12.2016
 *      Author: abx827
 */

#ifndef CONTEXTCONVEYOR1_H_
#define CONTEXTCONVEYOR1_H_

#include <iostream>
#include "Logger/Logger.h"
#include "Hal/HalBuilder.h"
#include "ContextMotor.h"
#include "Puck.h"
#include "vector"

extern HalBuilder hb; ///< Der HalBuilder um sicher und zentral auf die Hardware zuzugreifen.

struct Data {
	Data(int puckID, std::vector<Puck>* puckVector) :
			puckID(puckID), hb(), cm(ContextMotor::getInstance()), puck(), puckVector(puckVector) {
	}
	int puckID;
	HalBuilder hb;
	ContextMotor* cm;
	Puck puck;
	std::vector<Puck>* puckVector;
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

	Data* data; // pointer to data, which physically resides inside the context class (contextdata)
	}*statePtr;   // a pointer to current state. Used for polymorphism.

	struct TransportToEntry: public PuckOnConveyor1 {
		//ENTRY
		void signalLBBeginNotInterrupted() {
			data->hb.getHardware()->getTL()->turnGreenOn();
		}

		//TRANSACTION
		void signalLBBeginInterrupted() {
					new (this) MotorOn;
		}
	};

	struct MotorOn: public PuckOnConveyor1{
		//ENTRY
		void signalLBBeginInterrupted() {
		    data->cm->setSpeed(MOTOR_FAST);
		    data->cm->transact();
		    data->puck.setId(data->puckID);
		}
	};

	TransportToEntry stateMember;   //The memory for the state is part of context object
	Data contextdata;  //Data is also kept inside the context object

public:

	/**
	 *  Constructor des Contexts.
	 */
	ContextConveyor1(int, std::vector*);

	/**
	 *  Destructor des Contexts.
	 */
	virtual ~ContextConveyor1();

	/**
	*
	*return: gibt true zurück wenn der Context den Enzustand erreicht hat und false wenn Context noch nicht in einem Enzustand ist.
	*/
	bool isContextimEnzustand();

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
};

#endif /* CONTEXTCONVEYOR1_H_ */
