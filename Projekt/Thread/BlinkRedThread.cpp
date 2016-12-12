#include <Thread/BlinkRedThread.h>

BlinkRedThread::BlinkRedThread(){}

BlinkRedThread::~BlinkRedThread(){}

void BlinkRedThread::execute(void*) {
	if (ThreadCtl(_NTO_TCTL_IO_PRIV, 0) == -1) {
		cout << "Can't get Hardware access, therefore can't do anything."
				<< endl;
	}

	HalBuilder hb;
	Hardware* hw = hb.getHardware();
	while(!isStopped()){
	sleep(1);
	hw->getTL()->turnRedOn();
	sleep(1);
	hw->getTL()->turnRedOff();
	}

}

void BlinkRedThread::shutdown() {
	HalBuilder hb;
	Hardware* hw = hb.getHardware();
	hw->getTL()->turnRedOff();
}

