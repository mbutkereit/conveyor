#ifndef WORKPIECE_MESSAGE_H_
#define WORKPIECE_MESSAGE_H_

#include <stdint.h>
#include "Serializer/Serializer.h";

#define BAND 1

class WorkpieceMessage
{
public:
    static WorkpieceMessage& getWorkpieceMessage()
    {
        static WorkpieceMessage instances;
        return instances;
    }
    void send(uint16_t hohehnmesswert_1 ,uint16_t hohehnmesswert_2,uint8_t typ ,uint16_t id );
    void enqueue(struct workpiece_package_without_ch  element);
    void dequeue();

private:
   struct info_package info;
   WorkpieceMessage();
   ~WorkpieceMessage();
};

#endif /* SERIALIZER_H_ */
