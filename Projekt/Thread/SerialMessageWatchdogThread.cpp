#include "Thread/SerialMessageWatchdogThread.h"

SerialMessageWatchdogThread::SerialMessageWatchdogThread() {
	SerialMessageWatchdogThread::notify();
}

SerialMessageWatchdogThread::~SerialMessageWatchdogThread() {
}

extern int isrtConnection_;
extern HalBuilder hb;

static uint8_t isALive = 1;

void SerialMessageWatchdogThread::notify(){
		isALive = 1;
		LOG_DEBUG <<"Notify me:"<< (int) isALive<< "\n";
}

void SerialMessageWatchdogThread::execute(void*) {
	SerialMessageWatchdogThread::notify();

	for (;;) {

		LOG_DEBUG << "Aktueller isALive Value :" << (int)isALive << "\n";

		if ((int)isALive != 1) {
			struct critical_error_package errorPackage;

			errorPackage.typ = MESSAGE_TYPE_CRITICAL_ERROR;
			errorPackage.version = MESSAGE_VERSION;
			errorPackage.event = 0;
			errorPackage.reserved = 0;

			hb.getHardware()->getSerial()->sendPacket(&errorPackage,
					sizeof(struct critical_error_package));
			LOG_ERROR << "CRITICAL: Der Automat meldet sich nicht mehr. \n";
			//todo abfangen;
		}

		isALive = 0;
		LOG_DEBUG << "Watchdog Pulse Message Channel: "<< isrtChannel_ <<"\n";
		int error = MsgSendPulse(isrtConnection_,10,WATCHDOG_PULSE_CODE,10);
		if (error < 0) {
			LOG_ERROR << "CRITICAL: Watchdog kann keine Messages senden. \n";
		}

		sleep(WATCHDOG_SLEEP);
	}
}

void SerialMessageWatchdogThread::shutdown() {

}
