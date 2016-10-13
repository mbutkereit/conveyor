/*
 * Motor.h
 *
 *  Created on: 11.10.2016
 *      Author: marvin
 */

#ifndef MOTOR_H_
#define MOTOR_H_
#include <hw/inout.h>
#include <stdint.h>
#include "Logger/Logger.h"
#include "Hal/Adapter.h"

class Motor
{
public:
    Motor(Adapter* adapt);
    virtual ~Motor();

    uint8_t right();
    uint8_t left();
    uint8_t slow();
    uint8_t fast();
    uint8_t stop();
    uint8_t switchOpen();
    uint8_t switchClosed();

private:
    uint16_t registerAddress_;
    uint8_t motorrightBitmask__;
    uint8_t motorleftBitmask_;
    uint8_t motorstoppBitmask_;
    uint8_t motorlangsamBitmask_;
    uint8_t weicheaufBitmask_;

    Adapter* adapter;
};

#endif /* MOTOR_H_ */
