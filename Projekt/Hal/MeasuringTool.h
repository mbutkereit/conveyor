#ifndef MEASURINGTOOL_H_
#define MEASURINGTOOL_H_

#include <hw/inout.h>
#include <stdint.h>
#include "Logger/Logger.h"
#include "Hal/Adapter.h"

/**
 * @file
 * @section DESCRIPTION
 *
 * Eine Klasse um den Zustand der Messinstrumente auszulesen.
 */
class MeasuringTool {
public:

    /**
     *  Constructor des MeasuringTools.
     *
     *  @param adapt Adapter für die Steuerung des Measuring Tools.
     */
	MeasuringTool(Adapter* adapt);

	  /**
	   * Destructor der Ampel.
	   */
	virtual ~MeasuringTool();

    /**
     *  Checkt ob ein Item aktuell die Anfangs LS unterbricht.
     *
     * @return Gibt 0 zurueck bei FALSE ansonsten ein Wert >0.
     */
    uint8_t isItemRunningIn();

    /**
     *  Checkt ob ein Item aktuell die Hoehenmessungs LS unterbricht.
     *
     * @return Gibt 0 zurueck bei FALSE ansonsten ein Wert >0.
     */
    uint8_t isItemAltimetry();

    /**
     *  Checkt ob ein Item in der Hoehenmessung im Toleranz berreich ist.
     *
     *  0 = Werkstück zu klein oder zu groß
     *  1 = Werkstückhöhe ist im Toleranzbereich
     *
     * @return Gibt 0 zurueck bei FALSE ansonsten ein Wert >0.
     */
    uint8_t isItemInAltimetryToleranceRange();

    /**
     *  Checkt ob ein Item aktuell die Weichen LS unterbricht.
     *
     * @return Gibt 0 zurueck bei FALSE ansonsten ein Wert >0.
     */
    uint8_t isItemSwitch();

    /**
     *  Checkt ob das Item unter dem ... aus Metal ist.
     *
     * @return Gibt 0 zurueck bei FALSE ansonsten ein Wert >0.
     */
    uint8_t isItemMetal();

    /**
     *  Ueberprueft ob die Weiche offen ist.
     *
     * @return Gibt 0 zurueck bei FALSE ansonsten ein Wert >0.
     */
    uint8_t isSwitchOpen();

    /**
     *  Checkt ob ein Item aktuell die Rutschen LS unterbricht.
     *
     * @return Gibt 0 zurueck bei FALSE ansonsten ein Wert >0.
     */
    uint8_t isSkidFull();

    /**
     *  Checkt ob ein Item aktuell die End LS unterbricht.
     *
     * @return Gibt 0 zurueck bei FALSE ansonsten ein Wert >0.
     */
    uint8_t isItemRunningOut();

private:
    uint8_t itemRunningBitmask_; ///< Bitmask um herauszufinde ob ein Item in der Anfangs LR ist.
    uint8_t itemAltimetryBitmask_; ///< Bitmask um herauszufinde ob ein Item in der Hoehenmessung ist.
    uint8_t itemInAltimetryToleranceRangeBitmask_; ///< Bitmask um herauszufinde ob ein Item in der Hoehenmessung im Toleranzbereich ist.
    uint8_t itemSwitchBitmask_; ///< Bitmask um herauszufinde ob ein Item in der Weiche ist.
    uint8_t itemMetalBitmask_; ///< Bitmask um herauszufinde ob das Item aus Metal ist.
    uint8_t switchOpenBitmask_; ///< Bitmask um herauszufinde ob die Weiche offen ist.
    uint8_t skidFullBitmask_; ///< Bitmask um herauszufinde ob die Rutsche voll ist
    uint8_t itemRunningOutBitmask_; ///< Bitmask um herauszufinde ob ein Item die letzte LS unterbricht.

    Adapter* adapter; ///< Der Adapter mit der Baseadresse.
};

#endif /* MEASURINGTOOL_H_ */
