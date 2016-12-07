#include "Serializer/WorkpieceMessage.h"


extern HalBuilder hb;

WorkpieceMessage::WorkpieceMessage(){
}

WorkpieceMessage::~WorkpieceMessage(){}




void WorkpieceMessage::send(uint16_t hohehnmesswert_1 ,uint16_t hohehnmesswert_2,uint8_t typ ,uint16_t id ){
	struct workpiece_package message;

	message.version = MESSAGE_VERSION;
	message.typ = MESSAGE_TYPE_WORKPIECE;

	message.id = id;
	message.workpiece_type = typ;
	message.alimetry_value_one = hohehnmesswert_1;
	message.alimetry_value_two= hohehnmesswert_2;

	message.reserved = 0;

	hb.getHardware()->getSerial()->sendPacket(
			(void *) &message,
			sizeof(struct workpiece_package));
}

void WorkpieceMessage::enqueue(struct workpiece_package_without_ch  element){
	this->queue.push(element);
}

struct workpiece_package_without_ch WorkpieceMessage::getWorkpieceInfo(){
	struct workpiece_package_without_ch  element = this->queue.front();
	this->queue.pop();
	return element;
}


