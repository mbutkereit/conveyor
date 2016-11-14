/*
 * MeasuringTool.h
 *
 *  Created on: 13.10.2016
 *      Author: abx827
 */

#ifndef MEASURINGTOOL_H_
#define MEASURINGTOOL_H_

#include <hw/inout.h>
#include <stdint.h>
#include "Logger/Logger.h"
#include "Hal/Adapter.h"

class MeasuringTool {
public:
	MeasuringTool(Adapter* adapt);
	virtual ~MeasuringTool();

    /**
     * Oeffnet den .
     *
     * @return Gibt Konstant 0 zurueck.
     */
    uint8_t isItemRunningIn();

    /**
     * Oeffnet den .
     *
     * @return Gibt Konstant 0 zurueck.
     */
    uint8_t isItemAltimetry();

    /**
     * Oeffnet den .
     *
     * @return Gibt Konstant 0 zurueck.
     */
    uint8_t isItemInAltimetryToleranceRange();

    /**
     * Oeffnet den .
     *
     * @return Gibt Konstant 0 zurueck.
     */
    uint8_t isItemSwitch();

    /**
     * Oeffnet den .
     *
     * @return Gibt Konstant 0 zurueck.
     */
    uint8_t isItemMetal();

    /**
     * Oeffnet den .
     *
     * @return Gibt Konstant 0 zurueck.
     */
    uint8_t isSwitchOpen();

    /**
     * Oeffnet den .
     *
     * @return Gibt Konstant 0 zurueck.
     */
    uint8_t isSkidFull();

    /**
     * Oeffnet den .
     *
     * @return Gibt Konstant 0 zurueck.
     */
    uint8_t isItemRunningOut();

private:
    uint8_t itemRunningBitmask__; ///<
    uint8_t itemAltimetryBitmask_; ///<
    uint8_t itemInAltimetryToleranceRangeBitmask_; ///<
    uint8_t itemSwitchBitmask_; ///<
    uint8_t itemMetalBitmask_; ///<
    uint8_t switchOpenBitmask_; ///<
    uint8_t skidFullBitmask_; ///<
    uint8_t itemRunningOutBitmask_; ///<

    Adapter* adapter; ///< Der Adapter mit der Baseadresse.
};

#endif /* MEASURINGTOOL_H_ */
