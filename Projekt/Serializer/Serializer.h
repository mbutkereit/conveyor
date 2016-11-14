#ifndef SERIALIZER_H_
#define SERIALIZER_H_

#include "Serializer/SerializerInterface.h";

/**
 * @file
 * @section DESCRIPTION
 *
 * Eine Klasse um den Adapter(Ports) zugriff zu Managen.
 */
class Serializer
{
public:
    Serializer();
    ~Serializer();
    std::string serialize();
    std::string unserialize();
};

#endif /* SERIALIZER_H_ */
