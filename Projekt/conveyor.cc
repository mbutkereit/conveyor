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

int main(int argc, char *argv[]) {
	Logger::getLogger().setLoggingLevel(DEBUG);
	ConveyorThread c1;
	InfoMessage* message = InfoMessage::getInfoMessage();
	c1.start(NULL);
    c1.join();
    registerISR();
   LOG_DEBUG <<"##########Let's Go!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
 //  SerialMessageWatchdogThread::notify();

    SignalHandlerThread thread_automat;
   SerialMessageRecvThread threadRecv;
   SerialMessageWatchdogThread threadWatchdog;


    thread_automat.start(NULL);
	threadWatchdog.start(NULL);
	threadRecv.start(NULL);

    LOG_DEBUG <<"##########Let's Go!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";

    sleep(5);





//	while(1){
//		message->InhaltdesPaketesausgeben();
//
//		if(message->istBand1RutscheVoll()){
//			LOG_DEBUG <<"####################Die Rutsche 1 ist Voll.\n";
//		}

//	}


//    while(1){
//    	sleep(5);
//    	if(message->istBand1RutscheVoll()){
//    		 LOG_DEBUG <<"Die Rutsche von Band 1 ist voll!!!!!!!!\n";
//    	}
//
//    }




	threadWatchdog.join();
	threadRecv.join();
	thread_automat.join();


    /*


>>>>>>> develop
   	SignalHandlerThread c2;
   	c2.start(NULL);
    c2.join();
    unregisterISR();
<<<<<<< HEAD
    */
    return 0;
}
