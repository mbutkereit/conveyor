/*
 * HalBuilder.h
 *
 *  Created on: 12.10.2016
 *      Author: marvin
 */

#ifndef HALBUILDER_H_
#define HALBUILDER_H_

#include "Hal/Adapter.h"
#include "Hal/Hardware.h"
#include "Hal/Motor.h"
#include "Hal/HumanMachineInterface.h"
#include "Hal/MeasuringTool.h"
#include "Hal/TrafficLight.h"
#include "Hal/Mutexo.h"
#define BASEADRESS_A 0x300
#define BASEADRESS_B 0x320
#define BASEADRESS_C 0x340 //?????????????????????????????????????????????????

class HalBuilder
{
public:
    HalBuilder();
    Hardware* getHardware();
    virtual ~HalBuilder();
private:
    Adapter* adapterA;
    Adapter* adapterB;
    Adapter* adapterC;
    Hardware* buildHardware();
    HumanMachineInterface* buildHumanMachineInterface();
    Motor* buildMotor();
    TrafficLight* buildTrafficLights();
    MeasuringTool* buildMeasuringTool();
	static Hardware* instance_;
	Mutexo mutex;
};

#endif /* HALBUILDER_H_ */
