/*
 * ContextTimeMeasurementFast.h
 *
 *  Created on: 14.12.2016
 *      Author: abq808
 */

#ifndef CONTEXTTIMEMEASUREMENTFAST_H_
#define CONTEXTTIMEMEASUREMENTFAST_H_
#include <iostream>
#include "Logger/Logger.h"
#include "Hal/HalBuilder.h"
#include "ContextMotor.h"
#include "Puck.h"
#include <vector>
#include "ContextSorting.h"
#include "ContextSwitch.h"
#include "Serializer/Serializer.h"
#include "Serializer/InfoMessage.h"
#include "Serializer/WorkpieceMessage.h"
#include "Thread/BlinkRedThread.h"
#include "Thread/BlinkYellowThread.h"
#include "ContextI.h"

extern HalBuilder hb; ///< Der HalBuilder um sicher und zentral auf die Hardware zuzugreifen.

struct TimeFastData {
	TimeFastData() :
			cm(ContextMotor::getInstance()), cs(ContextSwitch::getInstance()), finished(
					false) {
	}

	ContextMotor *cm;
	ContextSwitch *cs;
	bool finished;
};

class ContextTimeMeasurementFast {

private:
	struct TimeMeasurementFast {
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

		TimeFastData* data; // pointer to data, which physically resides inside the context class (contextdata)
	}*statePtr;   // a pointer to current state. Used for polymorphism.

	struct MotorOn: public TimeMeasurementFast {
		void signalLBBeginInterrupted() {
			data->cm->setSpeed(MOTOR_FAST);

			new (this) TransportToHeightMeasurement;
		}
	};

	struct TransportToHeightMeasurement: public TimeMeasurementFast {
		void signalLBBeginNotInterrupted() {
			// TODO: Gib Ticks für t0
			//TODO: Zeiterfassung t0

			new (this) TransportToSwitch;
		}
	};

	struct TransportToSwitch: public TimeMeasurementFast {
		void signalLBAlitmetryInterrupted() {
			// TODO: Gib Ticks für th
			//TODO: Delta t0 und tH

			new (this) TransportToDelivery;
		}
	};

	struct TransportToDelivery: public TimeMeasurementFast {
		void signalLBSwitchInterrupted() {

			// TODO: Gib Ticks für tW
			//TODO: Delta tH und tW
			data->cs->setSwitchOpen();

			new (this) PuckInEnd;
		}
	};

	struct PuckInEnd: public TimeMeasurementFast {
		void signalLBSwitchInterrupted() {

			// TODO: Gib Ticks für tE
			//TODO:Zeiterfassung tE
			//TODO: Delta tW und tE
			//TODO: Delta von Zeiterfassung t0 und tE
			data->cs->resetSwitchOpen();
			data->cm->setSpeed(MOTOR_STOP);
			data->finished = true;
		}
	};

	MotorOn stateMember; //The memory for the state is part of context object
	TimeFastData contextdata;  //Data is also kept inside the context object

public:

	ContextTimeMeasurementFast();
	virtual ~ContextTimeMeasurementFast();

	void signalLBBeginInterrupted();

	void signalLBEndInterrupted();

	void signalLBAltimetryInterrupted();

	void signalLBSwitchInterrupted();

	void signalLBBeginNotInterrupted();

};

#endif /* CONTEXTTIMEMEASUREMENTFAST_H_ */
