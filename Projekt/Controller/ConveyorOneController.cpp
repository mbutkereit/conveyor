
/*
 * ConveyorOneController.cpp
 *
 *  Created on: 05.10.2016
 *      Author: marvin
 */

#include <Controller/ConveyorOneController.h>

ConveyorOneController::ConveyorOneController()
{
    // TODO Auto-generated constructor stub

}

ConveyorOneController::~ConveyorOneController()
{
    // TODO Auto-generated destructor stub
}

void ConveyorOneController::action(){
   // Blink_Thread th1(5); // Thread 1, soll 5 mal blinken
    //Blink_Thread th2(3); // Thread 2, soll 3 mal blinken

    //th1.start(NULL);     // Start Thread 1
    //th2.start(NULL);     // Start Thread 2

   // th2.join();          // Warten auf das Ende von Thread 1
    //th1.join();
}

void ConveyorOneController::shutdown(){

}

