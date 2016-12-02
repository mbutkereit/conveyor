#ifndef HUMANMACHINEINTERFACE_H_
#define HUMANMACHINEINTERFACE_H_

#include <hw/inout.h>
#include <stdint.h>
#include "Logger/Logger.h"
#include "Hal/Adapter.h"

/**
 * @file
 * @section DESCRIPTION
 *
 * Eine Klasse um mit den Buttons(Human Machine Interface) zu Kommunizieren.
 */
class HumanMachineInterface
{
public:

    /**
     *  Constructor des HumanMachineInterface.
     */
    HumanMachineInterface(Adapter* adapt);

    /**
     *  Constructor des HumanMachineInterface.
     */
    virtual ~HumanMachineInterface();

    /**
     * Macht die Lampe auf dem Start Button an.
     *
     * @return Gibt Konstant 0 zurueck.
     */
    uint8_t turnLedStartOn();

    /**
     * Macht die Lampe auf dem Start Button aus.
     *
     * @return Gibt Konstant 0 zurueck.
     */
    uint8_t turnLedStartOff();

    /**
     * Macht die Lampe auf dem Reset Button an.
     *
     * @return Gibt Konstant 0 zurueck.
     */
    uint8_t turnLedResetOn();

    /**
     * Macht die Lampe auf dem Reset Button aus.
     *
     * @return Gibt Konstant 0 zurueck.
     */
    uint8_t turnLedResetOff();

    /**
     * Macht die Lampe Q1 an.
     *
     * @return Gibt Konstant 0 zurueck.
     */
    uint8_t turnLedQ1On();

    /**
     * Macht die Lampe Q1 aus.
     *
     * @return Gibt Konstant 0 zurueck.
     */
    uint8_t turnLedQ1Off();

    /**
     * Macht die Lampe Q2 an.
     *
     * @return Gibt Konstant 0 zurueck.
     */
    uint8_t turnLedQ2On();

    /**
     * Macht die Lampe Q1 aus.
     *
     * @return Gibt Konstant 0 zurueck.
     */
    uint8_t turnLedQ2Off();

    /**
     *  Checkt ob der Button Start gedrueckt wird.
     *
     * @return Gibt 0 zurueck bei FALSE ansonsten ein Wert >0.
     */
    uint8_t isButtonStartPressed();

    /**
     *  Checkt ob der Button Stop gedrueckt wird.
     *
     * @return Gibt 0 zurueck bei FALSE ansonsten ein Wert >0.
     */
    uint8_t isButtonStopPressed();

    /**
     *  Checkt ob der Button Reset gedrueckt wird.
     *
     * @return Gibt 0 zurueck bei FALSE ansonsten ein Wert >0.
     */
    uint8_t isButtonResetPressed();

    /**
     *  Checkt ob der Button EStop gedrueckt wird.
     *
     * @return Gibt 0 zurueck bei FALSE ansonsten ein Wert >0.
     */
    uint8_t isButtonEStopPressed();

private:
    uint8_t hmiLedStartBitmask_; ///< Bitmask um die Led auf den Start Button zu Kontrollieren.
    uint8_t hmiLedResetBitmask_; ///< Bitmask um die Led auf den Reset Button zu Kontrollieren.
    uint8_t hmiLedQ1Bitmask_; ///<  Bitmask um die Led Q1 zu Kontrollieren.
    uint8_t hmiLedQ2Bitmask_; ///<  Bitmask um die Led Q2zu Kontrollieren.
    uint8_t hmiButtonStartBitmask_; ///<  Bitmask um den Zustand von dem Start Button zu erfahren.
    uint8_t hmiButtonStopBitmask_; ///< Bitmask um den Zustand von dem Stop Button zu erfahren.
    uint8_t hmiButtonResetBitmask_; ///< Bitmask um den Zustand von dem Reset Button zu erfahren.
    uint8_t hmiButtonEStopBitmask_; ///< Bitmask um den Zustand von dem EStop Button zu erfahren.

    Adapter* adapter; ///< Der Adapter mit der Baseadresse.
};
#endif /* HUMANMACHINEINTERFACE_H_ */
