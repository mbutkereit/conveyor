#include "MeasuringTool.h"

MeasuringTool::MeasuringTool(Adapter* adapt):adapter(adapt) {
	itemRunningBitmask_ = 0x01;
	itemAltimetryBitmask_ = 0x02;
	itemInAltimetryToleranceRangeBitmask_ = 0x04;
	itemSwitchBitmask_ = 0x08;
	itemMetalBitmask_ = 0x10;
	switchOpenBitmask_ = 0x20;
	skidFullBitmask_ = 0x40;
	itemRunningOutBitmask_ = 0x80;
}

MeasuringTool::~MeasuringTool() {
}

uint8_t MeasuringTool::isItemRunningIn() {
	return adapter->checkBitMask(itemRunningBitmask_);
}
;
uint8_t MeasuringTool::isItemAltimetry() {
	return adapter->checkBitMask(itemAltimetryBitmask_);
}
;
uint8_t MeasuringTool::isItemInAltimetryToleranceRange() {
	return adapter->checkBitMask(itemInAltimetryToleranceRangeBitmask_);
}
;
uint8_t MeasuringTool::isItemSwitch() {
	return adapter->checkBitMask(itemSwitchBitmask_);
}
;
uint8_t MeasuringTool::isItemMetal() {
	return adapter->checkBitMask(itemMetalBitmask_);
}
;
uint8_t MeasuringTool::isSwitchOpen() {
	return adapter->checkBitMask(switchOpenBitmask_);
}
;
uint8_t MeasuringTool::isSkidFull() {
	return adapter->checkBitMask(skidFullBitmask_);
}
;
uint8_t MeasuringTool::isItemRunningOut() {
	return adapter->checkBitMask(itemRunningOutBitmask_);
}
;
