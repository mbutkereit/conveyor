#include "Thread/SignalHandlerThread.h"


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

/**
 * Die Klasse empfängt alle eingehenden Pulse von der ISR(Kernel) und dem Thread von der serielle Schnittstelle.
 * Anschließend wird überprüft, ob ein Ereignis vorliegt und wenn ja wird es bei allen Listenern ausgeführt
 * Listener werden hinzugefügt, wenn die vordere Lichtschranke durchbrochen wird und Listener werden entfernt wenn die
 * Automaten den Emzustand erreicht haben.
 */
void SignalHandlerThread::execute(void*) {
	struct _pulse pulse;
	int globalerID_Zaehler=0;
	std::vector<Puck> puckvector;
	int skidcounter =0;
	//TODO delete von Dispatcher fehlt noch
	Dispatcher* disp = new Dispatcher();
	std::vector<ContextI*> contextContainer;
	InfoMessage* message = InfoMessage::getInfoMessage();

	if (ThreadCtl(_NTO_TCTL_IO_PRIV, 0) == -1) {
		LOG_DEBUG << "SignalHandlerThread kann keine rechte bekommen.";
		exit(EXIT_FAILURE);
	}
	// LOG_DEBUG << "Starte den Automaten Handler mit Channel Nummer: "<< isrtChannel_ <<"\n";
	do {

		if (MsgReceivePulse(isrtChannel_, &pulse, sizeof(pulse), NULL) == -1) {
			LOG_DEBUG << "Fehler beim Signal Handler Message Recieve.";
			exit(EXIT_FAILURE);
		}

		short pulsecode = pulse.code;
		//	LOG_DEBUG << "Aktueller Pulse CODE: "<< (int)pulsecode <<"\n" ;

		if (pulsecode == 0xE) {

			LOG_DEBUG << "Signalhandler hat einen Puls erhalten mit Code E:"
					<< pulse.value.sival_int << "\n";

			int code = pulse.value.sival_int;

			if (code & ESTOP) {
				disp->callListeners(ESTOPSIGNAL);
			}

			if (code & LIGHT_BARRIER_BEGIN_INTERRUPTED) {
				//Hier die Namen für die Fsm einfügen
#if defined BAND && BAND == 1
				ContextI* context= new ContextTopFSM1(globalerID_Zaehler++, &puckvector, &skidcounter);
#endif
#if defined BAND && BAND == 2
				ContextI* context = new ContextTopFSM2(globalerID_Zaehler++,&puckvector,&skidcounter);
#endif
#if defined BAND && BAND == 3

				ContextI* context =  new ContextTopFSM3(globalerID_Zaehler++,&puckvector);
#endif




				//TODO extract Method
				disp->addListener(context, LBBEGININTERRUPTED);
				disp->addListener(context, LBBEGINNOTINTERRUPTED);
				disp->addListener(context, LBENDINTERRUPTED);
				disp->addListener(context, LBENDNOTINTERRUPTED);
				disp->addListener(context, LBALTIMETRYINTERRUPTED);
				disp->addListener(context, LBALTIMETRYNOTINTERRUPTED);
				disp->addListener(context, LBSKIDINTERRUPTED);
				disp->addListener(context, LBSKIDNOTINTERRUPTED);
				disp->addListener(context, LBSWITCHINTERRUPTED);
				disp->addListener(context, LBSWITCHNOTINTERRUPTED);
				disp->addListener(context, RESETSIGNAL);
				disp->addListener(context, STARTSIGNAL);
				disp->addListener(context, ESTOPSIGNAL);
				disp->addListener(context, STOPSIGNAL);
				disp->addListener(context, ALTEMETRYCOMPLETE);
				disp->addListener(context, LBNEXTCONVEYOR);
				contextContainer.push_back(context);
				disp->callListeners(LBBEGININTERRUPTED);
				//@TODO
				message->setLBinterruptedBit();
			}
			if (code & LIGHT_BARRIER_BEGIN_NOT_INTERRUPTED) {
				disp->callListeners(LBBEGINNOTINTERRUPTED);
			}
			if (code & LIGHT_BARRIER_ALTIMETRY_INTERRUPTED) {
				disp->callListeners(LBALTIMETRYINTERRUPTED);
			}
			if (code & LIGHT_BARRIER_ALTIMETRY_NOT_INTERRUPTED) {
				disp->callListeners(LBALTIMETRYNOTINTERRUPTED);
			}
			if (code & LIGHT_BARRIER_SWITCH_INTERRUPTED) {
				disp->callListeners(LBSWITCHINTERRUPTED);
			}

			if (code & LIGHT_BARRIER_SWITCH_NOT_INTERRUPTED) {
				disp->callListeners(LBSWITCHNOTINTERRUPTED);
			}

			if (code & LIGHT_BARRIER_SKID_INTERRUPTED) {
				disp->callListeners(LBSKIDINTERRUPTED);
			}

			if (code & LIGHT_BARRIER_SKID_NOT_INTERRUPTED) {
				disp->callListeners(LBSKIDNOTINTERRUPTED);
			}

			if (code & LIGHT_BARRIER_END_INTERRUPTED) {
				disp->callListeners(LBENDINTERRUPTED);
			}

			if (code & LIGHT_BARRIER_END_NOT_INTERRUPTED) {
				disp->callListeners(LBENDNOTINTERRUPTED);
			}

			if (code & ALTIMETRY_COMPLETED) {
				disp->callListeners(ALTEMETRYCOMPLETE);
			}

			if (code & START) {
				disp->callListeners(STARTSIGNAL);
			}
			if (code & RESET) {
				message->setQuittierung();
				disp->callListeners(RESETSIGNAL);
			}
			if (code & STOP) {
				disp->callListeners(STOPSIGNAL);
			}
			if (code & LIGHT_BARRIER_NEXT_CONVEYOR) {
				disp->callListeners(LBNEXTCONVEYOR);
			}

			//Checkt ob ein Automat den Endzustand erreicht und wenn dies so ist, dann wird der Automat gelÃ¶scht.
			for (uint8_t i = 0; i < contextContainer.size(); i++) {
				if (contextContainer[i]->isContextimEnzustand()) {

					//TODO extract Method
					disp->remListeners(contextContainer[i], LBBEGININTERRUPTED);
					disp->remListeners(contextContainer[i],
							LBBEGINNOTINTERRUPTED);
					disp->remListeners(contextContainer[i], LBENDINTERRUPTED);
					disp->remListeners(contextContainer[i],
							LBENDNOTINTERRUPTED);
					disp->remListeners(contextContainer[i],
							LBALTIMETRYINTERRUPTED);
					disp->remListeners(contextContainer[i],
							LBALTIMETRYNOTINTERRUPTED);
					disp->remListeners(contextContainer[i], LBSKIDINTERRUPTED);
					disp->remListeners(contextContainer[i],
							LBSKIDNOTINTERRUPTED);
					disp->remListeners(contextContainer[i],
							LBSWITCHINTERRUPTED);
					disp->remListeners(contextContainer[i],
							LBSWITCHNOTINTERRUPTED);
					disp->remListeners(contextContainer[i], RESETSIGNAL);
					disp->remListeners(contextContainer[i], STARTSIGNAL);
					disp->remListeners(contextContainer[i], ESTOPSIGNAL);
					disp->remListeners(contextContainer[i], STOPSIGNAL);
					disp->remListeners(contextContainer[i], ALTEMETRYCOMPLETE);

#if defined BAND && BAND == 1

					ContextI *contextpointer = (ContextI*) contextContainer[i];

					contextContainer.erase(contextContainer.begin() + i);
			//		delete contextpointer;
#endif
#if defined BAND && BAND == 2
					ContextI *contextpointer = (ContextI*) contextContainer[i];
					contextContainer.erase(contextContainer.begin() + i);
					delete contextpointer;
#endif
#if defined BAND && BAND == 3
					ContextI *contextpointer = (ContextI*) contextContainer[i];
					contextContainer.erase(contextContainer.begin() + i);
					delete contextpointer;
#endif

					LOG_DEBUG << "Ich töte jetzt einen Automaten. \n";
				}
			}

		} else {
			if (pulsecode == WATCHDOG_PULSE_CODE) {
				SerialMessageWatchdogThread::notify(); // Schauen ob der Automat am leben ist.
			} else {
				LOG_WARNING << "Einen nicht bekannten Code erhalten."
						<< pulsecode << "\n";
			}
		}

	} while (1);

}

void SignalHandlerThread::shutdown() {
	if (ConnectDetach(isrtConnection_) == -1) {
		exit(EXIT_FAILURE);
	}

	if (ChannelDestroy(isrtChannel_) == -1) {
		exit(EXIT_FAILURE);
	}
}
