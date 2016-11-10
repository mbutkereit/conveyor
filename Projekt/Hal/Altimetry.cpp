/*
 * Altimetry.cpp
 *
 *  Created on: 10.11.2016
 *      Author: abx827
 */

#include "Altimetry.h"

Altimetry::Altimetry(Adapter* adapt):adapter(adapt) {
	readLowAddressOffset=0x02;
	readHighAddressOffset=0x03;
	startConversionOpCode=0x10;
	only12BitBitmask = 0x0FFF;
}

Altimetry::~Altimetry() {
	// TODO Auto-generated destructor stub
}

uint16_t Altimetry::getHeight(){
	uint16_t height=0;
	height = adapter->getValuefromAddress(readLowAddressOffset);
	height |= (uint16_t)adapter->getValuefromAddress(readHighAddressOffset) << 8;
	height &= only12BitBitmask;
	return height;
}

void Altimetry::startAltimetry(){
	adapter->forceBitmask(0b10000100,0x01);
	adapter->forceBitmask(startConversionOpCode,readLowAddressOffset);
}
