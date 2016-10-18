/*
 * TrafficLight.cpp
 *
 *  Created on: 11.10.2016
 *      Author: marvin
 */

#include <TrafficLight.h>
#include <Logger/Logger.h>

TrafficLight::TrafficLight(Adapter* adapt):adapter(adapt)
{
  //  registerAddress_  = 0x300;
    redLightBitmask_ = 0x80;
    greenLightBitmask_ = 0x20;
    yellowLightBitmask_ = 0x40;
}

TrafficLight::~TrafficLight()
{
}

uint8_t TrafficLight::turnGreenOn()
{
	adapter->setBitMask(greenLightBitmask_);
    LOG_DEBUG << " Gruenes licht an.";
    return 0;
}

uint8_t TrafficLight::turnGreenOff()
{
	adapter->removeBitMask(greenLightBitmask_);
    LOG_DEBUG << " Gruenes licht aus.";
    return 0;
}

uint8_t TrafficLight::turnYellowOn()
{

	adapter->setBitMask(yellowLightBitmask_);
    LOG_DEBUG << " Gelbes licht an.";
    return 0;
}

uint8_t TrafficLight::turnYellowOff()
{
	adapter->removeBitMask(yellowLightBitmask_);
    LOG_DEBUG << " Gelbes licht aus.";
    return 0;
}

uint8_t TrafficLight::turnRedOn()
{
	adapter->setBitMask(redLightBitmask_);
    LOG_DEBUG << " Rotes licht an.";
    return 0;
}

uint8_t TrafficLight::turnRedOff()
{
	adapter->removeBitMask(redLightBitmask_);
    LOG_DEBUG << " Rotes licht aus.";
    return 0;
}

