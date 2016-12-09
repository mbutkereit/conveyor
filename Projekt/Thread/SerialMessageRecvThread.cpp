#include "SerialMessageRecvThread.h"

extern HalBuilder hb;

SerialMessageRecvThread::SerialMessageRecvThread() {
}

SerialMessageRecvThread::~SerialMessageRecvThread() {
}

extern int isrtConnection_;
extern HalBuilder hb;

void SerialMessageRecvThread::execute(void*) {
	InfoMessage* message = InfoMessage::getInfoMessage();
	for (;;) {
		struct common_header header;
		memset(&header, 0, sizeof(struct common_header));
		hb.getHardware()->getSerial()->recvPacket((void*) &header,
				sizeof(struct common_header));

#if defined BAND && BAND == 1
		hb.getHardware()->getSerial()->sendPacket(
				(void *) message->getMessage(),
				sizeof(struct info_package));
#endif

		 LOG_DEBUG <<"Nachricht erhalten\n";

		if (header.version == MESSAGE_VERSION) {
			switch ((int) header.typ) {
			case MESSAGE_TYPE_INFO: {

				 LOG_DEBUG <<"Nachricht erhalten\n";

				struct info_package_without_ch messageInfo;
				memset(&messageInfo, 0, sizeof(struct info_package_without_ch));
				hb.getHardware()->getSerial()->recvPacket((void*) &messageInfo,
						sizeof(struct info_package_without_ch));
				message->update(&messageInfo);
				message->InhaltdesPaketesausgeben();
				if (message->isESTOPGedrueckt()) {
					int error = MsgSendPulse(isrtConnection_,
							10, 0xE, ESTOP);
					LOG_DEBUG <<"Estop steht im Paket.\n";
					if (error < 0) {
						LOG_ERROR
								<< "CRITICAL: Message ESTOP kann nicht gesendet werden .";
					}
				}
				if (message->isLbNextConveyorInterrupted()) {
					LOG_DEBUG <<"LbNextConveyorInterrupted-Bit ist gesetzt!!\n";
					int error = MsgSendPulse(isrtConnection_,
							10
							, 0xE,
							LIGHT_BARRIER_NEXT_CONVEYOR);
					if (error < 0) {
						LOG_ERROR
								<< "CRITICAL: Message NextConveyorInterrupted kann nicht gedrueckt werden.";
					}
				}

				usleep(20000);
				//@Todo slepp rausschmeißen
				sleep(5);
				hb.getHardware()->getSerial()->sendPacket(
						(void *) message->getMessage(),
						sizeof(struct info_package));
				message->resetisinterruptedBit();
				break;
			}
			case MESSAGE_TYPE_WORKPIECE: {
				struct workpiece_package_without_ch workpiece_info;
				memset(&workpiece_info, 0, sizeof(struct workpiece_package_without_ch));
				hb.getHardware()->getSerial()->recvPacket((void*) &workpiece_info,
						sizeof(struct workpiece_package_without_ch));
				WorkpieceMessage* workpiece = WorkpieceMessage::getWorkpieceMessage();
				workpiece->enqueue(workpiece_info);
				//struct workpiece_package_without_ch messageWorkpiece;
				break;
			}
			case MESSAGE_TYPE_CRITICAL_ERROR: {
				// Todo spaeter.
				break;
			}
			default: {
				LOG_ERROR
						<< "Unbekannte nachricht von der seriellen Schnittstelle erhalten \n";
			}
			}

		} else {
			LOG_ERROR << "Nicht unterstuetzte Version erhalten \n";
		}

	}
}

void SerialMessageRecvThread::shutdown() {

}
