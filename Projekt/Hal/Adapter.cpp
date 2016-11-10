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

void Adapter::forceBitmask(uint8_t bitmask,uint16_t offset){
	synchronizer->lock();
	out8(baseaddress+offset, bitmask);
	synchronizer->unlock();
}

uint8_t Adapter::checkBitMask(uint8_t bitmask){
	uint8_t result;
	result = (in8(baseaddress) &  bitmask);
	return result;
}


uint8_t Adapter::getValuefromAddress(uint8_t offset){
	return in8(baseaddress + offset);
}
