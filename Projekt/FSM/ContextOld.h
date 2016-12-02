/*
 * Context.h
 *
 *  Created on: 08.11.2016
 *      Author: abx827
 */

//#ifndef CONTEXT_H_
//#define CONTEXT_H_
//
//#define DELTA_T0_TH 0
//#define DELTA_TH_TW 0
//#define DELTA_TW_TE 0
//#define MAXIMUM_PUCKS 1
//#define HM1_HM2_TOLERANCE 0
//
//#include <iostream>
//#include <ctime>
//#include <map>
//#include <unistd.h>
//#include "Puck.h"
//#include "Logger/Logger.h"
//#include "Hal/HalBuilder.h"
//#include "ContextSorting.h"
//#include "ErrorStates.h"
//#include "FSM/ContextMotor.h"
//#include "FSM/MotorOptions.h"
//#include "FSM/ContextTimer.h"
//#include "FSM/ContextSwitch.h"
//
//using namespace std;
//extern HalBuilder hb;
//
//struct Datac {
//	Datac(ContextSorting* csorting, ContextMotor* cmotor) :
//			hb(), cs(csorting), cswitch(ContextSwitch::getInstance()), ct(ContextTimer::getInstance()), cm(cmotor), puck(-1), puckmap(), es(), id(0), height(-1), t1(0), t2(0), delta(0) {
//	}
//	HalBuilder hb;
//	ContextSorting* cs;
//	ContextSwitch* cswitch;
//	ContextTimer* ct;
//	ContextMotor* cm;
//	Puck puck;
//	map<int,Puck> puckmap;
//	ErrorStates es;
//	int id;
//	int height;
//	clock_t t1;
//	clock_t t2;
//	clock_t delta;
//};
//
//class Context {
//private:
//
//	struct PuckOnConveyor1 { //top-level state
//		Datac* data;
//		virtual void signalEStop() {
//			//struct PuckOnConveyor1* hs;
//			data->cm->setSpeed(MOTOR_STOP);
//			data->cm->transact();
//			//E-Stopp unlock missing
//			/*while (1) {
//			 if (0) {
//			 break;
//			 }
//			 }
//			 */
//
//		} //put code here for signalEStop in superstate
//		virtual void transact() {
//		}
//	}*statePtr;   // a pointer to current state. Used for polymorphism.
//
//	struct TransportToEntry: public PuckOnConveyor1 {
//		virtual void transact() {
//			data->hb.getHardware()->getTL()->turnGreenOn();
//			while (data->hb.getHardware()->getMT()->isItemRunningIn()) {
//			} //LS_E BAND 1 müsste stattdessen geprüft werden!
//			new (this) MotorOn;
//		}
//	};
//
//	struct MotorOn: public PuckOnConveyor1 { //ID wird von Band1 vergeben
//		virtual void transact() {
//			int id = (data->id);
//			Puck puck(id);
//			data->puck = puck;
//			data->cm->setSpeed(MOTOR_FAST);
//			data->cm->transact();
//			data->puckmap.insert(pair<int, Puck>(id, puck));
//			while (!(data->hb.getHardware()->getMT()->isItemRunningIn())) {
//			}
//			data->t1 = data->ct->giveTime();
//			new (this) TransportToHeightMeasurement;
//		}
//	};
//
//	struct TransportToHeightMeasurement: public PuckOnConveyor1 {
//		virtual void transact() {
//			while (data->hb.getHardware()->getMT()->isItemAltimetry()) {
//			}
//			data->t2 = data->ct->giveTime();
//			data->delta = data->t2 - data->t1;
//			data->cm->setSpeed(MOTOR_SLOW);
//			data->cm->transact();
//			if (data->delta < DELTA_T0_TH) {
//				new (this) PuckAdded;
//			} else {
//				new (this) PuckInHeightMeasurement;
//			}
//		}
//	};
//
//	struct PuckInHeightMeasurement: public PuckOnConveyor1 {
//		virtual void transact() {
//			int height = data->hb.getHardware()->getAltimetry()->getHeight();
//			data->puck.setHeightReading1(height);
//			if (data->hb.getHardware()->getMT()->isItemInAltimetryToleranceRange()) {
//				data->puck.setPuckdrillhole(DRILL_HOLE_UPSIDE);
//			} else {
//				data->puck.setPuckdrillhole(NO_DRILL_HOLE);
//			}
//			while (!(data->hb.getHardware()->getMT()->isItemAltimetry())) {
//			}
//			data->cm->resetSpeed(MOTOR_SLOW);
//			data->cm->transact();
//			new (this) TransPortToSkid;
//		}
//	};
//
//	struct TransPortToSkid: public PuckOnConveyor1 {
//		virtual void transact() {
//			while (data->hb.getHardware()->getMT()->isItemSwitch()) {
//			}
//			//t_h and t_w
//			data->t1 = data->t2;
//			data->t2 = data->ct->giveTime();
//			data->delta = data->t2 - data->t1;
//			new (this) MetalDetection;
//		}
//	};
//
//	struct MetalDetection: public PuckOnConveyor1 {
//		virtual void transact() {
//			if (data->hb.getHardware()->getMT()->isItemMetal()) {
//				data->puck.setPuckmaterial(METAL);
//			} else {
//				data->puck.setPuckmaterial(PLASTIC);
//			}
//			if (!(data->hb.getHardware()->getMT()->isSkidFull())) {
//                data->es.bothSkidsFull();
//			}
//			if (data->delta < DELTA_TH_TW) {
//				new (this) PuckAdded;
//			} else {
//				new (this) Sorting;
//			}
//		}
//	};
//
//	struct Sorting: public PuckOnConveyor1 {
//		virtual void transact() {
//			data->cs->setCurrentPh(data->puck.getPuckdrillhole());
//			data->cs->setCurrentPm(data->puck.getPuckmaterial());
//			data->cs->transact();
//			if (data->cs->getSequenceOk()) {
//				new (this) TransportToDelivery;
//			} else {
//				if (data->hb.getHardware()->getMT()->isSkidFull()) {
//					new (this) SortOutThroughSkid;
//				} else {
//					data->hb.getHardware()->getTL()->turnYellowOn();
//					new (this) TransportToDelivery;
//				}
//			}
//		}
//	};
//
//	struct SortOutThroughSkid: public PuckOnConveyor1 {
//        virtual void transact() {
//            while (data->hb.getHardware()->getMT()->isSkidFull()){
//            }
//            if (data->puckmap.size() > 0)
//            {
//                new (this) Finish;
//            }
//            else
//            {
//                new (this) Conveyor1Empty;
//            }
//        }
//	};
//
//	struct Conveyor1Empty: public PuckOnConveyor1 {
//	    virtual void transact() {
//	        data->cm->setSpeed(MOTOR_STOP);
//	        data->cm->transact();
//	        data->cm->resetSpeed(MOTOR_STOP);
//	        data->cm->transact();
//	        new (this) Finish;
//	    }
//	};
//
//	struct TransportToDelivery: public PuckOnConveyor1 {
//		virtual void transact() {
//		    data->cswitch->setSwitchOpen();
//		    data->cswitch->transact();
//			data->t1 = data->t2;
//			while (data->hb.getHardware()->getMT()->isItemRunningOut()) {
//			}
//			data->t2 = data->ct->giveTime();
//			data->delta = data->t2 - data->t1;
//			data->cswitch->resetSwitchOpen();
//			data->cswitch->transact();
//			if (data->delta < DELTA_TW_TE) {
//				data->hb.getHardware()->getTL()->turnGreenOff();
//				new (this) PuckAdded;
//			} else {
//				new (this) DeliveryToConveyor2;
//			}
//		}
//	};
//
//	struct DeliveryToConveyor2: public PuckOnConveyor1 {
//		virtual void transact() {
//		    data->cm->setSpeed(MOTOR_STOP);
//		    data->cm->transact();
//			/*Check for conveyor2 isFree() missing!
//			 * while(conveyor2NotFree){}
//			 * data->cm->resetSpeed(MOTOR_STOP);
//             * data->cm->transact();
//			 */
//			new (this) TransportToConveyor2;
//		}
//	};
//
//	struct TransportToConveyor2: public PuckOnConveyor1 {
//		virtual void transact() {
//			while (!(data->hb.getHardware()->getMT()->isItemRunningOut())) {
//			}//CHECK FOR LS_A OF CONVEYOR2 IS NEEDED
//			usleep(4000000);
//			data->puckmap.erase(data->puck.getId());
//			if (data->puckmap.empty()) {
//			    new (this) Conveyor1Empty;
//			}
//			else
//			{
//			    new (this) Finish;
//			}
//		}
//	};
//
//	struct BothSkidsFull: public PuckOnConveyor1 {
//		virtual void transact() {
//		    data->cm->setSpeed(MOTOR_STOP);
//			data->hb.getHardware()->getTL()->turnGreenOff();
//			//Errormessage impelementation missing
//			while (!(data->hb.getHardware()->getHMI()->isButtonStartPressed())) {
//				data->hb.getHardware()->getTL()->turnRedOn();
//				usleep(500000);
//				data->hb.getHardware()->getTL()->turnRedOff();
//				usleep(500000);
//			}
//			data->hb.getHardware()->getTL()->turnGreenOn();
//			//new (this) history
//		}
//	};
//
//	struct PuckLost: public PuckOnConveyor1 {
//		virtual void transact() {
//		    data->cm->setSpeed(MOTOR_STOP);
//			//Errormessage implementation missing
//			while (!(data->hb.getHardware()->getHMI()->isButtonStartPressed())) {
//				data->hb.getHardware()->getTL()->turnYellowOn();
//				usleep(500000);
//				data->hb.getHardware()->getTL()->turnYellowOff();
//				usleep(500000);
//			}
//			data->hb.getHardware()->getTL()->turnYellowOff();
//			data->hb.getHardware()->getTL()->turnGreenOn();
//			//new (this) history
//		}
//	};
//
//	struct PuckAdded: public PuckOnConveyor1 {
//		virtual void transact() {
//
//			data->hb.getHardware()->getMotor()->stop();
//			//Errormessage implementation missing
//			while (!(data->hb.getHardware()->getHMI()->isButtonStartPressed())) {
//				data->hb.getHardware()->getTL()->turnRedOn();
//				usleep(500000);
//				data->hb.getHardware()->getTL()->turnRedOff();
//				usleep(500000);
//			}
//			data->hb.getHardware()->getTL()->turnRedOff();
//			data->puckmap.erase(data->puck.getId());
//			new (this) Finish;
//		}
//	};
//
//	struct Finish: public PuckOnConveyor1 {
//		virtual void transact() {
//		}
//	};
//
//public:
//	Datac cdata;
//	TransportToEntry stateMember; //The memory for the state is part of context object
//
//	Context(ContextSorting* csorting, ContextMotor* cmotor) :
//			statePtr(&stateMember), cdata(csorting, cmotor) // assigning start state
//	{
//		statePtr->data = &cdata;
//	}
//
//	~Context(){};
//
//	void transact() {
//		statePtr->transact();
//	} // context delegates signals to state
//
//	void signalLBBeginInterrupted();
//	void signalLBEndInterrupted();
//	void signalLBAltimetryInterrupted();
//	void signalLBSwitchInterrupted();
//
//	void signalLBBeginNotInterrupted();
//	void signalLBEndInNotInterrupted();
//	void signalLBAltimetryNotInterrupted();
//	void signalLBSwitchNotInterrupted();
//
//	void signalEStop();
//	void signalStart();
//	void signalStop();
//	void signalReset();
//
//	void signalLBSkidInterrupted();
//	void signalLBSkidNotInterrupted();
//
//	void signalAltimetryCompleted();
//};
//
//#endif /* CONTEXT_H_ */
