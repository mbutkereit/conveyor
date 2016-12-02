#include "SignalHandlerThread.h"
//#include "Dispatcher.h"

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


	struct _pulse pulse;
	//TODO delete von Dispatcher fehlt noch
	Dispatcher* disp = new Dispatcher();
	std::vector<Context* >contextContainer;




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
				disp->callListeners(ESTOPSIGNAL);
			}

			if (code & LIGHT_BARRIER_BEGIN_INTERRUPTED) {
				Context* context = new Context();
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
				contextContainer.push_back(context);
				disp->callListeners(LBBEGININTERRUPTED);
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
				disp->callListeners(RESETSIGNAL);
			}
			if (code & STOP) {
				disp->callListeners(STOPSIGNAL);
			}
			
			//Checkt ob ein Automat den Endzustand erreicht und wenn dies so ist, dann wird der Automat gelöscht.
			for(int i=0;i < contextContainer.size();i++ ){
				if(contextContainer[i]->isContextimEnzustand()){
					//TODO extract Method
					disp->remListeners(contextContainer[i],LBBEGININTERRUPTED);
					disp->remListeners(contextContainer[i], LBBEGINNOTINTERRUPTED);
					disp->remListeners(contextContainer[i], LBENDINTERRUPTED);
					disp->remListeners(contextContainer[i], LBENDNOTINTERRUPTED);
					disp->remListeners(contextContainer[i], LBALTIMETRYINTERRUPTED);
					disp->remListeners(contextContainer[i], LBALTIMETRYNOTINTERRUPTED);
					disp->remListeners(contextContainer[i], LBSKIDINTERRUPTED);
					disp->remListeners(contextContainer[i], LBSKIDNOTINTERRUPTED);
					disp->remListeners(contextContainer[i], LBSWITCHINTERRUPTED);
					disp->remListeners(contextContainer[i], LBSWITCHNOTINTERRUPTED);
					disp->remListeners(contextContainer[i], RESETSIGNAL);
					disp->remListeners(contextContainer[i], STARTSIGNAL);
					disp->remListeners(contextContainer[i], ESTOPSIGNAL);
					disp->remListeners(contextContainer[i], STOPSIGNAL);
					disp->remListeners(contextContainer[i], ALTEMETRYCOMPLETE);
					delete (context*)contextContainer[i];
					contextContainer.erase[i];
				}
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
