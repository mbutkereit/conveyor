#ifndef MUTEX_H_
#define MUTEX_H_

#include <pthread.h>


/**
 * @file
 * @section DESCRIPTION
 *
 * Eine Klasse um einen Mutex zu erstellen.
 */
class Mutexo {
    private:
        pthread_mutex_t pmutex_;   ///< Mutex der von diesem Lock belegt wird.

    public:

        /**
         * Locked den Mutex.
         *
         * @return Gibt den erfolg des Lockes zurück.
         */
        int lock();

        /**
         * Unlocked den Mutex.
         *
         * @return  Gibt den erfolg des Unlockes zurück.
         */
        int unlock();

        /**
         *  Constructor des Mutex.
         */
        Mutexo();

        /**
         *  Destructor des Mutex.
         */
        virtual ~Mutexo();

};

#endif /* MUTEX_H_ */
