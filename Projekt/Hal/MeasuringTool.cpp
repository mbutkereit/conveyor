#include "MeasuringTool.h"

MeasuringTool::MeasuringTool(Adapter* adapt):adapter(adapt) {
	// TODO Auto-generated constructor stub

}

MeasuringTool::~MeasuringTool() {

	itemRunningBitmask__ = 0x01;
	itemAltimetryBitmask_ = 0x02;
	itemInAltimetryToleranceRangeBitmask_ = 0x04;
	itemSwitchBitmask_ = 0x08;
	itemMetalBitmask_ = 0x10;
	switchOpenBitmask_ = 0x20;
	skidFullBitmask_ = 0x40;
	itemRunningOutBitmask_ = 0x80;
}

uint8_t MeasuringTool::isItemRunningIn() {
	LOG_DEBUG << "Ist Werkstueck im Einlauf?";
	return adapter->checkBitMask(itemRunningBitmask__);
}
;
uint8_t MeasuringTool::isItemAltimetry() {
	LOG_DEBUG << "Ist Werkstueck in Hoehenmessung";
	return adapter->checkBitMask(itemAltimetryBitmask_);
}
;
uint8_t MeasuringTool::isItemInAltimetryToleranceRange() {
	LOG_DEBUG << "Ist Werkstueck im Toleranzbereich?";
	return adapter->checkBitMask(itemInAltimetryToleranceRangeBitmask_);
}
;
uint8_t MeasuringTool::isItemSwitch() {
	LOG_DEBUG << "Ist Werkstueck in Weiche?";
	return adapter->checkBitMask(itemSwitchBitmask_);
}
;
uint8_t MeasuringTool::isItemMetal() {
	LOG_DEBUG << "Ist Werkstueck aus Metall?";
	return adapter->checkBitMask(itemMetalBitmask_);
}
;
uint8_t MeasuringTool::isSwitchOpen() {
	LOG_DEBUG << "Ist Weiche offen?";
	return adapter->checkBitMask(switchOpenBitmask_);
}
;
uint8_t MeasuringTool::isSkidFull() {
	LOG_DEBUG << "Ist Rutsche voll?";
	return adapter->checkBitMask(skidFullBitmask_);
}
;
uint8_t MeasuringTool::isItemRunningOut() {
	LOG_DEBUG << "Ist Werkstueck im Auslauf";
	return adapter->checkBitMask(itemRunningOutBitmask_);
}
;
