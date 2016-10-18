/*
 * TrafficLight.h
 *
 *  Created on: 11.10.2016
 */

#ifndef TRAFFICLIGHT_H_
#define TRAFFICLIGHT_H_
#include <hw/inout.h>
#include <stdint.h>
#include "Logger/Logger.h"
#include "Hal/Adapter.h"

class TrafficLight
{
public:
    TrafficLight(Adapter* adapt);
    ~TrafficLight();

    uint8_t turnGreenOn();
    uint8_t turnGreenOff();
    uint8_t turnYellowOn();
    uint8_t turnYellowOff();
    uint8_t turnRedOn();
    uint8_t turnRedOff();

private:
    uint8_t redLightBitmask_;
    uint8_t greenLightBitmask_;
    uint8_t yellowLightBitmask_;

    Adapter* adapter;
};

#endif /* TRAFFICLIGHT_H_ */
