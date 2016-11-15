#include <unistd.h>
#include "Thread/ConveyorThread.h"

ConveyorThread::ConveyorThread() {}

ConveyorThread::~ConveyorThread() {}

void ConveyorThread::execute(void*) {
	if (ThreadCtl(_NTO_TCTL_IO_PRIV, 0) == -1) {
		cout << "Can't get Hardware access, therefore can't do anything."
				<< endl;
	}

	HalBuilder hb;
	Hardware* hw = hb.getHardware();
    hw->getMotor()->stop();
    	if( (int)hb.getHardware()->getMT()->isItemMetal()){
    		cerr << "So nicht toll";
    	}

   hw->getMotor()->fast();

}

void ConveyorThread::shutdown() {
	cout << "ConveyorThread shutdown" << endl;
}

