#ifndef SERIAL_H_
#define SERIAL_H_

#include <iostream>
#include <stdint.h>
#include <fcntl.h>
#include <termios.h>
#include <stdlib.h>
#include <string.h>
#include <Logger/Logger.h>

using namespace std;

typedef struct {
    uint8_t data;
}Packet;

class Serial{
private:
    const char* dev_;
    int fdesc_;

public:
    Serial(string deviceName);
    ~Serial();
    void config(void);
    int sendPacket(Packet* p);
    int recvPacket(Packet* p);

private:
    Serial(const Serial& other);
    Serial& operator=(const Serial& other);
};

#endif
