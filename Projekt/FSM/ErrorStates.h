/*
 * ErrorStates.h
 *
 *  Created on: 23.11.2016
 *      Author: abs949
 */

#ifndef ERRORSTATES_H_
#define ERRORSTATES_H_
#include "Hal/Halbuilder.h"
#include <unistd.h>
using namespace std;

extern HalBuilder hb;

class ErrorStates{

public:
	ErrorStates();
	~ErrorStates();

	void puckLost() {
		hb.getHardware()->getMotor()->stop();
		//Errormessage implementation missing
		while (!(hb.getHardware()->getHMI()->isButtonStartPressed())) {
			hb.getHardware()->getTL()->turnYellowOn();
			usleep(500000);
			hb.getHardware()->getTL()->turnYellowOff();
			usleep(500000);
		}
		hb.getHardware()->getTL()->turnYellowOff();
		hb.getHardware()->getTL()->turnGreenOn();
	}

	void bothSkidsFull() {
		hb.getHardware()->getMotor()->stop();
		hb.getHardware()->getTL()->turnGreenOff();
		//Errormessage impelementation missing
		while (!(hb.getHardware()->getHMI()->isButtonStartPressed())) {
			hb.getHardware()->getTL()->turnRedOn();
			usleep(500000);
			hb.getHardware()->getTL()->turnRedOff();
			usleep(500000);
		}
		hb.getHardware()->getTL()->turnGreenOn();
	}
};


#endif /* ERRORSTATES_H_ */
