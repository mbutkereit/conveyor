#include <cstdlib>
#include <iostream>
#include <hw/inout.h>
#include "Thread/ConveyorThread.h"
#include "Thread/SignalHandlerThread.h"
#include "Hal/SerialInterface/Serial.h"
#include "Hal/ISR/InterruptHandler.h"
#include "FSM/ContextTimeMeasurement.h"

HalBuilder hb;

int main(int argc, char *argv[]) {
	Logger::getLogger().setLoggingLevel(DEBUG);
	ConveyorThread c1;
	c1.start(NULL);
    c1.join();
    registerISR();
    cerr << "Lets go !\n";

    /*
   	SignalHandlerThread c2;
   	c2.start(NULL);
       // Cleanup
    c2.join();
    unregisterISR();
    */
    return 0;
	/*Packet p;
	p.data=0x08;
	LOG_DEBUG << "Recieve \n";
	Serial* serial_input = new Serial("/dev/ser1");
	serial_input->sendPacket(&p);
*/
	//Serial* serial_output = new Serial("/dev/ser2");
	//serial_output->recvPacket(&p);


	return EXIT_SUCCESS;
}
