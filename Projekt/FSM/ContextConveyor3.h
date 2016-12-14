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
#define DELTA_TW_TE 0
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


struct Data3 {
	Data3(int puckID, std::vector<Puck>* puckVector) :

			cswitch(ContextSwitch::getInstance()), ct1(
					ContextTimer::getInstance()), ct2(
					ContextTimer::getInstance()), ct3(
					ContextTimer::getInstance()), cth1(
					ContextTimer::getInstance()), cth2(
					ContextTimer::getInstance()), cth3(
					ContextTimer::getInstance()), cte1(
					ContextTimer::getInstance()), cte2(
					ContextTimer::getInstance()), cte3(
					ContextTimer::getInstance()), hb(), cm(
					ContextMotor::getInstance()), puck(-1), puck2(-1), puck3(
					-1), currState(0), id(0), height(-1), delta1(0), delta2(0), delta3(
					0), counterHeightFail(0), counterEndFail(0), finished(
					false), puckVector(puckVector), wpm(), im(), blinkRed(), blinkYellow() {
	}

	ContextSwitch* cswitch;
	ContextTimer* ct1;
	ContextTimer*ct2;
	ContextTimer* ct3;
	ContextTimer* cth1;
	ContextTimer*cth2;
	ContextTimer* cth3;
	ContextTimer* cte1;
	ContextTimer*cte2;
	ContextTimer* cte3;
	HalBuilder hb;
	ContextMotor* cm;
	Puck puck;
	Puck puck2;
	Puck puck3;
	int currState;
	int id;
	int height;
	int delta1;
	int delta2;
	int delta3;
	int counterHeightFail;
	int counterEndFail;
	bool finished;
	std::vector<Puck>* puckVector;
	WorkpieceMessage wpm;
	InfoMessage im;
	BlinkRedThread blinkRed;
	BlinkYellowThread blinkYellow;
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
	struct State { //top-level state
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
			/*data->cm->resetSpeed(MOTOR_STOP);
			 data->cm->transact();
			 new (this) Estop;*/
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
		Data3* data; // pointer to data, which physically resides inside the context class (contextdata)
	}*statePtr;   // a pointer to current state. Used for polymorphism.

	struct ReceivingPucks: public State {
		virtual void signalLBBeginInterrupted() {
			LOG_DEBUG <<"State:Recieving Pucks\n";
			data->hb.getHardware()->getTL()->turnGreenOn();
			data->cm->setSpeed(MOTOR_SLOW);
			data->cm->transact();
			data->ct1->giveTime();
			new (this) Puck1Recognized;

		}

	};

	struct Puck1Recognized: public State {
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
			//TODO: Timer Stopp
			data->puckVector->push_back(data->puck);
			data->cm->setSpeed(MOTOR_STOP);
			data->cm->transact();
			new (this) Puck2Ready;
		}

	};

	struct Puck2Ready: public State {

		virtual void signalLBBeginInterrupted() {
			LOG_DEBUG <<"State: Puck 2 Ready\n";
			data->cm->resetSpeed(MOTOR_STOP);
			data->cm->setSpeed(MOTOR_SLOW);
			data->cm->transact();
			data->ct2->giveTime();
			new (this) Puck2Recognized;
		}

	};

	struct Puck2Recognized: public State {
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

			data->cm->setSpeed(MOTOR_STOP);
			data->cm->transact();
			new (this) Puck3Ready;
		}

	};

	struct Puck3Ready: public State {
		virtual void signalLBBeginInterrupted() {
			LOG_DEBUG <<"State: Puck 3 Ready\n";
			data->cm->resetSpeed(MOTOR_STOP);
			data->cm->setSpeed(MOTOR_SLOW);
			data->cm->transact();
			data->ct3->giveTime();
			new (this) Puck3Recognized;
		}

	};

	struct Puck3Recognized: public State {
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
			data->cm->resetSpeed(MOTOR_SLOW);
			data->cm->setSpeed(MOTOR_FAST);

			new (this) EndReceiving;
		}

	};

	struct EndReceiving: public State {
		virtual void ssignalLBAltimetryInterrupted() {
			LOG_DEBUG <<"State: EndRecieving\n";
			new (this) HeightFailBegin;
		}

	};

	struct HeightFailBegin: public State {
		virtual void ssignalLBAltimetryInterrupted() {
			LOG_DEBUG <<"State: HeigntFailBegin\n";
			switch (data->counterHeightFail) {
			case 0:
				data->cth1->giveTime();

				new (this) HeightPuck1Recognized;
				break;
			case 1:
				new (this) HeightPuck2Ready;
				break;
			case 2:
				new (this) HeightPuck3Ready;
				break;
			}
			new (this) HeightPuck1Recognized;
		}

	};

	struct HeightPuck1Recognized: public State {
		virtual void ssignalLBAltimetryNotInterrupted() {
			LOG_DEBUG <<"State: HeightPuck1Recognized\n";
			data->counterHeightFail = 1;
			data->delta1 = data->cth1 - data->ct1;
			if (data->delta1 != DELTA_CT1_CTH1) {
				data->hb.getHardware()->getTL()->turnGreenOff();
				data->blinkRed.start(NULL);
				data->cm->resetSpeed(MOTOR_STOP);
				data->cm->transact();
				cout
						<< "FEHLER!!!!!!!!!!! PUCK WURDE HINZUGEFÜGT!!!!!!!!!!!!!!!!!!!!!!!!"
						<< endl;
				LOG_DEBUG <<"Fehler: PUCK WURDE HINZUGEFÜGT\n";

				new (this) PuckAdded;
			} else {
				new (this) HeightPuck2Ready;
			}

		}

	};

	struct HeightPuck2Ready: public State {
		virtual void ssignalLBAltimetryInterrupted() {
			LOG_DEBUG <<"State: HeightPuck2Ready\n";
			data->cth2->giveTime();
			new (this) HeightPuck2Recognized;
		}

	};

	struct HeightPuck2Recognized: public State {
		virtual void ssignalLBAltimetryNotInterrupted() {
			LOG_DEBUG <<"State: HeightPuck2Recognized\n";
			data->counterHeightFail = 2;
			data->delta2 = data->cth2 - data->ct2;
			if (data->delta2 != DELTA_CT2_CTH2) {
				data->hb.getHardware()->getTL()->turnGreenOff();
				data->blinkRed.start(NULL);
				data->cm->setSpeed(MOTOR_STOP);
				data->cm->transact();
				cout
						<< "FEHLER!!!!!!!!!!! PUCK WURDE HINZUGEFÜGT!!!!!!!!!!!!!!!!!!!!!!!!"
						<< endl;
				LOG_DEBUG <<"Fehler: PUCK WURDE HINZUGEFÜGT\n";

				new (this) PuckAdded;
			} else {
				new (this) HeightPuck3Ready;
			}

		}

	};

	struct HeightPuck3Ready: public State {

		virtual void ssignalLBAltimetryInterrupted() {
			LOG_DEBUG <<"State: HeightPuck3Ready\n";
			data->cth3->giveTime();
			new (this) HeightPuck3Recognized;
		}

	};

	struct HeightPuck3Recognized: public State {
		virtual void ssignalLBAltimetryNotInterrupted() {
			LOG_DEBUG <<"State: HeightPuck3Recognized\n";
			data->counterHeightFail = 0;
			data->delta3 = data->cth3 - data->ct3;
			if (data->delta3 != DELTA_CT3_CTH3) {
				data->hb.getHardware()->getTL()->turnGreenOff();
				data->blinkRed.start(NULL);
				data->cm->setSpeed(MOTOR_STOP);
				data->cm->transact();
				cout
						<< "FEHLER!!!!!!!!!!! PUCK WURDE HINZUGEFÜGT!!!!!!!!!!!!!!!!!!!!!!!!"
						<< endl;
				LOG_DEBUG <<"Fehler: PUCK WURDE HINZUGEFÜGT\n";

				new (this) PuckAdded;
			} else {
				new (this) HeightEnd;
			}

		}

	};

	struct HeightEnd: public State {
		virtual void signalLBSwitchInterrupted() {
			LOG_DEBUG <<"State: HeightEnd\n";
			data->cswitch->setSwitchOpen();
			//TODO: Timeout Weiche
			new (this) TransportToSwitch;
		}

	};

	struct TransportToSwitch: public State {
		virtual void signalLBEndInterrupted() {
			LOG_DEBUG <<"State: TransportToSwitch\n";
			new (this) EndFailBegin;

		}

	};

	struct EndFailBegin: public State {
		virtual void signalLBEndInterrupted() {
			LOG_DEBUG <<"State: EndFailBegin\n";

			switch (data->counterEndFail) {
			case 0:
				data->cte1->giveTime();
				new (this) EndPuck1Recognized;
				break;
			case 1:
				new (this) EndPuck2Ready;
				break;
			case 2:
				new (this) EndPuck3Ready;
				break;
			}
			new (this) EndPuck1Recognized;
		}

	};

	struct EndPuck1Recognized: public State {
		virtual void signalLBEndNotInterrupted() {
			LOG_DEBUG <<"State: EndPuck1Recognized\n";
			data->counterEndFail = 1;
			data->delta1 = data->cte1 - data->cth1;
			if (data->delta1 != DELTA_CTH1_CTE1) {
				data->hb.getHardware()->getTL()->turnGreenOff();
				data->blinkRed.start(NULL);
				data->cm->setSpeed(MOTOR_STOP);
				data->cm->transact();
				cout
						<< "HLER!!!!!!!!!!! PUCK WURDE HINZUGEFÜGT!!!!!!!!!!!!!!!!!!!!!!!!"
						<< endl;
				LOG_DEBUG <<"Fehler: PUCK WURDE HINZUGEFÜGT\n";

				new (this) PuckAdded;
			} else {
				new (this) EndPuck2Ready;
			}

		}

	};

	struct EndPuck2Ready: public State {
		virtual void signalLBEndInterrupted() {
			LOG_DEBUG <<"State: EndPuck2Ready\n";
			data->cte2->giveTime();
			new (this) EndPuck2Recognized;
		}

	};

	struct EndPuck2Recognized: public State {
		virtual void signalLBEndNotInterrupted() {
			LOG_DEBUG <<"State: EndPuck2Recognized\n";
			data->counterEndFail = 2;
			data->delta2 = data->cte2 - data->cth2;
			if (data->delta2 != DELTA_CTH2_CTE2) {
				data->hb.getHardware()->getTL()->turnGreenOff();
				data->blinkRed.start(NULL);
				data->cm->setSpeed(MOTOR_STOP);
				data->cm->transact();
				cout
						<< "HLER!!!!!!!!!!! PUCK WURDE HINZUGEFÜGT!!!!!!!!!!!!!!!!!!!!!!!!"
						<< endl;

				LOG_DEBUG <<"Fehler: PUCK WURDE HINZUGEFÜGT\n";

				new (this) PuckAdded;
			} else {
				new (this) EndPuck3Ready;
			}

		}

	};

	struct EndPuck3Ready: public State {
		virtual void signalLBEndInterrupted() {
			LOG_DEBUG <<"State: EndPuck3Ready\n";
			data->cte3->giveTime();
			new (this) HeightPuck3Recognized;
		}

	};

	struct EndPuck3Recognized: public State {
		virtual void signalLBEndNotInterrupted() {
			LOG_DEBUG <<"State: EndPuck3Recognized\n";
			data->counterEndFail = 0;
			data->delta3 = data->cte3 - data->cth3;
			if (data->delta3 != DELTA_CTH3_CTE3) {
				data->hb.getHardware()->getTL()->turnGreenOff();
				data->blinkRed.start(NULL);
				data->cm->setSpeed(MOTOR_STOP);
				data->cm->transact();
				cout
						<< "FEHLER!!!!!!!!!!! PUCK WURDE HINZUGEFÜGT!!!!!!!!!!!!!!!!!!!!!!!!"
						<< endl;
				LOG_DEBUG <<"Fehler: PUCK WURDE HINZUGEFÜGT\n";

				new (this) PuckAdded;
			} else {
				new (this) PucksToConsole;
			}

		}

	};

	struct PucksToConsole: public State {

		virtual void signalLBEndNotInterrupted() {
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

			data->finished = true;
		}

	};

	struct PuckAdded: public State {
		virtual void SignalReset() {
			data->cm->resetSpeed(MOTOR_STOP);
			data->blinkRed.stop();
			data->finished = true;
			new (this) EndOfTheEnd;
		}

	};

	struct PuckLost: public State { //TODO: ErrorMessage ausgben!!!!!!!!!!!!!
		virtual void SignalReset() {
			data->cm->resetSpeed(MOTOR_STOP);
			//TODO Yellow blink ausschalten, wenn überall gesetzt wurde --> Fehlerzustand fehlt noch
			data->finished = true;
			new (this) EndOfTheEnd;
		}

	};

	struct EndOfTheEnd: public State {

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
