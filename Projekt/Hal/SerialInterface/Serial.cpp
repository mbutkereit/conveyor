#include "Hal/SerialInterface/Serial.h"

Serial::Serial(string deviceName){
    this->dev_ = deviceName.c_str();
    this->fdesc_ = open(this->dev_, O_RDWR);
    if(this->fdesc_ == -1){
        LOG_ERROR << "Seriale Interface kann nicht geoeffnet werden. \n";
        exit(-1);
    }
    this->config();
}

Serial::~Serial(){
    if( close(this->fdesc_) < 0 ){
        LOG_ERROR << "Seriale Interface kann nicht geschlossen werden. \n";
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
    LOG_DEBUG<< "Serial::recvPacket() Send Packet : " << p->data << "\n" ;
    return 0;
}

int Serial::recvPacket(Packet* p){
    read(this->fdesc_,p, sizeof(Packet));
    LOG_DEBUG<< "Serial::recvPacket() Inhalt der nachricht : " << p->data << "\n" ;
    return -1;
}

