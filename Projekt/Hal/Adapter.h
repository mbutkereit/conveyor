/*
 * Adapter.h
 *
 *  Created on: 09.10.2016
 *      Author: marvin
 */

#ifndef ADAPTER_H_
#define ADAPTER_H_

#include <stdint.h>
#include <Hal/Mutexo.h>
#include <hw/inout.h>
#include <stdint.h>



class Adapter
{
public:
    Adapter(uint16_t);
    ~Adapter();
    void setBitMask(uint8_t bitmask);
    void removeBitMask(uint8_t bitmask);
    uint8_t checkBitMask(uint8_t bitmask);



private:
    Mutexo* synchronizer;
    uint16_t baseaddress;
};

#endif /* ADAPTER_H_ */
