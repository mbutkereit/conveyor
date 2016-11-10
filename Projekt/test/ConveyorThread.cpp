/*
 * ConveyorThread.cpp
 *
 *  Created on: 13.10.2016
 *      Author: abx827
 */

#include <unistd.h>
#include "ConveyorThread.h"
#include "Hal/HalBuilder.h"

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

	hw->getTL()->turnRedOn();
	sleep(1);
	hw->getTL()->turnRedOff();
	sleep(1);
	hw->getTL()->turnYellowOn();
	sleep(1);
	hw->getTL()->turnYellowOff();
	sleep(1);
	hw->getTL()->turnGreenOn();
	sleep(1);
	hw->getTL()->turnGreenOff();

//	hw->getMotor()->slow();
//	hw->getMotor()->fast();
//	sleep(5);
//	hw->getMotor()->left();
//	hw->getMotor()->right();
//	sleep(5);
//	sleep(5);
//	hw->getMotor()->slow();
//	sleep(5);
//	hw->getMotor()->stop();
//	sleep(5);
//	hw->getMotor()->switchOpen();
//	sleep(5);
//	hw->getMotor()->switchClosed();
//
//	hw->getHMI()->turnLedStartOn();
//	sleep(5);
//	hw->getHMI()->turnLedStartOff();
//	sleep(5);
//	hw->getHMI()->turnLedResetOn();
//	sleep(5);
//	hw->getHMI()->turnLedResetOff();
//	sleep(5);
//	hw->getHMI()->turnLedQ1On();
//	sleep(5);
//	hw->getHMI()->turnLedQ1Off();
//	sleep(5);
//	hw->getHMI()->turnLedQ2On();
//	sleep(5);
//	hw->getHMI()->turnLedQ2Off();

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

