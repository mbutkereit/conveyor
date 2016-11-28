/*
 * ConveyorThread.cpp
 *
 *  Created on: 13.10.2016
 *      Author: abx827
 */

#include <unistd.h>
#include "ConveyorThread.h"
#include "Hal/HalBuilder.h"
#include "FSM/ContextTimeMeasurement.h"

ConveyorThread::ConveyorThread() {
	// TODO Auto-generated constructor stub

}

ConveyorThread::~ConveyorThread() {
	// TODO Auto-generated destructor stub
}

/**
 * Hauptschleife des geerbten HAW-Thread.
 * Die oberklasse HAW-Thread erzwingt die Implementierung der execute Methode.
 * Der Thread endet nach Ende dieser Methode.
 */
void ConveyorThread::execute(void*) {
	/* Zugriffsrechte von QNX fuer diesen Thread, auf die Hardware erbitten. */
	if (ThreadCtl(_NTO_TCTL_IO_PRIV, 0) == -1) {
		cout << "Can't get Hardware access, therefore can't do anything."
				<< endl;
	}

	/* IO Register als Eingänge bzw. Ausgänge definieren. */
	//out8(ioControlAddress_, ioControlBitmask_);
	HalBuilder hb;
	Hardware* hw = hb.getHardware();
	if ((int) hb.getHardware()->getMT()->isItemMetal()) {
		cerr << "So nicht toll";
	}
	ContextTimeMeasurement cst;
	hw->getMotor()->stop();

	/*
	while(!cst.isFinished())
	{
		cst.transact();
	}
	*/

}

/**
 * Shutdown Methode.
 * Diese Methode muss beim erben von HAW-Thread implementiert werden.
 * Sie wird nach Ende der execute-Methode aufgerufen und dient dem
 * evtl. aufraumen das Threadablauf.
 */
void ConveyorThread::shutdown() {
	cout << "ConveyorThread shutdown" << endl;
}

