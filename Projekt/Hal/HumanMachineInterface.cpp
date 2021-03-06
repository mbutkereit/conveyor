#include <HumanMachineInterface.h>

HumanMachineInterface::HumanMachineInterface(Adapter* adapt):adapter(adapt) {
	hmiLedStartBitmask_ = 0x01;
	hmiLedResetBitmask_ = 0x02;
	hmiLedQ1Bitmask_ = 0x04;
	hmiLedQ2Bitmask_ = 0x08;
	hmiButtonStartBitmask_ = 0x10;
	hmiButtonStopBitmask_ = 0x20;
	hmiButtonResetBitmask_ = 0x40;
	hmiButtonEStopBitmask_ = 0x80;
}

HumanMachineInterface::~HumanMachineInterface() {
}

uint8_t HumanMachineInterface::turnLedStartOn() {
	adapter->setBitMask(hmiLedStartBitmask_);
	LOG_DEBUG << "Led Start an.";
	return 0;

}

uint8_t HumanMachineInterface::turnLedStartOff() {
	adapter->removeBitMask(hmiLedStartBitmask_);
	LOG_DEBUG << "Led Start aus.";
	return 0;
}

uint8_t HumanMachineInterface::turnLedResetOn() {
	adapter->setBitMask(hmiLedResetBitmask_);
	LOG_DEBUG << "Led Start an.";
	return 0;
}

uint8_t HumanMachineInterface::turnLedResetOff() {
	adapter->removeBitMask(hmiLedResetBitmask_);
	LOG_DEBUG << "Led Reset aus.";
	return 0;

}

uint8_t HumanMachineInterface::turnLedQ1On() {
	adapter->setBitMask(hmiLedQ1Bitmask_);
	LOG_DEBUG << "Led Q10 an.";
	return 0;
}

uint8_t HumanMachineInterface::turnLedQ1Off() {
	adapter->removeBitMask(hmiLedQ1Bitmask_);
	LOG_DEBUG << "Led Q10 aus.";
	return 0;
}

uint8_t HumanMachineInterface::turnLedQ2On() {
	adapter->setBitMask(hmiLedQ2Bitmask_);
	LOG_DEBUG << "Led Q20 an.";
	return 0;
}

uint8_t HumanMachineInterface::turnLedQ2Off() {
	adapter->removeBitMask(hmiLedQ2Bitmask_);
	LOG_DEBUG << "Led Q20 aus.";
	return 0;
}

uint8_t HumanMachineInterface::isButtonStartPressed() {
	return adapter->checkBitMask(hmiButtonStartBitmask_);
}

uint8_t HumanMachineInterface::isButtonStopPressed() {
	return adapter->checkBitMask(hmiButtonStopBitmask_);
}

uint8_t HumanMachineInterface::isButtonResetPressed(){
	return adapter->checkBitMask(hmiButtonResetBitmask_);
}

uint8_t HumanMachineInterface::isButtonEStopPressed(){
	return adapter->checkBitMask(hmiButtonEStopBitmask_);
}
