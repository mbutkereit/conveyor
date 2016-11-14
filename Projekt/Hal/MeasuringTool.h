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
 * Eine Klasse um den Motor des Foerderbandes an zu sprechen.
 */
class MeasuringTool {
public:

    /**
     *  Constructor des MeasuringTools.
     */
	MeasuringTool(Adapter* adapt);

	  /**
	   * Destructor der Ampel.
	   */
	virtual ~MeasuringTool();

    /**
     * Oeffnet den .
     *
     * @return Gibt 0 zurueck bei FALSE ansonsten ein Wert >0.
     */
    uint8_t isItemRunningIn();

    /**
     * Oeffnet den .
     *
     * @return Gibt 0 zurueck bei FALSE ansonsten ein Wert >0.
     */
    uint8_t isItemAltimetry();

    /**
     * Oeffnet den .
     *
     * @return Gibt 0 zurueck bei FALSE ansonsten ein Wert >0.
     */
    uint8_t isItemInAltimetryToleranceRange();

    /**
     * Oeffnet den .
     *
     * @return Gibt 0 zurueck bei FALSE ansonsten ein Wert >0.
     */
    uint8_t isItemSwitch();

    /**
     * Oeffnet den .
     *
     * @return Gibt 0 zurueck bei FALSE ansonsten ein Wert >0.
     */
    uint8_t isItemMetal();

    /**
     * Oeffnet den .
     *
     * @return Gibt 0 zurueck bei FALSE ansonsten ein Wert >0.
     */
    uint8_t isSwitchOpen();

    /**
     * Oeffnet den .
     *
     * @return Gibt 0 zurueck bei FALSE ansonsten ein Wert >0.
     */
    uint8_t isSkidFull();

    /**
     *  .
     *
     * @return Gibt 0 zurueck bei FALSE ansonsten ein Wert >0.
     */
    uint8_t isItemRunningOut();

private:
    uint8_t itemRunningBitmask__; ///< Bitmask um herauszufinde ob ein Item in der Anfangs LR ist.
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
