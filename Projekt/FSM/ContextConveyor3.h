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

struct Data {
	Data(int puckID, std::vector<Puck>* puckVector) :

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
					ContextMotor::getInstance()), puck(-1), currState(0), id(0), height(
					-1), delta1(0), delta2(0), delta3(0), counterHeightFail(0), counterEndFail(
					0), finished(false), puckVector(puckVector) {
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
		Data* data; // pointer to data, which physically resides inside the context class (contextdata)
	}*statePtr;   // a pointer to current state. Used for polymorphism.

	struct ReceivingPucks: public State {
		virtual void signalLBBeginInterrupted() {
			data->currState++;
			data->hb.getHardware()->getTL()->turnGreenOn();
			data->cm->setSpeed(MOTOR_SLOW);
			data->cm->transact();
			data->ct1->giveTime();
			new (this) Puck1Recognized;

		}

	};

	struct Puck1Recognized: public State {
		virtual void signalLBBeginNotInterrupted() {
			data->currState++;
			data->puckVector->push_back(data->puck);
			data->cm->resetSpeed(MOTOR_STOP);
			data->cm->transact();
			new (this) Puck2Ready;
		}

	};

	struct Puck2Ready: public State {

		virtual void signalLBBeginInterrupted() {
			data->currState++;
			data->cm->setSpeed(MOTOR_SLOW);
			data->cm->transact();
			data->ct2->giveTime();
			new (this) Puck2Recognized;
		}

	};

	struct Puck2Recognized: public State {
		virtual void signalLBBeginNotInterrupted() {
			data->currState++;
			data->puckVector->push_back(data->puck);

			data->cm->resetSpeed(MOTOR_STOP);
			data->cm->transact();
			new (this) Puck3Ready;
		}

	};

	struct Puck3Ready: public State {
		virtual void signalLBBeginInterrupted() {
			data->currState++;
			data->currState = 4;
			data->cm->setSpeed(MOTOR_SLOW);
			data->cm->transact();
			data->ct3->giveTime();
			new (this) Puck3Recognized;
		}

	};

	struct Puck3Recognized: public State {
		virtual void signalLBBeginNotInterrupted() {
			data->puckVector->push_back(data->puck);
			data->currState++;
			new (this) EndReceiving;
		}

	};

	struct EndReceiving: public State {
		virtual void ssignalLBAltimetryInterrupted() {
			data->currState++;
			new (this) HeightFailBegin;
		}

	};

	struct HeightFailBegin: public State {
		virtual void ssignalLBAltimetryInterrupted() {
			data->currState++;
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
			data->currState++;
			data->counterHeightFail = 1;
			data->delta1 = data->cth1 - data->ct1;
			if (data->delta1 != DELTA_CT1_CTH1) {
				data->hb.getHardware()->getTL()->turnGreenOff();
				data->hb.getHardware()->getTL()->turnRedOn();
				data->cm->resetSpeed(MOTOR_STOP);
				data->cm->transact();
				new (this) PuckAdded;
			} else {
				new (this) HeightPuck2Ready;
			}

		}

	};

	struct HeightPuck2Ready: public State {
		virtual void ssignalLBAltimetryInterrupted() {
			data->currState++;
			data->cth2->giveTime();
			new (this) HeightPuck2Recognized;
		}

	};

	struct HeightPuck2Recognized: public State {
		virtual void ssignalLBAltimetryNotInterrupted() {
			data->currState++;
			data->counterHeightFail = 2;
			data->delta2 = data->cth2 - data->ct2;
			if (data->delta2 != DELTA_CT2_CTH2) {
				data->hb.getHardware()->getTL()->turnGreenOff();
				data->hb.getHardware()->getTL()->turnRedOn();
				data->cm->resetSpeed(MOTOR_STOP);
				data->cm->transact();
				new (this) PuckAdded;
			} else {
				new (this) HeightPuck3Ready;
			}

		}

	};

	struct HeightPuck3Ready: public State {

		virtual void ssignalLBAltimetryInterrupted() {
			data->currState++;
			data->cth3->giveTime();
			new (this) HeightPuck3Recognized;
		}

	};

	struct HeightPuck3Recognized: public State {
		virtual void ssignalLBAltimetryNotInterrupted() {
			data->currState++;
			data->counterHeightFail = 0;
			data->delta3 = data->cth3 - data->ct3;
			if (data->delta3 != DELTA_CT3_CTH3) {
				data->hb.getHardware()->getTL()->turnGreenOff();
				data->hb.getHardware()->getTL()->turnRedOn();
				data->cm->resetSpeed(MOTOR_STOP);
				data->cm->transact();
				new (this) PuckAdded;
			} else {
				new (this) HeightEnd;
			}

		}

	};

	struct HeightEnd: public State {
		virtual void signalLBSwitchInterrupted() {
			data->currState++;
			data->cswitch->setSwitchOpen();
			new (this) TransportToSwitch;
		}

	};

	struct TransportToSwitch: public State {
		virtual void signalLBEndInterrupted() {
			data->currState++;
			new (this) EndFailBegin;

		}

	};

	struct EndFailBegin: public State {
		virtual void signalLBEndInterrupted() {
			data->currState++;
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
			data->currState++;
			data->counterEndFail = 1;
			data->delta1 = data->cte1 - data->cth1;
			if (data->delta1 != DELTA_CTH1_CTE1) {
				data->hb.getHardware()->getTL()->turnGreenOff();
				data->hb.getHardware()->getTL()->turnRedOn();
				data->cm->resetSpeed(MOTOR_STOP);
				data->cm->transact();
				new (this) PuckAdded;
			} else {
				new (this) EndPuck2Ready;
			}

		}

	};

	struct EndPuck2Ready: public State {
		virtual void signalLBEndInterrupted() {
			data->currState++;
			data->cte2->giveTime();
			new (this) EndPuck2Recognized;
		}

	};

	struct EndPuck2Recognized: public State {
		virtual void signalLBEndNotInterrupted() {
			data->currState++;
			data->counterEndFail = 2;
			data->delta2 = data->cte2 - data->cth2;
			if (data->delta2 != DELTA_CTH2_CTE2) {
				data->hb.getHardware()->getTL()->turnGreenOff();
				data->hb.getHardware()->getTL()->turnRedOn();
				data->cm->resetSpeed(MOTOR_STOP);
				data->cm->transact();
				new (this) PuckAdded;
			} else {
				new (this) EndPuck3Ready;
			}

		}

	};

	struct EndPuck3Ready: public State {
		virtual void signalLBEndInterrupted() {
			data->currState++;
			data->cte3->giveTime();
			new (this) HeightPuck3Recognized;
		}

	};

	struct EndPuck3Recognized: public State {
		virtual void signalLBEndNotInterrupted() {
			data->currState++;
			data->counterEndFail = 0;
			data->delta3 = data->cte3 - data->cth3;
			if (data->delta3 != DELTA_CTH3_CTE3) {
				data->hb.getHardware()->getTL()->turnGreenOff();
				data->hb.getHardware()->getTL()->turnRedOn();
				data->cm->resetSpeed(MOTOR_STOP);
				data->cm->transact();
				new (this) PuckAdded;
			} else {
				new (this) PucksToConsole;
			}

		}

	};

	struct PucksToConsole: public State {

		virtual void signalLBEndNotInterrupted() {
			data->currState++;
			//werkstückdaten in der Konsole ausgeben
			data->finished = true;
		}

	};

	struct PuckAdded: public State {
		virtual void SignalReset() {
			data->hb.getHardware()->getTL()->turnRedOff();
			new (this) EndOfTheEnd;
		}

	};

	struct PuckLost: public State {
		virtual void SignalReset() {
			// Band muss geräumt  werden, da wir am Ende 3er Pärchen brauchen, da darf keins fehlen
			//data->cm->resetSpeed(MOTOR_STOP); Nach State Pattern so leider NICHT MÖGLICH!!!!
			data->hb.getHardware()->getTL()->turnYellowOff();
			data->hb.getHardware()->getTL()->turnGreenOn();
			data->finished = true;
			new (this) EndOfTheEnd;
		}

	};

	struct EndOfTheEnd: public State {

	};

	/*struct Estop: public State {
	 virtual void signalReset() {

	 while (data->hb.getHardware()->getHMI()->isButtonEStopPressed()) {
	 }
	 data->cm->resetSpeed(MOTOR_STOP);
	 data->cm->transact();
	 //TODO UNLOCK CHECK FOR OTHER CONVEYOR
	 //alle Förderbänder quitiert   22
	 switch (data->currState) {
	 case 1:
	 new (this) ReceivingPucks;
	 break;

	 case 2:
	 new (this) Puck1Recognized;
	 break;
	 case 3:
	 new (this) Puck2Ready;
	 break;
	 case 4:
	 new (this) Puck2Recognized;
	 break;
	 case 5:
	 new (this) Puck3Ready;
	 break;
	 case 6:
	 new (this) Puck3Recognized;
	 break;
	 case 7:
	 new (this) EndReceiving;
	 break;
	 case 8:
	 new (this) HeightfailBegin;
	 break;
	 case 9:
	 new (this) HeightPuck1Recognized;
	 break;
	 case 10:
	 new (this) HeightPuck2Ready;
	 break;
	 case 11:
	 new (this) HeightPuck2Recognized;
	 break;
	 case 12:
	 new (this) HeightPuck3Ready;
	 break;
	 case 13:
	 new (this) HeightPuck3Recognized;
	 break;
	 case 14:
	 new (this) HeightEnd;
	 break;
	 case 15:
	 new (this) TransportToSwitch;
	 break;
	 case 16:
	 new (this) EndFailBegin;
	 break;
	 case 17:
	 new (this) EndPuck1Recognized;
	 break;
	 case 18:
	 new (this) EndPuck2Ready;
	 break;
	 case 19:
	 new (this) EndPuck2Recognized;
	 break;
	 case 20:
	 new (this) EndPuck3Ready;
	 break;
	 case 21:
	 new (this) EndPuck3Recognized;
	 break;
	 case 22:
	 new (this) PucksToConsole;
	 break;

	 }
	 }

	 };*/

	ReceivingPucks stateMember; //The memory for the state is part of context object
	Data contextdata;  //Data is also kept inside the context object

public:
	//ContextConveyor3() ;

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
};

#endif /* CONTEXTCONVEYOR3_H_ */
