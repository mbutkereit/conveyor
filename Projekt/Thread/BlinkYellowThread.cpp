#include <Thread/BlinkYellowThread.h>

BlinkYellowThread::BlinkYellowThread(){}

BlinkYellowThread::~BlinkYellowThread(){}

void BlinkYellowThread::execute(void*) {
	if (ThreadCtl(_NTO_TCTL_IO_PRIV, 0) == -1) {
		cout << "Can't get Hardware access, therefore can't do anything."
				<< endl;
	}

	HalBuilder hb;
	Hardware* hw = hb.getHardware();
	while(!isStopped()){
	sleep(1);
	hw->getTL()->turnYellowOn();
	sleep(1);
	hw->getTL()->turnYellowOff();
	}

}

void BlinkYellowThread::shutdown() {
	HalBuilder hb;
	Hardware* hw = hb.getHardware();
	hw->getTL()->turnYellowOff();
}

