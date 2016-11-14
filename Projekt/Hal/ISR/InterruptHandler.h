/*
 * InterruptHandler.h
 *
 *  Created on: 08.11.2016
 *      Author: abx827
 */

#ifndef INTERRUPTHANDLER_H_
#define INTERRUPTHANDLER_H_

#include <sys/neutrino.h>
#include <sys/siginfo.h>
#include <cstdlib>
#include <iostream>
#include <hw/inout.h>
#include "Hal/HalBuilder.h"

extern HalBuilder hb;
extern int coid;
extern struct sigevent isrtEvent_;
extern int isrtConnection_;
extern int isrtChannel_;

extern int coid2;
extern struct sigevent isrtEvent_2;

const struct sigevent* ISR_DIO(void* arg, int id);
const struct sigevent* ISR_AIO(void* arg, int id);

void registerISR(void);
void unregisterISR(void);

#define INTERRUPT_RESET_DIO 0x30F ///< Addresse um den Interrupt der DIO zurueck zu nehmen.
#define INTERRUPT_RESET_AIO 0x320 ///< Addresse um den Interrupt der AIO zurueck zu nehmen.

/**
 * @file
 * @section DESCRIPTION
 *
 * Eine Klasse um die Ampel des Foerderbandes zu Steuern.
 */
class InterruptHandler {
public:
	InterruptHandler();
	virtual ~InterruptHandler();
};

#endif /* INTERRUPTHANDLER_H_ */
