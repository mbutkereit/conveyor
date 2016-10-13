/*
 * Hardware.h
 *
 *  Created on: 05.10.2016
 *      Author: marvin
 */

#ifndef HARDWARE_H_
#define HARDWARE_H_

#include "Hal/Motor.h"
#include "Hal/HumanMachineInterface.h"
#include "Hal/MeasuringTool.h"
#include "Hal/TrafficLight.h"

class Hardware
{
	public:
		Hardware(HumanMachineInterface* hmi, Motor* motor,TrafficLight* tl, MeasuringTool* mt);
		~Hardware();
		HumanMachineInterface* getHMI();
		Motor* getMotor();
		TrafficLight* getTL();
		MeasuringTool* getMT();

	private:
		HumanMachineInterface* hmi;
		Motor* motor;
		TrafficLight* tl;
		MeasuringTool* mt;
};

#endif /* HARDWARE_H_ */
