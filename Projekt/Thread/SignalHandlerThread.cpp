#include "SignalHandlerThread.h"

int coid = 0;
int coid2 = 0;
int isrtChannel_ = 0;
int isrtChannel_2 = 0;
int isrtConnection_ = 0;
int isrtConnection_2 = 0;

struct sigevent isrtEvent_ = { };
struct sigevent isrtEvent_2 = { };

SignalHandlerThread::SignalHandlerThread() {
}

SignalHandlerThread::~SignalHandlerThread() {
}

void SignalHandlerThread::execute(void*) {

	Context context;
	struct _pulse pulse;

	if (ThreadCtl(_NTO_TCTL_IO_PRIV, 0) == -1) {
		LOG_DEBUG << "SignalHandlerThread kann keine rechte bekommen.";
		exit (EXIT_FAILURE);
	}

	do {

		if (MsgReceivePulse(isrtChannel_, &pulse, sizeof(pulse), NULL) == -1) {
			LOG_DEBUG << "Fehler beim Signal Handler Message Recieve.";
			exit (EXIT_FAILURE);
		}

		short pulsecode = pulse.code;

		if (pulsecode == 0xE) {

			int code = pulse.value.sival_int;

			if (code & ESTOP) {
				context.signalEStop();
			}

			if (code & LIGHT_BARRIER_BEGIN_INTERRUPTED) {
				context.signalLBBeginInterrupted();
			}
			if (code & LIGHT_BARRIER_BEGIN_NOT_INTERRUPTED) {
				context.signalLBBeginNotInterrupted();
			}
			if (code & LIGHT_BARRIER_ALTIMETRY_INTERRUPTED) {
				context.signalLBAltimetryInterrupted();
			}
			if (code & LIGHT_BARRIER_ALTIMETRY_NOT_INTERRUPTED) {
				context.signalLBAltimetryNotInterrupted();
			}
			if (code & LIGHT_BARRIER_SWITCH_INTERRUPTED) {
				context.signalLBSwitchInterrupted();
			}

			if (code & LIGHT_BARRIER_SWITCH_NOT_INTERRUPTED) {
				context.signalLBSwitchNotInterrupted();
			}

			if (code & LIGHT_BARRIER_SKID_INTERRUPTED) {
				context.signalLBSkidInterrupted();
			}

			if (code & LIGHT_BARRIER_SKID_NOT_INTERRUPTED) {
				context.signalLBSkidNotInterrupted();
			}

			if (code & LIGHT_BARRIER_END_INTERRUPTED) {
				context.signalLBEndInterrupted();
			}

			if (code & LIGHT_BARRIER_END_NOT_INTERRUPTED) {
				context.signalLBEndNotInterrupted();
			}

			if (code & ALTIMETRY_COMPLETED) {
				context.signalAltimetryCompleted();
			}

			if (code & START) {
				context.signalStart();
			}
			if (code & RESET) {
				context.signalReset();
			}
			if (code & STOP) {
				context.signalStop();
			}
		}else{
			LOG_WARNING << "Einen nicht bekannten Code erhalten." << pulsecode << "\n" ;
		}

	} while (1);

}

void SignalHandlerThread::shutdown() {
	if (ConnectDetach(isrtConnection_) == -1) {
		exit (EXIT_FAILURE);
	}

	if (ChannelDestroy(isrtChannel_) == -1) {
		exit (EXIT_FAILURE);
	}
}
