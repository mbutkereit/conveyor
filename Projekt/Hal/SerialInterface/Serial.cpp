/*
 * Serial.cpp
 *
 *  Created on: 11.10.2016
 *      Author: marvin
 */

#include <SerialInterface/Serial.h>
Serial::Serial(){
    // Open File descriptor
    this->dev_ = "/dev/ser1";
    this->fdesc_ = open(this->dev_, O_RDWR);
    if(this->fdesc_ == -1){
        exit(-1);
    }

    // Configure Serial Interface
    this->config();
}

Serial::~Serial(){
    // Close File descriptor
    if( close(this->fdesc_) < 0 ){
        exit(-1);
    }
}

void Serial::config(void){
    struct termios ts;
    tcflush(this->fdesc_, TCIOFLUSH);
    tcgetattr(this->fdesc_, &ts);
    cfsetispeed(&ts, B19200);
    cfsetospeed(&ts, B19200);
    ts.c_cflag &= ~CSIZE;
    ts.c_cflag &= ~CSTOPB;
    ts.c_cflag &= ~PARENB;
    ts.c_cflag |= CS8;
    ts.c_cflag |= CREAD;
    ts.c_cflag |= CLOCAL;
    tcsetattr(this->fdesc_, TCSANOW, &ts);
}

int Serial::sendPacket(Packet* p){
    write(this->fdesc_, p, sizeof(Packet));
    return 0;
}

int Serial::recvPacket(Packet* p){
    if( readcond(this->fdesc_, p, sizeof(Packet), sizeof(Packet), 0, 0) > 0) {
        return 0;
    }
    return -1;
}

