/*
 * Altimetry.h
 *
 *  Created on: 10.11.2016
 *      Author: abx827
 */

#ifndef ALTIMETRY_H_
#define ALTIMETRY_H_

#include <hw/inout.h>
#include <stdint.h>
#include "Logger/Logger.h"
#include "Hal/Adapter.h"

#define ACTIVATE_ADC_IN 0b10000100 ///< Befehl um die Interrupts bei dem ADC zu aktivieren.
#define ADC_IN_OFFSET 0x01 ///< Offset zu dem ADC Config Register.

/**
 * @file
 * @section DESCRIPTION
 *
 * Eine Klasse um die Ampel des Foerderbandes zu Steuern.
 */
class Altimetry {
public:

    /**
     *  Constructor der Ampel
     *
     *  @param adapt Adapter für die Steuerung der Ampel.
     */
	Altimetry(Adapter* adapt);

	  /**
	   *  Constructor der Ampel
	   *
	   *  @param adapt Adapter für die Steuerung der Ampel.
	   */
	virtual ~Altimetry();

	/**
	     * Oeffnet den .
	     *
	     * @return Gibt Konstant 0 zurueck.
	     */
	uint16_t getHeight();

	/**
	 * Oeffnet den .
	 *
	 * @return Gibt Konstant 0 zurueck.
	 */
	void startAltimetry();
private:
    uint8_t readLowAddressOffset;  ///<
    uint8_t readHighAddressOffset; ///<
    uint8_t	startConversionOpCode; ///<
    uint16_t only12BitBitmask;     ///<

    Adapter* adapter;              ///< Der Adapter mit der Baseadresse.
};

#endif /* ALTIMETRY_H_ */
