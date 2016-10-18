/*
 * Adapter.cpp
 *
 *  Created on: 09.10.2016
 *      Author: marvin
 */

#include <Hal/Adapter.h>


Adapter::Adapter(uint16_t base):baseaddress(base) {
	synchronizer = new Mutexo();

}

Adapter::~Adapter() {
}


void Adapter::setBitMask(uint8_t bitmask){
	synchronizer->lock();
	out8(baseaddress, in8(baseaddress) | bitmask);
	synchronizer->unlock();
}

void Adapter::removeBitMask(uint8_t bitmask){
	synchronizer->lock();
	out8(baseaddress, in8(baseaddress) & ~bitmask);
	synchronizer->unlock();
}

uint8_t Adapter::checkBitMask(uint8_t bitmask){
	synchronizer->lock();
	uint8_t result;
	result = (in8(baseaddress) &  bitmask);
	synchronizer->unlock();
	return result;
}
