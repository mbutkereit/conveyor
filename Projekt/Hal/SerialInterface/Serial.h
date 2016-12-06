#ifndef SERIAL_H_
#define SERIAL_H_

#include <iostream>
#include <stdint.h>
#include <fcntl.h>
#include <termios.h>
#include <stdlib.h>
#include <string.h>
#include "Logger/Logger.h"
#include "Hal/Mutexo.h"

using namespace std;

typedef struct {
    uint8_t data;
}Packet;

/**
 * @file
 * @section DESCRIPTION
 *
 * Eine Klasse um den Motor des Foerderbandes an zu sprechen.
 */
class Serial{

private:
    const char* dev_write_;
    const char* dev_read_;
    int fdesc_read_;
    int fdesc_write_;

public:
    Serial(string deviceNameWrite,string deviceNameRead);
    ~Serial();

    void config(void);

    int sendPacket(void* p, int size);

    int recvPacket(void* p, int size);

private:

    Serial(const Serial& other);
    Mutexo mutex;
    Serial& operator=(const Serial& other);
};

#endif
