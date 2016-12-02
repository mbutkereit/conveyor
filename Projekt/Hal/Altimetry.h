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
 * Eine Klasse um die Hoehenmessung des Foerderbandes zu Steuern.
 */
class Altimetry {
public:

    /**
     *  Constructor der Altimetry.
     *
     *  @param adapt Adapter für die Steuerung der Ampel.
     */
	Altimetry(Adapter* adapt);

	  /**
	   *  Destructor der Altimetry.
	   */
	virtual ~Altimetry();

	/**
	     * Gibt die aktuell berechnete hoehe zurueck.
	     *(Dieses ist ein uint16_t wert hat aber nur maximal 12 bit gesetzt).
	     *
	     * @return Gibt den Wert aus der Letzten Hoehenmessung zurueck.
	     */
	uint16_t getHeight();

	/**
	 * Gibt die Berechnung der Hoehe frei welche
	 * wenn das Ergebnis berechnet ist eine
	 * Pulse Message an den SignalHandler schickt.
	 */
	void startAltimetry();
private:
    uint8_t readLowAddressOffset;  ///< Addresse um die Niederwertigen Anteil des Ergebenises aus zu lesen.
    uint8_t readHighAddressOffset; ///< Addresse um den Hoeherwertigen Anteil des Ergebenises aus zu lesen.
    uint8_t	startConversionOpCode; ///< Befehl um die Hoehenmessung zu starten.
    uint16_t only12BitBitmask;     ///< Helfer Bitmask um die Ueberfluessigen Bits zu entfernen. (Besser auswertbare Ergebnise)

    Adapter* adapter;              ///< Der Adapter mit der Baseadresse.
};

#endif /* ALTIMETRY_H_ */
