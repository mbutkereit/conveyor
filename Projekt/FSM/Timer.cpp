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
#include "Timer.h"
#include "src/lib/Lock.h"
#include "src/lib/HWaccess.h"

/* Zuweisung der Klassenvariablen */
pthread_mutex_t Timer::mtx_ = PTHREAD_MUTEX_INITIALIZER;

/**
 *  Standard Konstruktor.
 *  Genauer beschreibender Text für Doxygen...
 *  @param seconds bestimmt Sekunden.
 */
Timer* Timer::getInstance(){
    static Timer instance_;
    return &instance_;
}

Timer::Timer() {
    /* Einfacher Konstruktor, setzt die Werte der Instanzvariablen.
     * Methode bei Times ist vorzuziehen. Direktes kopieren bei
     * Objekterzeugung, gilt nicht als Zuweisung und verstösst
     * damit auch nicht gegen die const correctness siehe Blink_Thread.h .
     */
	// Setup Event Structure
	SIGEV_PULSE_INIT(&timerEvent_, isrtConnection_, SIGEV_PULSE_PRIO_INHERIT, 0, 0);
	timerEvent_.__sigev_un2.__st.__sigev_code=0xE;
	timerEvent_.sigev_value.sival_int = TIMER_INTERRUPT;

	// Create Timer
	if (timer_create (CLOCK_REALTIME, &timerEvent_, &timerId_) == -1) {
		exit (EXIT_FAILURE);
	}
    cout << "ctor: Timer" << endl;
}


/**
 * Standard Dekonstruktor.
 * Wird aufgerufen beim loeschen des Objektes.
 * Objekte die im lokalen Kontext von Methoden allogiert wurden, werden
 * beim Bereinigen des Stacks, bei Methodenende automatisch deallokiert.
 * --> Automatischer Dekonstruktoraufruf.
 */
Timer::~Timer() {
    cout << "dtor: Timer" << endl;
}

void Timer::startTimer(){
	timer_settime(timerId_, 0, &timerSpec_, NULL);
}

void Timer::stopTimer(){
	//timer_settime(timerId_, 0, &timerSpec_, &backup_);
    timerSpec_.it_value.tv_sec = 0;
    timerSpec_.it_value.tv_nsec = 0;
    timerSpec_.it_interval.tv_sec = 0;
    timerSpec_.it_interval.tv_nsec = 0;
	timer_settime(timerId_, 0, &timerSpec_, NULL);
}

void Timer::continueTimer(){
	timer_settime(timerId_, 0, &timerSpec_, NULL);
}

void Timer::setMode(enum TimerOptions to){
    switch (to) {
        case TIMER_FAST:
            // Configure Timer to send every x seconds.
            timerSpec_.it_value.tv_sec = SECONDS_FAST;
            timerSpec_.it_value.tv_nsec = NSECONDS_FAST;
            timerSpec_.it_interval.tv_sec = INTERVAL_SECONDS_FAST;
            timerSpec_.it_interval.tv_nsec = INTERVAL_NSECONDS_FAST;
            break;
        case TIMER_SLOW:
            // Configure Timer to send every x seconds.
            timerSpec_.it_value.tv_sec = SECONDS_SLOW;
            timerSpec_.it_value.tv_nsec = NSECONDS_SLOW;
            timerSpec_.it_interval.tv_sec = INTERVAL_SECONDS_SLOW;
            timerSpec_.it_interval.tv_nsec = INTERVAL_NSECONDS_SLOW;
            break;
        default:
            break;
    }
}
