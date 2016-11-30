/**
 * @file    Lock.cpp
 * @author  Simon Brummer
 * @version 0.1
 *
 * Einfache implementierung des scoped locking patterns.
 * Der Konstruktoraufruf belegt einen Mutex, der
 * Dekonstruktor gibt den Mutex frei.
 */

#include <pthread.h>
#include "Hal/Mutexo.h"


Mutexo::Mutexo(){
	pmutex_ = PTHREAD_MUTEX_INITIALIZER;
}

Mutexo::~Mutexo(){

}

int Mutexo::lock() {
    return pthread_mutex_lock(&pmutex_);
}

int Mutexo::unlock() {
    return pthread_mutex_unlock(&pmutex_);
}



