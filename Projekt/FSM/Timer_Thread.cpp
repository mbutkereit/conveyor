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
#include "Timer_Thread.h"
#include "src/lib/Lock.h"
#include "src/lib/HWaccess.h"

/* Zuweisung der Klassenvariablen */
pthread_mutex_t Timer_Thread::mtx_ = PTHREAD_MUTEX_INITIALIZER;

/**
 *  Standard Konstruktor.
 *  Genauer beschreibender Text für Doxygen...
 *  @param seconds bestimmt Sekunden.
 */
Timer_Thread::Timer_Thread(uint32_t seconds, uint32_t nseconds, uint32_t intervalSeconds, uint32_t intervalNseconds, uint32_t* tickCount): seconds_(seconds), nseconds_(nseconds), intervalSeconds_(intervalSeconds), intervalNseconds_(intervalNseconds), tickCount_(tickCount), timerPaused_(false) {
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

	// Configure Timer to send every two seconds.
	timerSpec_.it_value.tv_sec = seconds_;
	timerSpec_.it_value.tv_nsec = nseconds_;
	timerSpec_.it_interval.tv_sec = intervalSeconds_;
	timerSpec_.it_interval.tv_nsec = intervalNseconds_;
    cout << "ctor: Timer_Thread" << endl;
}


/**
 * Standard Dekonstruktor.
 * Wird aufgerufen beim loeschen des Objektes.
 * Objekte die im lokalen Kontext von Methoden allogiert wurden, werden
 * beim Bereinigen des Stacks, bei Methodenende automatisch deallokiert.
 * --> Automatischer Dekonstruktoraufruf.
 */
Timer_Thread::~Timer_Thread() {
    cout << "dtor: Timer_Thread" << endl;
}


/**
 * Hauptschleife des geerbten HAW-Thread.
 * Die oberklasse HAW-Thread erzwingt die Implementierung der execute Methode.
 * Der Thread endet nach Ende dieser Methode.
 */
void Timer_Thread::execute(void*){
	// Loop started by timer.
	int temp = 0;
	while(!(this->isStopped())){
		    MsgReceivePulse(channel_, &pulse_, sizeof (pulse_), NULL);
		    temp = *tickCount_;
		    temp+=1;
		    *tickCount_ = temp;
		    cout << *tickCount_ << endl;
	}
}

/**
 * Shutdown Methode.
 * Diese Methode muss beim erben von HAW-Thread implementiert werden.
 * Sie wird nach Ende der execute-Methode aufgerufen und dient dem
 * evtl. aufraumen das Threadablauf.
 */
void Timer_Thread::shutdown(){
	// Cleanup
	if( ConnectDetach(connection_) == -1){
		exit(EXIT_FAILURE);
	}

	if( ChannelDestroy(channel_) == -1 ){
		exit(EXIT_FAILURE);
	}
    cout << "Timer_Thread shutdown" << endl;
}

void Timer_Thread::startTimer(){
	timer_settime(timerId_, 0, &timerSpec_, NULL);
	this->start(NULL);
}

void Timer_Thread::stopTimer(){
	this->hold();
    timerSpec_.it_value.tv_sec = 0;
    timerSpec_.it_value.tv_nsec = 0;
	timer_settime(timerId_, 0, &timerSpec_, &backup_);
}

void Timer_Thread::continueTimer(){
	this->cont();
	timer_settime(timerId_, 0, &backup_, NULL);
}
