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

#ifndef BLINK_THREAD_H_
#define BLINK_THREAD_H_

#include <stdint.h>
#include <pthread.h>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <sys/neutrino.h>
#include "src/lib/HAWThread.h"

using namespace thread;

class Timer_Thread: public HAWThread {
    /* Variablen deklarationen */
    private:
        uint32_t const seconds_ ;    ///< Anzahl der Sekunden.
        uint32_t const nseconds_;    ///< Anzahl der Nanosekunden.
        uint32_t const intervalSeconds_; ///< Intervall in Sekundenzahl
        uint32_t const intervalNseconds_;///< Intervall in Nanosekundenanzahl
        uint32_t* tickCount_;    ///< Enthält die Anzahl der Ticks
        static pthread_mutex_t mtx_; ///< Synchronisationsmutex.
    	// Message Passing specific Variables
    	int channel_;
    	int connection_;
    	bool timerPaused_;
    	struct _pulse     pulse_;

    	// Timer specific Variables
    	timer_t           timerId_;
    	struct itimerspec timerSpec_;
    	struct itimerspec backup_;
    	struct sigevent   timerEvent_;

    /* Methoden deklarationen */
    public:
        Timer_Thread(uint32_t, uint32_t, uint32_t, uint32_t, uint32_t*); ///< Standard Konstruktor. Offentlich Aufrufbar.
        virtual ~Timer_Thread();      ///< Standard Dekonstruktor.
        void startTimer(void);
        void stopTimer(void);
        void continueTimer(void);
    private:
        Timer_Thread(const Timer_Thread& b);      ///< Copy-Konstruktor. Privat, deshalb kann dieses Objekt nicht als "Call-by-value" uebergeben werden.
        Timer_Thread& operator=(Timer_Thread& b); ///< Zuweisungskonstruktor. Objekt kann nicht an eine gleichartige Referenz zugewiesen werden da private.

        virtual void execute(void*); ///< Geerbt aus HAWThread. Muss implementiert werden.
        virtual void shutdown();     ///< Geerbt aus HAWThread. Muss implementiert werden.
};

#endif
