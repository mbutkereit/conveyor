#ifndef TRAFFICLIGHT_H_
#define TRAFFICLIGHT_H_

#include <hw/inout.h>
#include <stdint.h>
#include "Logger/Logger.h"
#include "Hal/Adapter.h"

/**
 * @file
 * @section DESCRIPTION
 *
 * Eine Klasse um die Ampel des Foerderbandes zu Steuern.
 */
class TrafficLight
{

public:

  /**
   *  Constructor der Ampel
   *
   *  @param adapt Adapter für die Steuerung der Ampel.
   */
   TrafficLight(Adapter* adapt);

  /**
   * Destructor der Ampel.
   */
   ~TrafficLight();

  /**
   * Macht die gruene Lampe der Ampel an.
   *
   * @return Gibt Konstant 0 zurueck.
   */
    uint8_t turnGreenOn();

  /**
   * Macht die gruene Lampe der Ampel aus.
   *
   * @return Gibt Konstant 0 zurueck.
   */
    uint8_t turnGreenOff();

   /**
    * Macht die gelbe Lampe der Ampel aus.
    *
    * @return Gibt Konstant 0 zurueck.
    */
    uint8_t turnYellowOn();

   /**
    * Macht die gelbe Lampe der Ampel aus.
    *
    * @return Gibt constant 0 zurueck.
    */
    uint8_t turnYellowOff();

   /**
    * Macht die rote Lampe der Ampel an.
    *
    * @return Gibt Konstant 0 zurueck.
    */
    uint8_t turnRedOn();

   /**
    * Macht die rote Lampe der Ampel aus.
    *
    * @return Gibt Konstant 0 zurueck.
    */
    uint8_t turnRedOff();

private:
    uint8_t redLightBitmask_;    ///< Bitmask für die rote Lampe.
    uint8_t greenLightBitmask_;  ///< Bitmask für die gruene Lampe.
    uint8_t yellowLightBitmask_; ///< Bitmask für die gelbe Lampe.

    Adapter* adapter; ///< Der Adapter mit der Baseadresse.
};

#endif /* TRAFFICLIGHT_H_ */
