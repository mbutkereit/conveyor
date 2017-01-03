/*
 * ContextConveyor3.h
 *
 *  Created on: 01.12.2016
 *      Author: abq808
 */

#ifndef CONTEXTCONVEYOR3_H_
#define CONTEXTCONVEYOR3_H_
#define DELTA_CT1_CTH1 0
#define DELTA_CT2_CTH2 0
#define DELTA_CT3_CTH3 0
#define DELTA_CTH1_CTE1 0
#define DELTA_CTH2_CTE2 0
#define DELTA_CTH3_CTE3 0
//#define DELTA_TW_TE 0
#include "MotorOptions.h"
#include "ContextMotor.h"
#include "ContextSwitch.h"
#include "ContextTimer.h"
#include "Hal/HalBuilder.h"
#include "Puck.h"
#include <vector>
#include <iostream>
#include "ContextI.h"
#include "Serializer/InfoMessage.h"
#include "Serializer/Serializer.h"
#include "Serializer/WorkpieceMessage.h"
#include "Thread/BlinkRedThread.h"
#include "Thread/BlinkYellowThread.h"
#include "Logger/Logger.h"
#include "ContextTimeout.h"
#include "ContextAltimetryFailManagement.h"
#include "ContextConveyorEndFailManagement.h"

struct Data3 {
	Data3(int puckID, std::vector<Puck>* puckVector) :
			cswitch(ContextSwitch::getInstance()), cto1(), cto2(), cto3(), hb(), cm(ContextMotor::getInstance()), puck(-1), puck2(-1), puck3(-1), id(0),
			height(-1), delta1(0), delta2(0), delta3(0), finished(false), puckVector(puckVector), wpm(), im(InfoMessage::getInfoMessage()), blinkRed(), blinkYellow(), cafm(&cto1, &cto2, &cto3), ccefm(&cto1, &cto2, &cto3) {
	}

	ContextSwitch* cswitch;
	ContextTimeout cto1;
	ContextTimeout cto2;
	ContextTimeout cto3;
	HalBuilder hb;
	ContextMotor* cm;
	Puck puck;
	Puck puck2;
	Puck puck3;
	int id;
	int height;
	int delta1;
	int delta2;
	int delta3;
	bool finished;
	std::vector<Puck>* puckVector;
	WorkpieceMessage wpm;
	InfoMessage* im;
	BlinkRedThread blinkRed;
	BlinkYellowThread blinkYellow;
	ContextAltimetryFailManagement cafm;
	ContextConveyorEndFailManagement ccefm;
};

class ContextConveyor3 {
public:
	ContextConveyor3(int puckID, std::vector<Puck>* puckVector) :
			statePtr(&stateMember), // assigning start state
			contextdata(puckID, puckVector) // initializing data
	{
		statePtr->data = &contextdata; // connecting state->data with the context data
	}

	bool isContextimEnzustand() {
		return contextdata.finished;
	}

private:
	struct PucksOnConveyor3 { //top-level state
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
		virtual void signalTimerTick(){
            data->cto1.signalTimerTick();
            data->cto2.signalTimerTick();
            data->cto3.signalTimerTick();
            if(data->cto1.timeoutOccured() || data->cto2.timeoutOccured() || data->cto3.timeoutOccured() ){
            	data->hb.getHardware()->getTL()->turnGreenOff();
				data->blinkYellow.start(NULL);
				data->cm->setSpeed(MOTOR_STOP);

				cout << "TIMEOUT" << endl;
				data->puckVector->clear();
                new (this) PuckLost;
            }
		}
		Data3* data; // pointer to data, which physically resides inside the context class (contextdata)
	}*statePtr;   // a pointer to current state. Used for polymorphism.

	struct ReceivingPucks: public PucksOnConveyor3 {
		virtual void signalLBBeginInterrupted() {
			LOG_DEBUG <<"State:Recieving Pucks\n";
			data->hb.getHardware()->getTL()->turnGreenOn();
			data->cm->setSpeed(MOTOR_FAST);

			new (this) Puck1Recognized;

		}

	};

	struct Puck1Recognized: public PucksOnConveyor3 {
		virtual void signalLBBeginNotInterrupted() {
			LOG_DEBUG <<"State: Puck 1 Recognized\n";

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
			//TODO t_01
			data->cto1.startTimerT0();
			data->cm->setSpeed(MOTOR_STOP);

			new (this) Puck2Ready;
		}

	};

	struct Puck2Ready: public PucksOnConveyor3 {

		virtual void signalLBBeginInterrupted() {
			LOG_DEBUG <<"State: Puck 2 Ready\n";
			data->cm->resetSpeed(MOTOR_STOP);
			data->cm->setSpeed(MOTOR_FAST);

			new (this) Puck2Recognized;
		}

	};

	struct Puck2Recognized: public PucksOnConveyor3 {
		virtual void signalLBBeginNotInterrupted() {
			LOG_DEBUG <<"State: Puck 2 Recognized\n";
			struct workpiece_package_without_ch recieve =
					data->wpm.getWorkpieceInfo();

			switch (recieve.workpiece_type) {
			case 0:
				data->puck2.setPuckType(DRILL_HOLE_UPSIDE);
				break;

			case 1:
				data->puck2.setPuckType(DRILL_HOLE_UPSIDE_METAL);
				break;

			case 2:
				data->puck2.setPuckType(DRILL_HOLE_UPSIDE_PLASTIC);
				break;

			case 3:
				data->puck2.setPuckType(NO_DRILL_HOLE);
				break;

			case 4:
			default:
				data->puck2.setPuckType(TYPE404PT);
				break;

			}
			data->puck2.setId(recieve.id);
			data->puck2.setHeightReading1(recieve.alimetry_value_one);
			data->puck2.setHeightReading2(recieve.alimetry_value_two);
			data->puckVector->push_back(data->puck2);
			//TODO t_02
			data->cto2.startTimerT0();
			data->cm->setSpeed(MOTOR_STOP);

			new (this) Puck3Ready;
		}

	};

	struct Puck3Ready: public PucksOnConveyor3 {
		virtual void signalLBBeginInterrupted() {
			LOG_DEBUG <<"State: Puck 3 Ready\n";
			data->cm->resetSpeed(MOTOR_STOP);

			data->cm->setSpeed(MOTOR_FAST);

			new (this) Puck3Recognized;
		}

	};

	struct Puck3Recognized: public PucksOnConveyor3 {
		virtual void signalLBBeginNotInterrupted() {
			LOG_DEBUG <<"State: Puck 3 Recognized\n";
			struct workpiece_package_without_ch recieve =
					data->wpm.getWorkpieceInfo();

			switch (recieve.workpiece_type) {
			case 0:
				data->puck3.setPuckType(DRILL_HOLE_UPSIDE);
				break;

			case 1:
				data->puck3.setPuckType(DRILL_HOLE_UPSIDE_METAL);
				break;

			case 2:
				data->puck3.setPuckType(DRILL_HOLE_UPSIDE_PLASTIC);
				break;

			case 3:
				data->puck3.setPuckType(NO_DRILL_HOLE);
				break;

			case 4:
			default:
				data->puck3.setPuckType(TYPE404PT);
				break;

			}
			data->puck3.setId(recieve.id);
			data->puck3.setHeightReading1(recieve.alimetry_value_one);
			data->puck3.setHeightReading2(recieve.alimetry_value_two);
			data->puckVector->push_back(data->puck3);
			//TODO t_03
			data->cto3.startTimerT0();
			data->cm->resetSpeed(MOTOR_STOP);

			data->cm->setSpeed(MOTOR_FAST);//Wirklich nötig? Vorheriger Zustand setzt bereits Fast. Bitte auch ohne testen./MC

			new (this) EndReceiving;

		}

	};

	struct EndReceiving: public PucksOnConveyor3 {
		virtual void signalLBAltimetryInterrupted() {
			LOG_DEBUG <<"State: EndRecieving\n";
			data->cto1.stopTimerT0();
			//TODO t_H1
			data->cto1.startTimerTH();
			new (this) AltimetryAndConveyorEndFailManagement_TOP;
		}

	};

	struct AltimetryAndConveyorEndFailManagement_TOP: public PucksOnConveyor3{

		//ConveyorAltimetryFailManagment - Altimetry not interrupted transaction
	    virtual void signalLBAltimetryNotInterrupted(){
	        data->cafm.signalLBAltimetryNotInterrupted();
	        if(data->cafm.puckAdded()){
                data->cm->setSpeed(MOTOR_STOP);

                data->hb.getHardware()->getTL()->turnGreenOff();
                data->blinkRed.start(NULL);
                new (this) PuckAdded;
	        }
	    }

	    //ConveyorAltimetryFailManagment - Altimetry interrupted transaction
	    virtual void signalLBAltimetryInterrupted(){
	        data->cafm.signalLBAltimetryInterrupted();
	    }

	    virtual void signalLBSwitchInterrupted(){
	        data->cswitch->setSwitchOpen();
	    }

	    //ConveyorEndFailManagement - End interrupted transaction
	    virtual void signalLBEndInterrupted(){
	        data->cswitch->resetSwitchOpen();
	        data->cto1.stopTimerTH();
	        data->ccefm.signalLBEndInterrupted();
	    }

	    //ConveyorEndFailManagement - End not interrupted transaction
	    virtual void signalLBEndNotInterrupted(){
	        data->ccefm.signalLBEndNotInterrupted();
	        if(data->ccefm.puckAdded()){
	            data->cm->setSpeed(MOTOR_STOP);

	            data->hb.getHardware()->getTL()->turnGreenOff();
	            data->blinkRed.start(NULL);
	            new (this) PuckAdded;
	        }
	        else if(data->ccefm.isContextimEnzustand()){
                //Puck 1
                cout << "ID Puck 1: " <<

                cout << data->puck.getId() << endl;
                cout << "Height on Conveyor1 Puck 1: " <<

                cout << data->puck.getHeightReading1() << endl;
                cout << "Height on Conveyor2 Puck 1: " <<

                cout << data->puck.getHeightReading2() << endl;
                cout << "Puck Type Puck 1: " <<

                cout << data->puck.getPuckType() << endl;

                //Puck 2
                cout << "ID Puck 2: " <<

                cout << data->puck2.getId() << endl;
                cout << "Height on Conveyor1 Puck 2: " <<

                cout << data->puck2.getHeightReading1() << endl;
                cout << "Height on Conveyor2 Puck 2: " <<

                cout << data->puck2.getHeightReading2() << endl;
                cout << "Puck Type Puck 2: " <<

                cout << data->puck2.getPuckType() << endl;

                //Puck 3
                cout << "ID Puck 3: " <<

                cout << data->puck3.getId() << endl;
                cout << "Height on Conveyor1 Puck 3: " <<

                cout << data->puck3.getHeightReading1() << endl;
                cout << "Height on Conveyor2 Puck 3: " <<

                cout << data->puck3.getHeightReading2() << endl;
                cout << "Puck Type Puck 3: " <<

                cout << data->puck3.getPuckType() << endl;

                new (this) EndOfTheEnd;
	        }
	    }
	};

	struct PuckAdded: public PucksOnConveyor3 {
		virtual void SignalReset() {
			data->cm->resetSpeed(MOTOR_STOP);

			data->blinkRed.stop();
			data->finished = true;
			new (this) EndOfTheEnd;
		}

	};

	struct PuckLost: public PucksOnConveyor3 { //TODO: ErrorMessage ausgben!!!!!!!!!!!!!
	    virtual void signalReset(){
	        data->blinkYellow.stop();
	        data->hb.getHardware()->getTL()->turnYellowOff();
	        data->hb.getHardware()->getTL()->turnGreenOn();
	        data->cm->resetSpeed(MOTOR_STOP);

	        data->finished = true;
	        new (this) EndOfTheEnd;
	    }

	};

	struct EndOfTheEnd: public PucksOnConveyor3 {
	    virtual void signalLBBeginInterrupted() {
            data->cm->resetSpeed(MOTOR_STOP);

            data->finished = true;
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
        virtual void signalTimerTick(){
        }
	};

	ReceivingPucks stateMember; //The memory for the state is part of context object
	Data3 contextdata;  //Data is also kept inside the context object

public:

	void deltaToLow() {

	}
	;
	void signalLBBeginInterrupted() {
		statePtr->signalLBBeginInterrupted();
	}
	void signalLBEndInterrupted() {
		statePtr->signalLBEndInterrupted();
	}
	void signalLBAltimetryInterrupted() {
		statePtr->signalLBAltimetryInterrupted();
	}
	void signalLBSwitchInterrupted() {
		statePtr->signalLBSwitchInterrupted();
	}
	void signalLBBeginNotInterrupted() {
		statePtr->signalLBBeginNotInterrupted();
	}
	void signalLBEndNotInterrupted() {
		statePtr->signalLBEndInNotInterrupted();
	}
	void signalLBAltimetryNotInterrupted() {
		statePtr->signalLBAltimetryNotInterrupted();
	}
	void signalLBSwitchNotInterrupted() {
		statePtr->signalLBSwitchNotInterrupted();
	}
	void signalEStop() {
		statePtr->signalEStop();
	}
	void signalStart() {
		statePtr->signalStart();
	}
	void signalStop() {
		statePtr->signalStop();
	}
	void signalReset() {
		statePtr->signalReset();
	}
	void signalLBSkidInterrupted() {
		statePtr->signalLBSkidInterrupted();
	}
	void signalLBSkidNotInterrupted() {
		statePtr->signalLBSkidNotInterrupted();
	}
	void signalAltimetryCompleted() {
		statePtr->signalAltimetryCompleted();
	}
	void signalTimerTick(){
	    statePtr->signalTimerTick();
	}
};

#endif /* CONTEXTCONVEYOR3_H_ */
