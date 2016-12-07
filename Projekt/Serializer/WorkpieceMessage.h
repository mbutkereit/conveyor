#ifndef WORKPIECE_MESSAGE_H_
#define WORKPIECE_MESSAGE_H_

#include <stdint.h>
#include "Serializer/Serializer.h"
#include  <queue>
#include "Hal/HalBuilder.h"

class WorkpieceMessage
{
public:
    static WorkpieceMessage* getWorkpieceMessage()
    {
        static WorkpieceMessage instances;
        return &instances;
    }
    void send(uint16_t hohehnmesswert_1 ,uint16_t hohehnmesswert_2,uint8_t typ ,uint16_t id );
    void enqueue(struct workpiece_package_without_ch  element);
    struct workpiece_package_without_ch getWorkpieceInfo();

    ~WorkpieceMessage();
private:
    WorkpieceMessage();

   struct info_package info;
   std::queue<struct workpiece_package_without_ch> queue;

};

#endif /* SERIALIZER_H_ */
