/*
 * Hardware.cpp
 *
 *  Created on: 05.10.2016
 *      Author: marvin
 */

#include <Hal/Hardware.h>

Hardware::Hardware(HumanMachineInterface* hmi, Motor* motor,TrafficLight* tl, MeasuringTool* mt)
{
	this->hmi = hmi;
	this->motor = motor;
	this->tl = tl;
	this->mt = mt;

}

Hardware::~Hardware()
{
    // TODO Auto-generated destructor stub
}

HumanMachineInterface* Hardware::getHMI(){
	return hmi;
}
Motor* Hardware::getMotor(){
	return motor;
}
TrafficLight* Hardware::getTL(){
	return tl;
}
MeasuringTool* Hardware::getMT(){
	return mt;
}
