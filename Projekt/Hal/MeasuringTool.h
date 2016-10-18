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

    uint8_t isItemRunningIn();
    uint8_t isItemAltimetry();
    uint8_t isItemInAltimetryToleranceRange();
    uint8_t isItemSwitch();
    uint8_t isItemMetal();
    uint8_t isSwitchOpen();
    uint8_t isSkidFull();
    uint8_t isItemRunningOut();

private:
    uint8_t registerAddress_;
    uint8_t itemRunningBitmask__;
    uint8_t itemAltimetryBitmask_;
    uint8_t itemInAltimetryToleranceRangeBitmask_;
    uint8_t itemSwitchBitmask_;
    uint8_t itemMetalBitmask_;
    uint8_t switchOpenBitmask_;
    uint8_t skidFullBitmask_;
    uint8_t itemRunningOutBitmask_;

    Adapter* adapter;
};

#endif /* MEASURINGTOOL_H_ */
