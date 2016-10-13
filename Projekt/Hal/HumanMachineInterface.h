/*
 * HumanMachineInterface.h
 *
 *  Created on: 11.10.2016
 *      Author: marvin
 */

#ifndef HUMANMACHINEINTERFACE_H_
#define HUMANMACHINEINTERFACE_H_
#include <hw/inout.h>
#include <stdint.h>
#include "Logger/Logger.h"
#include "Hal/Adapter.h"

class HumanMachineInterface
{
public:
    HumanMachineInterface(Adapter* adapt);
    virtual ~HumanMachineInterface();

    uint8_t turnLedStartOn();
    uint8_t turnLedStartOff();
    uint8_t turnLedResetOn();
    uint8_t turnLedResetOff();
    uint8_t turnLedQ1On();
    uint8_t turnLedQ1Off();
    uint8_t turnLedQ2On();
    uint8_t turnLedQ2Off();

    uint8_t isButtonStartPressed();
    uint8_t isButtonStopPressed();
    uint8_t isButtonResetPressed();
    uint8_t isButtonEStopPressed();

private:
    //todo adresse finden.
    uint16_t registerAddress_;
    uint8_t hmiLedStartBitmask_;
    uint8_t hmiLedResetBitmask_;
    uint8_t hmiLedQ1Bitmask_;
    uint8_t hmiLedQ2Bitmask_;
    uint8_t hmiButtonStartBitmask_;
    uint8_t hmiButtonStopBitmask_;
    uint8_t hmiButtonResetBitmask_;
    uint8_t hmiButtonEStopBitmask_;
    Adapter* adapter;
};
#endif /* HUMANMACHINEINTERFACE_H_ */
