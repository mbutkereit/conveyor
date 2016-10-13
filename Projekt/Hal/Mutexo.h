#ifndef MUTEX_H_
#define MUTEX_H_
#include <pthread.h>

class Mutexo {
    private:
        pthread_mutex_t pmutex_;   ///< Mutex der von diesem Lock belegt wird.

    public:
        int lock();
        int unlock();
        Mutexo();
        virtual ~Mutexo();

};

#endif /* MUTEX_H_ */
