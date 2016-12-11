/*
 * ContextTopFSM.h
 *
 *  Created on: 07.12.2016
 *      Author: abs949
 */

#ifndef CONTEXTTOPFSM1_H_
#define CONTEXTTOPFSM1_H_


#include <iostream>
#include <ContextConveyor2.h>
#include "Logger/Logger.h"
#include "Hal/HalBuilder.h"
#include <vector>
#include <Puck.h>
#include "ContextMotor.h"

extern HalBuilder hb; ///< Der HalBuilder um sicher und zentral auf die Hardware zuzugreifen.

struct TOPData {
	TOPData(int puckID, std::vector<Puck>* puckVector) : cc2(puckID, puckVector), cm(ContextMotor::getInstance()), hb() {
	}
	ContextConveyor2 cc2;
	ContextMotor *cm;
	HalBuilder hb;
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
class ContextTopFSM2 {
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
			data->cc2.signalLBBeginInterrupted();
		}
		void signalLBBeginNotInterrupted() {
			data->cc2.signalLBBeginNotInterrupted();
		}
		void signalLBEndInterrupted() {
			data->cc2.signalLBEndInterrupted();
		}
		void signalLBEndNotInterrupted() {
			data->cc2.signalLBEndNotInterrupted();
		}
		void signalLBAltimetryInterrupted() {
			data->cc2.signalLBAltimetryInterrupted();
		}
		void signalLBAltimetryNotInterrupted() {
			data->cc2.signalLBAltimetryNotInterrupted();
		}

		void signalLBSwitchInterrupted() {
            data->cc2.signalLBSwitchInterrupted();
            if (0){   //TODO BOTH SKIDS FULL AND ERRORMESSAGE
                data->hb.getHardware()->getTL()->turnGreenOff();
                data->hb.getHardware()->getTL()->turnRedOn();
                data->cm->setSpeed(MOTOR_STOP);
                data->cm->transact();
                new (this) BothSkidsFull;
            }
            else{
                data->cc2.sensorMeasurementCompleted();
                if(data->cc2.skidOfConveyor2Full()){
                    new (this) SkidOfConveyor2Full;
                }
            }
		}
		void signalLBSwitchNotInterrupted() {//ACTUALLY NOT EXECUTABLE BECAUSE OF signalLBSwitchInterrupted()
			data->cc2.signalLBSwitchNotInterrupted();
		}
		void signalEStop() {
		    data->cm->setSpeed(MOTOR_STOP);
		    data->cm->transact();
		    new (this) E_Stopp;
		}

		void signalStart() {
		}

		void signalStop() {
			data->cc2.signalStop();
		}

		void signalReset() {
			data->cc2.signalReset();
		}

		void signalLBSkidInterrupted() {
			data->cc2.signalLBSkidInterrupted();
		}
		void signalLBSkidNotInterrupted() {
			data->cc2.signalLBSkidNotInterrupted();
		}
		void signalAltimetryCompleted() {
		}
		void signalLBNextConveyor() {
			data->cc2.signalLBNextConveyor();
		}
	};

    struct E_Stopp: public TOPFSM{
        void signalReset(){//TODO ALL CONVEYOR UNLOCK MISSING
        	while(data->hb.getHardware()->getHMI()->isButtonEStopPressed()){}
            //TODO UNLOCK CHECK FOR OTHER CONVEYOR
        	data->cm->resetSpeed(MOTOR_STOP);
        	data->cm->transact();
        	new (this) MainState;
        }
    };

    struct BothSkidsFull: public TOPFSM{
        void signalReset(){
            data->hb.getHardware()->getTL()->turnRedOff();
            data->hb.getHardware()->getTL()->turnGreenOn();
            data->cm->resetSpeed(MOTOR_STOP);
            data->cm->transact();
            data->cc2.sensorMeasurementCompleted();
            if(data->cc2.skidOfConveyor2Full()){
                new (this) SkidOfConveyor2Full;
            }else{
                new (this) MainState;
            }
        }
    };


    struct SkidOfConveyor2Full: public TOPFSM{
        virtual void signalReset(){
            data->hb.getHardware()->getTL()->turnYellowOff();
            data->hb.getHardware()->getTL()->turnGreenOn();
            data->cc2.skidOfConveyor2Cleared();
            data->cm->resetSpeed(MOTOR_STOP);
            data->cm->transact();
        }
    };

	MainState stateMember;   //The memory for the state is part of context object
	TOPData contextdata;  //Data is also kept inside the context object

public:

	/**
	 *  Constructor des Contexts.
	 */
	ContextTopFSM2(int, std::vector<Puck>*);

	/**
	 *  Destructor des Contexts.
	 */
	virtual ~ContextTopFSM2();

	/**
	*
	*return: gibt true zurück wenn der Context den Enzustand erreicht hat und false wenn Context noch nicht in einem Enzustand ist.
	*/
	bool isContextimEnzustand(){return contextdata.cc2.isContextimEnzustand();}

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
