/*
 * Altimetry.h
 *
 *  Created on: 10.11.2016
 *      Author: abx827
 */

#ifndef ALTIMETRY_H_
#define ALTIMETRY_H_

#include <hw/inout.h>
#include <stdint.h>
#include "Logger/Logger.h"
#include "Hal/Adapter.h"

class Altimetry {
public:
	Altimetry(Adapter* adapt);
	virtual ~Altimetry();
	uint16_t getHeight();
	void startAltimetry();
private:
    uint8_t readLowAddressOffset;
    uint8_t readHighAddressOffset;
    uint8_t	startConversionOpCode;
    uint16_t only12BitBitmask;

    Adapter* adapter;
};

#endif /* ALTIMETRY_H_ */
