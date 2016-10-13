/*
 * HalBuilder.cpp
 *
 *  Created on: 12.10.2016
 *      Author: marvin
 */

#include <HalBuilder.h>

Hardware* HalBuilder::instance_ = NULL;

HalBuilder::HalBuilder():adapterA(new Adapter(BASEADRESS_A)), adapterB(new Adapter(BASEADRESS_B)), adapterC(new Adapter(BASEADRESS_C))
{

}

HalBuilder::~HalBuilder()
{
    // TODO Auto-generated destructor stub
}

Hardware* HalBuilder::buildHardware(){
	return new Hardware(buildHumanMachineInterface(), buildMotor(), buildTrafficLights(), buildMeasuringTool());
}

Hardware* HalBuilder::getHardware(){
	if( instance_ == NULL){
			mutex.lock();
			if( instance_ == NULL){
			    instance_ = buildHardware();
			}
			mutex.unlock();
		}
		return instance_;
}

HumanMachineInterface* HalBuilder::buildHumanMachineInterface(){
	return new HumanMachineInterface(adapterC);
}

Motor* HalBuilder::buildMotor(){
	return new Motor(adapterA);
}

TrafficLight* HalBuilder::buildTrafficLights(){
	return new TrafficLight(adapterA);
}

MeasuringTool* HalBuilder::buildMeasuringTool(){
	return new MeasuringTool(adapterB);
}




