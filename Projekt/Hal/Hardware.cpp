#include "Hal/Hardware.h"

Hardware::Hardware(HumanMachineInterface* hmi, Motor* motor,TrafficLight* tl, MeasuringTool* mt, Altimetry* am, Serial* serial)
{
	this->hmi = hmi;
	this->motor = motor;
	this->tl = tl;
	this->mt = mt;
	this->am = am;
	this->serial = serial;
}

Hardware::~Hardware(){}

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

Altimetry* Hardware::getAltimetry(){
	return am;
}

Serial* Hardware::getSerial(){
	return serial;
}

