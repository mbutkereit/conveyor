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
            hb(), cm(ContextMotor::getInstance()), cswitch(ContextSwitch::getInstance()), t0_th(0), th_tw(0), tw_te(0), tickX(NULL), finished(false) {
    }
    HalBuilder hb;
    ContextMotor* cm;
    ContextSwitch* cswitch;
    int t0_th;
    int th_tw;
    int tw_te;
    int *tickX;
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
    struct TimereadingFast { //top-level state
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
            *data->tickX += 1;
        }

        Datactms* data;
    }*statePtr;   // a pointer to current state. Used for polymorphism.

    struct TransportToEntry: public TimereadingFast {
        virtual void signalLBBeginInterrupted() {
            data->cm->setSpeed(MOTOR_SLOW);

            new (this) MotorOn;
        }
    };

    struct MotorOn: public TimereadingFast{
        virtual void signalLBBeginNotInterrupted() {
            data->tickX = &data->t0_th;
            new (this) TransportToAltimetry;
        }
    };

    struct TransportToAltimetry: public TimereadingFast{
        virtual void signalLBAltimetryInterrupted() {
            data->tickX = &data->th_tw;
            new (this) TransportToSkid;
        }
    };

    struct TransportToSkid: public TimereadingFast{
        virtual void signalLBSwitchInterrupted() {
            data->tickX = &data->tw_te;
            data->cswitch->setSwitchOpen();
            data->cswitch->transact();
            new (this) TransportToDelivery;
        }
    };

    struct TransportToDelivery: public TimereadingFast {
        virtual void signalLBEndInterrupted() {
            data->tickX = NULL;
            data->cswitch->resetSwitchOpen();
            data->cswitch->transact();
            data->cm->setSpeed(MOTOR_STOP);

            cout << "DELTA_T0_TH: " << data->t0_th << endl;
            cout << "DELTA_TH_TW: " << data->th_tw << endl;
            cout << "DELTA_TW_TE: " << data->tw_te << endl;
            data->finished = true;
        }
    };

    TransportToEntry stateMember; //The memory for the state is part of context object
    Datactms ctmsdata;
};

#endif /* CONTEXTTIMEMEASUREMENTSLOW_H_ */
