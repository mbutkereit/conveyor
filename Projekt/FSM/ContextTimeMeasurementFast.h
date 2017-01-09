/*
 * ContextTimeMeasurementFast.h
 *
 *  Created on: 14.12.2016
 *      Author: abq808
 */

#ifndef CONTEXTTIMEMEASUREMENTFAST_H_
#define CONTEXTTIMEMEASUREMENTFAST_H_
#include <iostream>
#include <unistd.h>
#include <iostream>
#include "Logger/Logger.h"
#include "Hal/HalBuilder.h"
#include "ContextMotor.h"
#include "ContextSwitch.h"
#include "ContextI.h"

struct Datactmf {
    Datactmf() :
            hb(), cm(ContextMotor::getInstance()), cswitch(ContextSwitch::getInstance()), delta_t0_tH(0), delta_tH_tW(0), delta_tW_tE(0), delta_t0_tE(0), tickX(NULL), tickGesamtSchnell(NULL) ,finished(false) {

    }
    HalBuilder hb;
    ContextMotor* cm;
    ContextSwitch* cswitch;
    int delta_t0_tH;
    int delta_tH_tW;
    int delta_tW_tE;
    int delta_t0_tE;
    int *tickX;
    int *tickGesamtSchnell;
    bool finished;
};

class ContextTimeMeasurementFast :public ContextI {
public:
    ContextTimeMeasurementFast();
    virtual ~ContextTimeMeasurementFast();

    bool isContextimEnzustand() {
        if (ctmfdata.finished){
            LOG_DEBUG << "Ich bin jetzt im Endzustand \n";
        }
        return ctmfdata.finished;
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
            if(data->tickX != NULL && data->tickGesamtSchnell != NULL){
               	*data->tickX += 1;
                *data->tickGesamtSchnell += 1;
            }
        }

        Datactmf* data;
    }*statePtr;   // a pointer to current state. Used for polymorphism.

    struct TransportToEntry: public TimereadingFast {
        virtual void signalLBBeginInterrupted() {
        	LOG_DEBUG << "TimereadingFast State: TransportToEntry\n";
            data->cm->setSpeed(MOTOR_FAST);
            new (this) MotorOn;
        }
    };

    struct MotorOn: public TimereadingFast{
        virtual void signalLBBeginNotInterrupted() {
        	LOG_DEBUG << "TimereadingFast State: MotorOn\n";
            data->tickX = &data->delta_t0_tH;
            data->tickGesamtSchnell = &data->delta_t0_tE;
            new (this) TransportToAltimetry;
        }
    };

    struct TransportToAltimetry: public TimereadingFast{
        virtual void signalLBAltimetryInterrupted() {
        	LOG_DEBUG << "TimereadingFast State: TransportToAltimetry\n";
            data->tickX = &data->delta_tH_tW;
            new (this) TransportToSwitch;
        }
    };

    struct TransportToSwitch: public TimereadingFast{
        virtual void signalLBSwitchInterrupted() {
        	LOG_DEBUG << "TimereadingFast State: TransportToSwitch\n";
            data->tickX = &data->delta_tW_tE;
            data->cswitch->setSwitchOpen();
            new (this) TransportToDelivery;
        }
    };

    struct TransportToDelivery: public TimereadingFast {
        virtual void signalLBEndInterrupted() {
        	LOG_DEBUG << "TimereadingFast State: TransportToDelivery\n";
            data->tickX = NULL;
            data->tickGesamtSchnell = NULL;
            data->cswitch->resetSwitchOpen();
            data->cm->setSpeed(MOTOR_STOP);

            cout << "DELTA_T0_TH: " << data->delta_t0_tH << endl;
            cout << "DELTA_TH_TW: " << data->delta_tH_tW << endl;
            cout << "DELTA_TW_TE: " << data->delta_tW_tE << endl;
            cout << "DELTA_T0_TE: " << data->delta_t0_tE << endl;
            data->finished = true;
        }
    };

    TransportToEntry stateMember; //The memory for the state is part of context object
    Datactmf ctmfdata;
};

#endif /* CONTEXTTIMEMEASUREMENTFAST_H_ */
