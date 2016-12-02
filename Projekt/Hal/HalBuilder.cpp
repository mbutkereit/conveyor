#include "Hal/HalBuilder.h"

Hardware* HalBuilder::instance_ = NULL;

HalBuilder::HalBuilder():adapterA(new Adapter(BASEADRESS_A)), adapterB(new Adapter(BASEADRESS_B)), adapterC(new Adapter(BASEADRESS_C)),adapterD(new Adapter(BASEADRESS_D)){}

HalBuilder::~HalBuilder(){}

Hardware* HalBuilder::buildHardware(){
	return new Hardware(
	        buildHumanMachineInterface(),
	        buildMotor(),
	        buildTrafficLights(),
	        buildMeasuringTool(),
	        buildAltimetry()
	      );
}

Hardware* HalBuilder::getHardware(){
	if( instance_ == NULL){
			mutex.lock();
			if( instance_ == NULL){
				// Initialisierung der Digitalen IO Card (Control Group 0).
			    out8(CONTROL_ADDRESS_0, CONTROL_BITMASK);
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

Altimetry* HalBuilder::buildAltimetry(){
	return new Altimetry(adapterD);
}
