/*
 * MessageTokenRing.h
 *
 *  Created on: 23.10.2016
 *      Author: marvin


#ifndef MESSAGETOKENRING_H_
#define MESSAGETOKENRING_H_

#include <Hal/SerialInterface/Serial.h>
#include <Hal/SerialInterface/MessageTokenRing.h>
#include <Serializer/Serializer.h>
//#include <Messages/RingMessage.h>
#include <map>

class MessageTokenRing
{
private:
    Serial* input;
    Serial* output;

    Serializer* serializer;


public:
    MessageTokenRing(Serial* input,Serial* output);
  //  Message getMessage();
    uint8_t sendMessage(RingMessage* message);
    uint8_t receiveMessage();
    virtual ~MessageTokenRing();
};

#endif
 */
