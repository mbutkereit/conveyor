#include "Hal/SerialInterface/Serial.h"

Serial::Serial(string deviceNameWrite,string deviceNameRead){
    this->dev_write_ = deviceNameWrite.c_str();
    this->fdesc_write_ = open(this->dev_write_, O_RDWR);

    if(this->fdesc_write_ == -1){
        LOG_ERROR << "Seriale Interface kann nicht geoeffnet werden:" <<deviceNameWrite <<"\n";
        exit(-1);
    }

    this->dev_read_ = deviceNameRead.c_str();
    this->fdesc_read_ = open(this->dev_read_, O_RDWR);
    if(this->fdesc_read_ == -1){
        LOG_ERROR << "Seriale Interface kann nicht geoeffnet werden:" << deviceNameRead<<"\n";
        exit(-1);
    }
    this->config();
}

Serial::~Serial(){
    if( close(this->fdesc_read_) < 0 ){
        LOG_ERROR << "Seriale Interface Read kann nicht geschlossen werden. \n";
        exit(-1);
    }
    if( close(this->fdesc_write_) < 0 ){
        LOG_ERROR << "Seriale Interface Write kann nicht geschlossen werden. \n";
        exit(-1);
    }
}

void Serial::config(void){
	// Initialisieren der serialen schnittstelle zum schreiben.
    struct termios ts_write;
    tcflush(this->fdesc_write_, TCIOFLUSH);
    tcgetattr(this->fdesc_write_, &ts_write);
    cfsetispeed(&ts_write, B19200);
    cfsetospeed(&ts_write, B19200);
    ts_write.c_cflag &= ~CSIZE;
    ts_write.c_cflag &= ~CSTOPB;
    ts_write.c_cflag &= ~PARENB;
    ts_write.c_cflag |= CS8;
    ts_write.c_cflag |= CREAD;
    ts_write.c_cflag |= CLOCAL;
    tcsetattr(this->fdesc_write_, TCSANOW, &ts_write);

    struct termios ts_read;
    tcflush(this->fdesc_read_, TCIOFLUSH);
    tcgetattr(this->fdesc_read_, &ts_read);
    cfsetispeed(&ts_read, B19200);
    cfsetospeed(&ts_read, B19200);
    ts_read.c_cflag &= ~CSIZE;
    ts_read.c_cflag &= ~CSTOPB;
    ts_read.c_cflag &= ~PARENB;
    ts_read.c_cflag |= CS8;
    ts_read.c_cflag |= CREAD;
    ts_read.c_cflag |= CLOCAL;
    tcsetattr(this->fdesc_read_, TCSANOW, &ts_read);
}




int Serial::sendPacket(void* p,int size){
	this->mutex.lock();
    write(this->fdesc_write_, p, size);
    LOG_DEBUG<< "Serial::recvPacket() Send Packet : " << "\n" ;
	this->mutex.unlock();
    return 0;
}

int Serial::recvPacket(void* p,int size){
    read(this->fdesc_read_,p, size);
    LOG_DEBUG<< "Serial::recvPacket() Inhalt der nachricht : " << "\n" ;
    return 0;
}

