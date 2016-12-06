#include "SerialMessageRecvThread.h"

extern int isrtChannel_;
extern HalBuilder hb;

SerialMessageRecvThread::SerialMessageRecvThread() {
}

SerialMessageRecvThread::~SerialMessageRecvThread() {
}

extern int isrtChannel_;
extern HalBuilder hb;

void SerialMessageRecvThread::execute(void*) {
	for (;;) {
		struct common_header header;
		memset(&header, 0, sizeof(struct common_header));
		hb.getHardware()->getSerial()->recvPacket((void*) &header,
				sizeof(struct common_header));
		if (header.version == MESSAGE_VERSION) {
			switch ((int)header.typ) {
			case MESSAGE_TYPE_INFO:{
				struct info_package_without_ch messageInfo;
				hb.getHardware()->getSerial()->recvPacket((void*) &messageInfo,
						sizeof(struct info_package_without_ch));
				InfoMessage* message = InfoMessage::getInfoMessage();
				message->update(&messageInfo);

				if (message->isESTOPGedrueckt()) {
					int error = MsgSendPulse(isrtChannel_,
							sched_get_priority_max(0), 0xE, STOP);
					if (error < 0) {
						LOG_ERROR
								<< "CRITICAL: ESTOP kann nicht gedrueckt werden.";
					}
				}
				usleep(20000);
				hb.getHardware()->getSerial()->sendPacket((void *)message->getMessage(),sizeof(struct info_package));
				break;}
			case MESSAGE_TYPE_WORKPIECE:{
				//struct workpiece_package_without_ch messageWorkpiece;
				break;}
			case MESSAGE_TYPE_CRITICAL_ERROR:{
				// Todo spaeter.
				break;}
			default:{
				LOG_ERROR
						<< "Unbekannte nachricht von der seriellen Schnittstelle erhalten \n";}
			}



		} else {
			LOG_ERROR << "Nicht unterstuetzte Version erhalten \n";
		}

	}
}

void SerialMessageRecvThread::shutdown() {

}
