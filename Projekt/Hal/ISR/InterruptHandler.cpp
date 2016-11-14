#include "InterruptHandler.h"

InterruptHandler::InterruptHandler() {}

InterruptHandler::~InterruptHandler() {}

const struct sigevent* ISR_DIO(void* arg, int id) {
    struct sigevent* event = (struct sigevent*) arg;
    Hardware* hw = hb.getHardware();
    int code = 0;

    // Hilfs Variable um Positive und Negative Interrupts ab zu fangen.
    static char interruptflags = 0;

    // Lichtschranken Interrupts.

    if(hw->getMT()->isItemRunningIn()==0 && (interruptflags & 0x1) == 0){
      code=code+1;
      interruptflags=interruptflags | 0x1;
    }

    if(hw->getMT()->isItemRunningIn()!=0 && (interruptflags & 0x1) != 0){
      code=code+2;
      interruptflags=interruptflags-0x1;
    }

    if(hw->getMT()->isItemAltimetry()==0 && (interruptflags & 0x2) == 0){
      code=code+4;
      interruptflags=interruptflags | 0x2;
    }

    if(hw->getMT()->isItemAltimetry()!=0 && (interruptflags & 0x2) != 0){
      code=code+8;
      interruptflags=interruptflags-0x2;
    }

    if(hw->getMT()->isItemSwitch()==0 && (interruptflags & 0x4) == 0){
      code=code+16;
      interruptflags=interruptflags | 0x4;
    }

    if(hw->getMT()->isItemSwitch()!=0 && (interruptflags & 0x4) != 0){
      code=code+32;
      interruptflags=interruptflags-0x4;
    }


    if(hw->getMT()->isSkidFull()==0 && (interruptflags & 0x8) == 0){
      code=code+64;
      interruptflags=interruptflags | 0x8;
    }

    if(hw->getMT()->isSkidFull()!=0 && (interruptflags & 0x8) != 0){
      code=code+128;
      interruptflags=interruptflags-0x8;
    }

    if(hw->getMT()->isItemRunningOut()==0 && (interruptflags & 0x10) == 0){
      code=code+256;
      interruptflags=interruptflags | 0x10;
    }

    if(hw->getMT()->isItemRunningOut()!=0 && (interruptflags & 0x10) != 0){
      code=code+512;
      interruptflags=interruptflags-0x10;
    }

// Button Interrupts.

    if(hw->getHMI()->isButtonEStopPressed()==0){
      code=code+1024;
    }
    if(hw->getHMI()->isButtonResetPressed() > 0){
    	code=code+2048;
    }
    if(hw->getHMI()->isButtonStartPressed() > 0){
    	code=code+4096;
    }
    if(hw->getHMI()->isButtonStopPressed() ==0){
    	code=code+8192;
    }

    //Hinweise:
    //http://www.qnx.com/developers/docs/660/index.jsp?topic=%2Fcom.qnx.doc.neutrino.getting_started%2Ftopic%2Fs1_timer_how_sigev.html

    event->sigev_notify = SIGEV_PULSE;
    event->sigev_value.sival_int = code;

    out8(INTERRUPT_RESET_DIO, 0);

    if(code != 0){
    	return event;
    }

    return NULL;
}

const struct sigevent* ISR_AIO(void* arg, int id) {
    struct sigevent* event = (struct sigevent*) arg;
    event->sigev_notify = SIGEV_PULSE;
    int code = 0;

    if(in8(0x321) & 0x80){
    	code = code + 16384;
    }

    event->sigev_value.sival_int = code;
    out8(0x321,0b00000100);
    out8(INTERRUPT_RESET_AIO, 0);

    if(code != 0){
    	return event;
    }

    return NULL;
}
void registerISR(void){

    if (ThreadCtl(_NTO_TCTL_IO_PRIV, 0) == -1){
        exit(EXIT_FAILURE);
    }
    if (( isrtChannel_ = ChannelCreate(0)) == -1){
        exit(EXIT_FAILURE);
    }

    if ((isrtConnection_ = ConnectAttach(0, 0, isrtChannel_, 0, 0)) == -1){
        exit(EXIT_FAILURE);
    }

    out8(INTERRUPT_RESET_DIO, 0);
    out8(0x30B, 0b11111001);

    SIGEV_PULSE_INIT(&isrtEvent_, isrtConnection_, SIGEV_PULSE_PRIO_INHERIT, 0, 0);
    coid = InterruptAttach(11, ISR_DIO, &isrtEvent_, sizeof(isrtEvent_), 0);
    if (coid == -1) {
        exit(EXIT_FAILURE);
    }

    out8(INTERRUPT_RESET_AIO, 0);
    out8(0x321, 0b10000100);

    SIGEV_PULSE_INIT(&isrtEvent_2, isrtConnection_, SIGEV_PULSE_PRIO_INHERIT, 0, 0);
    coid2 = InterruptAttach(14, ISR_AIO, &isrtEvent_2, sizeof(isrtEvent_2), 0);
    if (coid2 == -1) {
        exit(EXIT_FAILURE);
    }
}

void unregisterISR(void){
    if( InterruptDetach(coid) == -1 ){
        exit(EXIT_FAILURE);
    }

    out8(0x30B, 0b11111111);
    out8(INTERRUPT_RESET_DIO,0);

    if( InterruptDetach(coid2) == -1 ){
        exit(EXIT_FAILURE);
    }

    out8(0x321,  0b11111111);
    out8(INTERRUPT_RESET_AIO, 0);
}
