#ifndef MOTOR_H_
#define MOTOR_H_

#include <hw/inout.h>
#include <stdint.h>
#include "Logger/Logger.h"
#include "Hal/Adapter.h"

/**
 * @file
 * @section DESCRIPTION
 *
 * Eine Klasse um den Motor des Foerderbandes an zu sprechen.
 */
class Motor
{
public:

    /**
     *  Constructor des Motors.
     *
   *  @param adapt Adapter für die Steuerung des Motors.
     */
    Motor(Adapter* adapt);

    /**
     *  Destructor des Motors.
     */
    virtual ~Motor();

    /**
     * Laesst den Motor in die Rechte richtung fahren.
     *
     * @return Gibt Konstant 0 zurueck.
     */
    uint8_t right();

    /**
     * Laesst den Motor in die Linke richtung fahren.
     *
     * @return Gibt Konstant 0 zurueck.
     */
    uint8_t left();

    /**
     * Laesst den Motor langsam fahren.
     *
     * @return Gibt Konstant 0 zurueck.
     */
    uint8_t slow();

    /**
     * Laesst den Motor schnell fahren.
     *
     * @return Gibt Konstant 0 zurueck.
     */
    uint8_t fast();

    /**
     * Stoppt den Motor.
     *
     * @return Gibt Konstant 0 zurueck.
     */
    uint8_t stop();

    /**
     * Oeffnet die Weiche.
     *
     * @return Gibt Konstant 0 zurueck.
     */
    uint8_t switchOpen();

    /**
     * Schliesst die Weiche.
     *
     * @return Gibt Konstant 0 zurueck.
     */
    uint8_t switchClosed();

private:
    uint8_t motorrightBitmask__;  ///< Bitmask um den Motor nach Rechtsfahren zu lassen.
    uint8_t motorleftBitmask_;    ///< Bitmask um den Motor nach Linksfahren zu lassen.
    uint8_t motorstoppBitmask_;   ///< Bitmask die den Motor anhalten laesst.
    uint8_t motorlangsamBitmask_; ///< Bitmask um den Motor langsam zu machen.
    uint8_t weicheaufBitmask_;    ///< Bitmask um die Weiche auf zu machen.

    Adapter* adapter;             ///< Der Adapter mit der Baseadresse.
};

#endif /* MOTOR_H_ */
