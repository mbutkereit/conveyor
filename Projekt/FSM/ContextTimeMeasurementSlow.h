/*
 * ContextTimeMeasurementSlow.h
 *
 *  Created on: 14.12.2016
 *      Author: abq808
 */

#ifndef CONTEXTTIMEMEASUREMENTSLOW_H_
#define CONTEXTTIMEMEASUREMENTSLOW_H_
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

struct TimeSlowData {
	TimeSlowData() :
			cm(ContextMotor::getInstance()), cs(ContextSwitch::getInstance()), finished(
					false) {
	}

	ContextMotor *cm;
	ContextSwitch *cs;
	bool finished;
};

class ContextTimeMeasurementSlow {

private:
	struct TimeMeasurementSlow {
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

		TimeSlowData* data; // pointer to data, which physically resides inside the context class (contextdata)
	}*statePtr;   // a pointer to current state. Used for polymorphism.

	struct MotorOn: public TimeMeasurementSlow {
		void signalLBBeginInterrupted() {
			data->cm->setSpeed(MOTOR_SLOW);

			new (this) TransportToHeightMeasurement;
		}
	};

	struct TransportToHeightMeasurement: public TimeMeasurementSlow {
		void signalLBBeginNotInterrupted() {
			//TODO: Zeiterfassung t0

			new (this) TransportToDelivery;
		}
	};



	struct TransportToDelivery: public TimeMeasurementSlow {
		void signalLBSwitchInterrupted() {
			data->cs->setSwitchOpen();

			new (this) PuckInEnd;
		}
	};

	struct PuckInEnd: public TimeMeasurementSlow {
		void signalLBSwitchInterrupted() {
			//TODO:Zeiterfassung tE
			//TODO: Delta von Zeiterfassung t0 und tE
			data->cs->resetSwitchOpen();
			data->cm->setSpeed(MOTOR_STOP);
			data->finished = true;
		}
	};

	MotorOn stateMember; //The memory for the state is part of context object
	TimeSlowData contextdata;  //Data is also kept inside the context object

public:

	ContextTimeMeasurementSlow();
	virtual ~ContextTimeMeasurementSlow();

	void signalLBBeginInterrupted();

	void signalLBEndInterrupted();

	void signalLBAltimetryInterrupted();

	void signalLBSwitchInterrupted();

	void signalLBBeginNotInterrupted();

};

#endif /* CONTEXTTIMEMEASUREMENTSLOW_H_ */
