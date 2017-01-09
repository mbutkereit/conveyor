/**
 * @file    Blink_Thread.h
 * @author  Simon Brummer
 * @version 0.1
 *
 * Headerfile zur Demoklasse fuer das SEP2 Tutorium. Es wird folgendes gezeigt:
 * - Ableiten von Klassen
 * - Sichtbarkeitsregeln fuer Variablen und Methoden
 * - Konstruktorenarten
 * - Doxygenkompatible Kommentare.
 */

#ifndef TIMER_H_
#define TIMER_H_

#define SLOW 249122807
#define FAST 100000000

#define SECONDS_SLOW 0
#define NSECONDS_SLOW SLOW
#define INTERVAL_SECONDS_SLOW 0
#define INTERVAL_NSECONDS_SLOW SLOW

#define SECONDS_FAST 0
#define NSECONDS_FAST FAST
#define INTERVAL_SECONDS_FAST 0
#define INTERVAL_NSECONDS_FAST FAST

#include <stdint.h>
#include <pthread.h>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <sys/neutrino.h>
#include "src/lib/HAWThread.h"
#include "InterruptHandler.h"
#include "TimerOptions.h"

using namespace thread;

extern int isrtConnection_;

class Timer {
    /* Variablen deklarationen */
    private:
        static pthread_mutex_t mtx_; ///< Synchronisationsmutex.
    	// Message Passing specific Variables
    	struct _pulse     pulse_;

    	// Timer specific Variables
    	timer_t           timerId_;
    	struct itimerspec timerSpec_;
    	struct itimerspec backup_;
    	struct sigevent   timerEvent_;

    /* Methoden deklarationen */
    public:
        static Timer* getInstance();
        void startTimer(void);
        void stopTimer(void);
        void continueTimer(void);
        void setMode(enum TimerOptions);
    private:
        Timer(); ///< Standard Konstruktor. Offentlich Aufrufbar.
        virtual ~Timer();      ///< Standard Dekonstruktor.
        Timer(const Timer& b);      ///< Copy-Konstruktor. Privat, deshalb kann dieses Objekt nicht als "Call-by-value" uebergeben werden.
        Timer& operator=(Timer& b); ///< Zuweisungskonstruktor. Objekt kann nicht an eine gleichartige Referenz zugewiesen werden da private.
};

#endif
