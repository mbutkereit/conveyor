#include <cstdlib>
#include <iostream>
#include <hw/inout.h>
#include "Thread/ConveyorThread.h"
#include "Thread/SignalHandlerThread.h"
#include "Hal/SerialInterface/Serial.h"
#include "Hal/ISR/InterruptHandler.h"
#include "FSM/ContextTimeMeasurement.h"
#include "Thread/SerialMessageRecvThread.h"

HalBuilder hb;

#define BAND 1

int main(int argc, char *argv[]) {
	Logger::getLogger().setLoggingLevel(DEBUG);
	ConveyorThread c1;
	InfoMessage* message = InfoMessage::getInfoMessage();
	c1.start(NULL);
    c1.join();
    registerISR();
   LOG_DEBUG <<"##########Let's Go!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
   SerialMessageWatchdogThread::notify();

  // message->setBand2NichtFrei();
    SignalHandlerThread thread_automat;
    SerialMessageRecvThread threadRecv;
//   SerialMessageWatchdogThread threadWatchdog;


    thread_automat.start(NULL);
//	threadWatchdog.start(NULL);
	threadRecv.start(NULL);
//	message->setBand2RutscheVoll();





//	while(1){
//		if(message->istBand1RutscheVoll()){
//			LOG_DEBUG <<"Die Rutsche Voll.\n";
//		}
//
//
//		sleep(5);
//	}


//    while(1){
//    	sleep(30);
//    	message->setBand2RutscheLeer();
//    }



//	threadWatchdog.join();
	threadRecv.join();
	thread_automat.join();


    /*
=======


>>>>>>> develop
   	SignalHandlerThread c2;
   	c2.start(NULL);
    c2.join();
    unregisterISR();
<<<<<<< HEAD
    */
    return 0;
}
