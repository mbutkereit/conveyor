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
#define BASEADRESS_B 0x301
#define BASEADRESS_C 0x302
#define BASEADRESS_D 0x320

#define CONTROL_ADDRESS_0 0x303
#define CONTROL_BITMASK 0x8A

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
    Adapter* adapterD;
    Hardware* buildHardware();
    HumanMachineInterface* buildHumanMachineInterface();
    Motor* buildMotor();
    TrafficLight* buildTrafficLights();
    MeasuringTool* buildMeasuringTool();
    Altimetry* buildAltimetry();
	static Hardware* instance_;
	Mutexo mutex;
};

#endif /* HALBUILDER_H_ */
