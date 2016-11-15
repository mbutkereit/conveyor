#include "Context.h"

Context::Context() {}

Context::~Context() {}

void Context::signalLBBeginInterrupted(){
	LOG_DEBUG  << "Signal Anfang!!" <<std::endl;
}

void Context::signalLBBeginNotInterrupted(){
	LOG_DEBUG  << "Signal Anfang nicht mehr unterbrochen!!" <<std::endl;
}

void Context::signalLBEndInterrupted()  {
	LOG_DEBUG  << "Signal Hinten!!" <<std::endl;
}
void Context::signalLBEndNotInterrupted()  {
	LOG_DEBUG  << "Signal Hinten nicht mehr unterbrochen!!" <<std::endl;
}

void Context::signalLBAltimetryInterrupted(){
	hb.getHardware()->getAltimetry()->startAltimetry();
	LOG_DEBUG  << "Signal Hoehenmessung!!" <<std::endl;
}
void Context::signalLBAltimetryNotInterrupted(){
	LOG_DEBUG  << "Signal Hoehenmessung nicht mehr unterbrochen!!" <<std::endl;
}


void Context::signalLBSwitchInterrupted(){
	LOG_DEBUG  << "Ist Metal ? " << (int)hb.getHardware()->getMT()->isItemMetal()  <<std::endl;
	LOG_DEBUG  << "Signal Weiche!!" <<std::endl;
}
void Context::signalLBSwitchNotInterrupted(){
	LOG_DEBUG  << "Signal Weiche nicht mehr unterbrochen!!" <<std::endl;
}

void Context::signalEStop(){
	LOG_DEBUG  << "Signal Estop!!" <<std::endl;
}
void Context::signalStart(){
	LOG_DEBUG  << "Signal Start!!" <<std::endl;
}
void Context::signalStop(){
	LOG_DEBUG  << "Signal Stop!!" <<std::endl;
}
void Context::signalReset(){
	LOG_DEBUG  << "Signal Reset!!" <<std::endl;
}

void Context::signalLBSkidInterrupted(){
	LOG_DEBUG  << "Signal Rutsche!!" <<std::endl;
}
void Context::signalLBSkidNotInterrupted(){
	LOG_DEBUG  << "Signal Rutsche nicht mehr unterbrochen!!" <<std::endl;
}

void Context::signalAltimetryCompleted(){
    LOG_DEBUG  << "Hoehenmessung: " << hb.getHardware()->getAltimetry()->getHeight() << "\n"  <<std::endl;
	LOG_DEBUG  << "Signal Hoehenmessung ist fertig.!!" <<std::endl;
}
