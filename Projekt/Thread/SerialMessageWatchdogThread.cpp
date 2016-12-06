#include "Thread/SerialMessageWatchdogThread.h"

SerialMessageWatchdogThread::SerialMessageWatchdogThread() {
	SerialMessageWatchdogThread::notify();
}

SerialMessageWatchdogThread::~SerialMessageWatchdogThread() {
}

extern int isrtChannel_;
extern HalBuilder hb;

void SerialMessageWatchdogThread::execute(void*) {
	SerialMessageWatchdogThread::notify();

	for (;;) {

		if (isALive != 1) {
			struct critical_error_package errorPackage;

			errorPackage.typ = MESSAGE_TYPE_CRITICAL_ERROR;
			errorPackage.version = MESSAGE_VERSION;
			errorPackage.event = 0;
			errorPackage.reserved = 0;

			hb.getHardware()->getSerial()->sendPacket(&errorPackage,
					sizeof(struct critical_error_package));
			LOG_ERROR << "CRITICAL: Der Automat meldet sich nicht mehr. \n";
		}

		isALive = 0;

		int error = MsgSendPulse(isrtChannel_,sched_get_priority_max(0),WATCHDOG_PULSE_CODE,0);
		if (error < 0) {
			LOG_ERROR << "CRITICAL: Watchdog kann keine Messages senden. \n";
		}

		sleep(WATCHDOG_SLEEP);
	}
}

void SerialMessageWatchdogThread::shutdown() {

}
