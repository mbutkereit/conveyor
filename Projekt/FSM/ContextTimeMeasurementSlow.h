/*
 * ContextTimeMeasurementSlow.h
 *
 *  Created on: 14.12.2016
 *      Author: abq808
 */

#ifndef CONTEXTTIMEMEASUREMENTSLOW_H_
#define CONTEXTTIMEMEASUREMENTSLOW_H_

#include <iostream>
#include <unistd.h>
#include <iostream>
#include "Logger/Logger.h"
#include "Hal/HalBuilder.h"
#include "ContextMotor.h"
#include "ContextSwitch.h"
#include "ContextI.h"

struct Datactms {
    Datactms() :
            hb(), cm(ContextMotor::getInstance()), cswitch(ContextSwitch::getInstance()), delta_t0_tE(0), tickGesamtLangsam(NULL), finished(false) {
    }
    HalBuilder hb;
    ContextMotor* cm;
    ContextSwitch* cswitch;
    int delta_t0_tE;
    int *tickGesamtLangsam;
    bool finished;
};

class ContextTimeMeasurementSlow :public ContextI {
public:
    ContextTimeMeasurementSlow();
    virtual ~ContextTimeMeasurementSlow();

    bool isContextimEnzustand() {
        if (ctmsdata.finished){
            LOG_DEBUG << "Ich bin jetzt im Endzustand \n";
        }
        return ctmsdata.finished;
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

    void sensorMeasurementCompleted();

    void signalLBNextConveyor();

    void signalTimerTick();

private:
    struct TimereadingSlow { //top-level state
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
        virtual void sensorMeasurementCompleted() {
        }
        virtual void signalLBNextConveyor() {
        }
        virtual void signalTimerTick(){
        	if(data->tickGesamtLangsam != NULL){
                *data->tickGesamtLangsam += 1;
        	}
        }

        Datactms* data;
    }*statePtr;   // a pointer to current state. Used for polymorphism.

    struct TransportToEntry: public TimereadingSlow {
            virtual void signalLBBeginInterrupted() {
            	LOG_DEBUG << "TimereadingSlow State: TransportToEntry\n";
                data->cm->setSpeed(MOTOR_SLOW);
                new (this) MotorOn;
            }
        };

        struct MotorOn: public TimereadingSlow{
            virtual void signalLBBeginNotInterrupted() {
            	LOG_DEBUG << "TimereadingSlow State: MotorOn(signalLBBeginNotInterrupted)\n";
                data->tickGesamtLangsam = &data->delta_t0_tE;
            }

            virtual void signalLBSwitchInterrupted(){
            	LOG_DEBUG << "TimereadingSlow State: MotorOn(signalLBSwitchInterrupted)\n";
            	data->cswitch->setSwitchOpen();
            	new (this) TransportToDelivery;
            }
        };

        struct TransportToDelivery: public TimereadingSlow {
            virtual void signalLBEndInterrupted() {
            	LOG_DEBUG << "TimereadingSlow State: TransportToDelivery\n";
                data->tickGesamtLangsam = NULL;
                data->cswitch->resetSwitchOpen();
                data->cm->setSpeed(MOTOR_STOP);

                cout << "DELTA_T0_TE: " << data->delta_t0_tE << endl;
                data->finished = true;
            }
        };

    TransportToEntry stateMember; //The memory for the state is part of context object
    Datactms ctmsdata;
};

#endif /* CONTEXTTIMEMEASUREMENTSLOW_H_ */
