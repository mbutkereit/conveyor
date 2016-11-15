#include "Hal/Mutexo.h"

Mutexo::Mutexo(){
	pmutex_ = PTHREAD_MUTEX_INITIALIZER;
}

Mutexo::~Mutexo(){}

int Mutexo::lock() {
    return pthread_mutex_lock(&pmutex_);

}

int Mutexo::unlock() {
    return pthread_mutex_unlock(&pmutex_);
}
