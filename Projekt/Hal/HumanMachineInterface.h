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

    /**
     * Oeffnet den .
     *
     * @return Gibt Konstant 0 zurueck.
     */
    uint8_t turnLedStartOn();

    /**
     * Oeffnet den .
     *
     * @return Gibt Konstant 0 zurueck.
     */
    uint8_t turnLedStartOff();

    /**
     * Oeffnet den .
     *
     * @return Gibt Konstant 0 zurueck.
     */
    uint8_t turnLedResetOn();

    /**
     * Oeffnet den .
     *
     * @return Gibt Konstant 0 zurueck.
     */
    uint8_t turnLedResetOff();

    /**
     * Oeffnet den .
     *
     * @return Gibt Konstant 0 zurueck.
     */
    uint8_t turnLedQ1On();

    /**
     * Oeffnet den .
     *
     * @return Gibt Konstant 0 zurueck.
     */
    uint8_t turnLedQ1Off();

    /**
     * Oeffnet den .
     *
     * @return Gibt Konstant 0 zurueck.
     */
    uint8_t turnLedQ2On();

    /**
     * Oeffnet den .
     *
     * @return Gibt Konstant 0 zurueck.
     */
    uint8_t turnLedQ2Off();

    /**
     * Oeffnet den .
     *
     * @return Gibt Konstant 0 zurueck.
     */
    uint8_t isButtonStartPressed();

    /**
     * Oeffnet den .
     *
     * @return Gibt Konstant 0 zurueck.
     */
    uint8_t isButtonStopPressed();

    /**
     * Oeffnet den .
     *
     * @return Gibt Konstant 0 zurueck.
     */
    uint8_t isButtonResetPressed();

    /**
     * Oeffnet den .
     *
     * @return Gibt Konstant 0 zurueck.
     */
    uint8_t isButtonEStopPressed();

private:
    uint8_t hmiLedStartBitmask_; ///<
    uint8_t hmiLedResetBitmask_; ///<
    uint8_t hmiLedQ1Bitmask_; ///<
    uint8_t hmiLedQ2Bitmask_; ///<
    uint8_t hmiButtonStartBitmask_; ///<
    uint8_t hmiButtonStopBitmask_; ///<
    uint8_t hmiButtonResetBitmask_; ///<
    uint8_t hmiButtonEStopBitmask_; ///<

    Adapter* adapter; ///< Der Adapter mit der Baseadresse.
};
#endif /* HUMANMACHINEINTERFACE_H_ */
