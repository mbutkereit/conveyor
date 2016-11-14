#include "InterruptHandler.h"

const struct sigevent* ISR_DIO(void* arg, int id) {
    struct sigevent* event = (struct sigevent*) arg;
    Hardware* hw = hb.getHardware();
    int code = 0;

    // Hilfs Variable um Positive und Negative Interrupts ab zu fangen.
    static char interruptflags = 0;

    // Lichtschranken Interrupts.

    if(hw->getMT()->isItemRunningIn()==0 && (interruptflags & IS_RUNNING_IN_STATE) == 0){
      code=code+LIGHT_BARRIER_BEGIN_INTERRUPTED;
      interruptflags=interruptflags | IS_RUNNING_IN_STATE;
    }

    if(hw->getMT()->isItemRunningIn()!=0 && (interruptflags & IS_RUNNING_IN_STATE) != 0){
      code=code+LIGHT_BARRIER_BEGIN_NOT_INTERRUPTED;
      interruptflags=interruptflags-IS_RUNNING_IN_STATE;
    }

    if(hw->getMT()->isItemAltimetry()==0 && (interruptflags & IS_IN_ALTIMETRY_STATE) == 0){
      code=code+LIGHT_BARRIER_ALTIMETRY_INTERRUPTED;
      interruptflags=interruptflags | IS_IN_ALTIMETRY_STATE;
    }

    if(hw->getMT()->isItemAltimetry()!=0 && (interruptflags & IS_IN_ALTIMETRY_STATE) != 0){
      code=code+LIGHT_BARRIER_ALTIMETRY_NOT_INTERRUPTED;
      interruptflags=interruptflags-IS_IN_ALTIMETRY_STATE;
    }

    if(hw->getMT()->isItemSwitch()==0 && (interruptflags & IS_IN_SWITCH_STATE) == 0){
      code=code+LIGHT_BARRIER_SWITCH_INTERRUPTED;
      interruptflags=interruptflags | IS_IN_SWITCH_STATE;
    }

    if(hw->getMT()->isItemSwitch()!=0 && (interruptflags & IS_IN_SWITCH_STATE) != 0){
      code=code+LIGHT_BARRIER_SWITCH_NOT_INTERRUPTED;
      interruptflags=interruptflags-IS_IN_SWITCH_STATE;
    }


    if(hw->getMT()->isSkidFull()==0 && (interruptflags & IS_SKID_FULL_STATE) == 0){
      code=code+LIGHT_BARRIER_SKID_INTERRUPTED;
      interruptflags=interruptflags | IS_SKID_FULL_STATE;
    }

    if(hw->getMT()->isSkidFull()!=0 && (interruptflags & IS_SKID_FULL_STATE) != 0){
      code=code+LIGHT_BARRIER_SKID_NOT_INTERRUPTED;
      interruptflags=interruptflags-IS_SKID_FULL_STATE;
    }

    if(hw->getMT()->isItemRunningOut()==0 && (interruptflags & IS_RUNNING_OUT_STATE) == 0){
      code=code+LIGHT_BARRIER_END_INTERRUPTED;
      interruptflags=interruptflags | IS_RUNNING_OUT_STATE;
    }

    if(hw->getMT()->isItemRunningOut()!=0 && (interruptflags & 0x10) != 0){
      code=code+LIGHT_BARRIER_END_NOT_INTERRUPTED;
      interruptflags=interruptflags-0x10;
    }

// Button Interrupts.

    if(hw->getHMI()->isButtonEStopPressed() == 0){
      code=code+ESTOP;
    }
    if(hw->getHMI()->isButtonResetPressed() > 0){
    	code=code+RESET;
    }
    if(hw->getHMI()->isButtonStartPressed() > 0){
    	code=code+START;
    }
    if(hw->getHMI()->isButtonStopPressed() == 0){
    	code=code+STOP;
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
    	code = code + ALTIMETRY_COMPLETED;
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
