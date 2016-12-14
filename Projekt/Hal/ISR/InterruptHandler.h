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
#include <hw/inout.h>
#include "Hal/HalBuilder.h"

// HAL Builder fuer den Zugriff auf die Hardware.
extern HalBuilder hb;

// Variablen fuer die DIO.
extern int coid;
extern struct sigevent isrtEvent_;

// Variablen fuer die AIO.
extern int coid2;
extern struct sigevent isrtEvent_2;

// Variablen fuer die Kommunikation ueber Pulse Messages.
extern int isrtConnection_;
extern int isrtChannel_;

/**
 * ISR fuer die DIO.
 * Sendet eine Pulse Message an den isrtChannel_
 * falls ein Interrupt behandelt werden konnte.
 *
 */
const struct sigevent* ISR_DIO(void* arg, int id);

/**
 * ISR fuer die AIO.
 * Sendet eine Pulse Message an den isrtChannel_
 * falls ein Interrupt behandelt werden konnte.
 */
const struct sigevent* ISR_AIO(void* arg, int id);

/**
 * Registriert die beiden ISR fuer AIO und DIO.
 */
void registerISR(void);

/**
 * Entfernt die beiden ISR vom System.
 */
void unregisterISR(void);

#define INTERRUPT_RESET_DIO 0x30F ///< Addresse um den Interrupt der DIO zurueck zu nehmen.
#define INTERRUPT_RESET_AIO 0x320 ///< Addresse um den Interrupt der AIO zurueck zu nehmen.

// Konstante fuer die Erkennung des Aktuellen ISR statuses.
#define IS_RUNNING_IN_STATE 0x1 ///< Status um zu zeigen das aktuell ein Element in der LR Anfang des Foerderbandes ist.
#define IS_IN_ALTIMETRY_STATE 0x2 ///<
#define IS_IN_SWITCH_STATE 0x4 ///<
#define IS_SKID_FULL_STATE 0x8 ///<
#define IS_RUNNING_OUT_STATE 0x10 ///<

// Event Detektion
#define ESTOP 0x400 //< ESTOP Event.
#define RESET 0x800//< RESET Event.
#define START 0x1000 //< START Event.
#define STOP 0x2000 //< STOP Event.

#define ALTIMETRY_COMPLETED 0x4000

#define LIGHT_BARRIER_BEGIN_INTERRUPTED 0x1
#define LIGHT_BARRIER_END_INTERRUPTED 0x100
#define LIGHT_BARRIER_SWITCH_INTERRUPTED 0x10
#define LIGHT_BARRIER_ALTIMETRY_INTERRUPTED 0x4
#define LIGHT_BARRIER_SKID_INTERRUPTED 0x40

#define LIGHT_BARRIER_BEGIN_NOT_INTERRUPTED 0x2
#define LIGHT_BARRIER_END_NOT_INTERRUPTED 0x200
#define LIGHT_BARRIER_SWITCH_NOT_INTERRUPTED 0x20
#define LIGHT_BARRIER_ALTIMETRY_NOT_INTERRUPTED 0x8
#define LIGHT_BARRIER_SKID_NOT_INTERRUPTED 0x80
#define LIGHT_BARRIER_NEXT_CONVEYOR 0x8000

#define TIMER_INTERRUPT 0x10000


#endif /* INTERRUPTHANDLER_H_ */
