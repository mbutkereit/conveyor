/**
 * @file    Blink_Thread.cpp
 * @author  Simon Brummer
 * @version 0.1
 *
 * Demoklasse fuer das SEP2 Tutorium. Es wird folgendes gezeigt:
 * - Grundlegende Syntax von C++. Variablen und Methodendeklaration
 *   sowie statische Methoden.
 * - Implementierung des HAW-Threads
 * - Einsatz des "scoped locking"-Pattern. Lock in Blink_Thread::execute
 * - Grundlegende initialisierung und Ansteuerung der Hardware
 * - Kommentierung in einem Format mit das Doxygen parsen kann.
 */

#include <unistd.h>
#include "Switchtimer_Thread.h"
#include "lib/Lock.h"
#include "lib/HWaccess.h"
#include "Hal/Halbuilder.h"

extern HalBuilder hb;

/* Zuweisung der Klassenvariablen */
pthread_mutex_t Switchtimer_Thread::mtx_ = PTHREAD_MUTEX_INITIALIZER;

/**
 *  Standard Konstruktor.
 *  Genauer beschreibender Text für Doxygen...
 *  @param seconds bestimmt Sekunden.
 */
Switchtimer_Thread::Switchtimer_Thread(uint32_t seconds): seconds_(seconds) {
    /* Einfacher Konstruktor, setzt die Werte der Instanzvariablen.
     * Methode bei Times ist vorzuziehen. Direktes kopieren bei
     * Objekterzeugung, gilt nicht als Zuweisung und verstösst
     * damit auch nicht gegen die const correctness siehe Blink_Thread.h .
     */
	// Setup Message Passing Channels and Connections
	if( (channel_  = ChannelCreate(0)) == -1){
		exit(EXIT_FAILURE);
	}
	if( (connection_ = ConnectAttach(0, 0, channel_, 0, 0)) == -1){
		exit(EXIT_FAILURE);
	}

	// Setup Event Structure
	SIGEV_PULSE_INIT (&timerEvent_, connection_, SIGEV_PULSE_PRIO_INHERIT, 0, 0);

	// Create Timer
	if (timer_create (CLOCK_REALTIME, &timerEvent_, &timerId_) == -1) {
		exit (EXIT_FAILURE);
	}

	// Configure Timer to send every given seconds.
	timerSpec_.it_value.tv_sec = 0;
	timerSpec_.it_value.tv_nsec = 0;
	timerSpec_.it_interval.tv_sec = 0;
	timerSpec_.it_interval.tv_nsec = 0;
    cout << "ctor: Switchtimer_Thread" << endl;
}


/**
 * Standard Dekonstruktor.
 * Wird aufgerufen beim loeschen des Objektes.
 * Objekte die im lokalen Kontext von Methoden allogiert wurden, werden
 * beim Bereinigen des Stacks, bei Methodenende automatisch deallokiert.
 * --> Automatischer Dekonstruktoraufruf.
 */
Switchtimer_Thread::~Switchtimer_Thread() {
    cout << "dtor: Switchtimer_Thread" << endl;
}


/**
 * Hauptschleife des geerbten HAW-Thread.
 * Die oberklasse HAW-Thread erzwingt die Implementierung der execute Methode.
 * Der Thread endet nach Ende dieser Methode.
 */
void Switchtimer_Thread::execute(void*){
	// Loop started by timer.
	while(!(this->isStopped())){
		MsgReceivePulse(channel_, &pulse_, sizeof (pulse_), NULL);
		hb.getHardware()->getMotor()->closedSwitch();
	}
}

/**
 * Shutdown Methode.
 * Diese Methode muss beim erben von HAW-Thread implementiert werden.
 * Sie wird nach Ende der execute-Methode aufgerufen und dient dem
 * evtl. aufraumen das Threadablauf.
 */
void Switchtimer_Thread::shutdown(){
	// Cleanup
	if( ConnectDetach(connection_) == -1){
		exit(EXIT_FAILURE);
	}

	if( ChannelDestroy(channel_) == -1 ){
		exit(EXIT_FAILURE);
	}
    cout << "Switchtimer_Thread shutdown" << endl;
}

void Switchtimer_Thread::startTimer(){
    this->cont();
    timerSpec_.it_value.tv_sec = seconds_;
    timer_settime(timerId_, 0, &timerSpec_, NULL);
}

void Switchtimer_Thread::stopTimer(){
	this->hold();
    timerSpec_.it_value.tv_sec = 0;
    timerSpec_.it_value.tv_nsec = 0;
	timer_settime(timerId_, 0, &timerSpec_, NULL);
}

void Switchtimer_Thread::continueTimer(){
	this->cont();
	timerSpec_.it_value.tv_sec = seconds_;
	timer_settime(timerId_, 0, &timerSpec_, NULL);
}
