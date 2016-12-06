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
#include <vector>
#include "ContextSorting.h"
#include "ContextSwitch.h"


extern HalBuilder hb; ///< Der HalBuilder um sicher und zentral auf die Hardware zuzugreifen.

struct Data {
	Data(int puckID, std::vector<Puck>* puckVector) :
			puckID(puckID), hb(), cm(ContextMotor::getInstance()), cs(ContextSorting::getInstance()), cswitch(ContextSwitch::getInstance()), puck(puckID), puckVector(puckVector), finished(false), posInVector(0) {
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
		    data->cm->setSpeed(MOTOR_STOP);
		    data->cm->transact();
		    new (this) E_Stopp;
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

		//TODO SIGNALS THAT ARE MISSING
        virtual void signalSequenceOK(){
        }
        virtual void signalSequenceNotOK(){
        }
        virtual void signalConveyor2isFree(){
        }
        virtual void signalLBBeginOfConveyor2Interrupted(){
        }
        virtual void signalTimeout(){
        }

        //TODO DELETE IF ALL UNKNOWN ARE ELIMINATED IN STATES
        virtual void Unknown(){
        }

	Data* data; // pointer to data, which physically resides inside the context class (contextdata)
	}*statePtr;   // a pointer to current state. Used for polymorphism.

	struct TransportToEntry: public PuckOnConveyor1 {
		//TRANSACTION/LEAVE
		void signalLBBeginInterrupted() {
		    data->hb.getHardware()->getTL()->turnGreenOn();
            data->cm->setSpeed(MOTOR_FAST);
            data->cm->transact();
            new (this) MotorOn;
		}
	};

	struct MotorOn: public PuckOnConveyor1{
		//LEAVE
		void signalLBBeginNotInterrupted() {
		    //TODO t0 = GIVE TIME, START TIMER(t0)!
		    data->puckVector->push_back(data->puck);
		    data->posInVector = data->puckVector->size()-1;
		    new (this) TransportToHeightMeasurement;
		}
	};

	struct TransportToHeightMeasurement: public PuckOnConveyor1{
	    virtual void signalLBAltimetryInterrupted() {
	        data->cm->setSpeed(MOTOR_SLOW);
	        data->cm->transact();
	        if(1){//TODO DELTA t0 and tH OK
	            data->puck.setHeightReading1(data->hb.getHardware()->getAltimetry()->getHeight());
	            new (this) PuckInHeightMeasurement;
	        }
	        else if(0){//TODO DELTA t0 AND tH TOO LOW
	            data->hb.getHardware()->getTL()->turnGreenOff();
	            data->hb.getHardware()->getTL()->turnRedOn();//TODO ACTUALLY SHOULD BLINK!
	            data->cm->setSpeed(MOTOR_STOP);
	            data->cm->transact();
	            new (this) PuckAdded;
	        }
	        else if(0){//TODO BOTH SKIDS FULL AND ERRORMESSAGE
	            data->hb.getHardware()->getTL()->turnGreenOff();
	            data->hb.getHardware()->getTL()->turnRedOn();
	            data->cm->setSpeed(MOTOR_STOP);
	            data->cm->transact();
	            new (this) BothSkidsFull;
	        }
	    }
	};

	struct PuckInHeightMeasurement: public PuckOnConveyor1{
        virtual void signalLBAltimetryNotInterrupted() {
            data->cm->resetSpeed(MOTOR_SLOW);
            data->cm->transact();
            new (this) TransportToSwitch;
        }
	};

	struct TransportToSwitch: public PuckOnConveyor1{
	    virtual void signalLBSwitchInterrupted() {
	        //TODO STOP TIMER tH, GIVE TIME tW, DELTA th AND tW CALCULATION
	        if(data->hb.getHardware()->getMT()->isItemMetal()){
	            data->puck.setPuckmaterial(METAL);
	        }
	        else
	        {
	            data->puck.setPuckmaterial(PLASTIC);
	        }
	        //TODO SENSORMEASUREMENT COMPLETE?
            if (1){//TODO DELTA tH and tW OK AND BOTHSKIDS NOT FULL
                new (this) Sorting;
            } else if (0){//TODO DELTA tH AND tW TOO LOW
                data->hb.getHardware()->getTL()->turnGreenOff();
                data->hb.getHardware()->getTL()->turnRedOn(); //TODO ACTUALLY SHOULD BLINK!
                data->cm->setSpeed(MOTOR_STOP);
                data->cm->transact();
                new (this) PuckAdded;
            } else if (0){//TODO BOTH SKIDS FULL AND ERRORMESSAGE
                data->hb.getHardware()->getTL()->turnGreenOff();
                data->hb.getHardware()->getTL()->turnRedOn();
                data->cm->setSpeed(MOTOR_STOP);
                data->cm->transact();
                new (this) BothSkidsFull;
            }
	    }
	};

	struct Sorting: public PuckOnConveyor1{
        virtual void sequenceOk(){//TODO NEED TO BE MODIFIED TO FIT IN CURRENT DESIGN WITH DISPATCHER
            data->cs->setCurrentPh(data->puck.getPuckdrillhole());
            data->cs->setCurrentPm(data->puck.getPuckmaterial());
            data->cs->transact();
            if (data->cs->getSequenceOk()){
                data->cswitch->setSwitchOpen();
                data->cswitch->transact();
                new (this) TransportToDelivery;
            } else{
                if (data->hb.getHardware()->getMT()->isSkidFull()){
                    new (this) SortOutThroughSkid;
                } else{
                    data->hb.getHardware()->getTL()->turnYellowOn();
                    data->cswitch->setSwitchOpen();
                    data->cswitch->transact();
                    new (this) TransportToDelivery;
                }
            }
        }
	};

	struct TransportToDelivery: public PuckOnConveyor1{
	    virtual void signalLBEndInterrupted() {
	        //TODO STOP TIME(tW), tE = GIVE TIME, CALCULATE tW AND tE
	        data->cswitch->resetSwitchOpen();
	        data->cswitch->transact();
            if (1){//TODO DELTA tW and tE OK
                data->cm->setSpeed(MOTOR_STOP);
                data->cm->transact();
                new (this) DeliverToConveyor2;
            } else if (0){//TODO DELTA tW AND tE TOO LOW
                data->hb.getHardware()->getTL()->turnGreenOff();
                data->hb.getHardware()->getTL()->turnRedOn(); //TODO ACTUALLY SHOULD BLINK!
                data->cm->setSpeed(MOTOR_STOP);
                data->cm->transact();
                new (this) PuckAdded;
            }
	    }
	};

    struct DeliverToConveyor2: public PuckOnConveyor1{
            virtual void signalConveyor2isFree(){
                data->cm->resetSpeed(MOTOR_STOP);
                data->cm->transact();
                new (this) TransportToConveyor2;
            }
    };

    struct TransportToConveyor2: public PuckOnConveyor1{
        virtual void signalLBBeginOfConveyor2Interrupted(){
            //TODO SEND PUCKINFORMATION
        	data->puckVector->erase(data->puckVector->begin()+data->posInVector);
            if (data->puckVector->size() > 0){
                data->finished = true;
            } else{
                data->cm->setSpeed(MOTOR_STOP);
                data->cm->transact();
                new (this) Conveyor1Empty;
            }
        }
    };

	struct SortOutThroughSkid: public PuckOnConveyor1{
	        virtual void signalLBSkidInterrupted(){
	            if(data->puckVector->size()>0){
	            	data->finished = true;
	            }
	            else{
	                data->cm->setSpeed(MOTOR_STOP);
	                data->cm->transact();
	                new (this) Conveyor1Empty;
	            }
	        }
	};

	struct Conveyor1Empty: public PuckOnConveyor1{
	    virtual void Unknown(){
	        data->cm->resetSpeed(MOTOR_STOP);
	        data->cm->transact();
	        data->finished = true;
	    }
	};

	struct PuckAdded: public PuckOnConveyor1{
	    virtual void signalReset(){
	        data->hb.getHardware()->getTL()->turnRedOff();
	        data->finished = true;
	    }
	};

	struct BothSkidsFull: public PuckOnConveyor1{
	    void signalReset(){
	        data->hb.getHardware()->getTL()->turnRedOff();
	        data->hb.getHardware()->getTL()->turnGreenOn();
	        //TODO new (this) history
	    }
	};

    struct E_Stopp: public PuckOnConveyor1{
        void signalReset(){
            //TODO UNLOCK CHECK FOR OTHER CONVEYOR
            //TODO new(this) HISTORY
        }
    };

	TransportToEntry stateMember;   //The memory for the state is part of context object
	Data contextdata;  //Data is also kept inside the context object

public:

	/**
	 *  Constructor des Contexts.
	 */
	ContextConveyor1(int,std::vector<Puck>*);

	/**
	 *  Destructor des Contexts.
	 */
	virtual ~ContextConveyor1();

	/**
	*
	*return: gibt true zurück wenn der Context den Enzustand erreicht hat und false wenn Context noch nicht in einem Enzustand ist.
	*/
	bool isContextimEnzustand(){
		return contextdata.finished;
	}

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
