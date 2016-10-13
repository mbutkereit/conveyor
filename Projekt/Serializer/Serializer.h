/*
 * Serializer.h
 *
 *  Created on: 05.10.2016
 *      Author: marvin
 */

#ifndef SERIALIZER_H_
#define SERIALIZER_H_

#include "Serializer/SerializerInterface.h";

class Serializer
{
public:
    Serializer();
    ~Serializer();
    std::string serialize();
    std::string unserialize();
};

#endif /* SERIALIZER_H_ */
