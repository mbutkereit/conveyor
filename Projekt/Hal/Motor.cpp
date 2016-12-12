#include "Hal/Motor.h"

Motor::Motor(Adapter* adapt):adapter(adapt)
{
	motorrightBitmask__ = 0x01;
	motorleftBitmask_ = 0x02;
	motorstoppBitmask_ = 0x08;
	motorlangsamBitmask_ = 0x04;
	weicheaufBitmask_ = 0x10;
}

Motor::~Motor(){}

uint8_t Motor::right(){
	adapter->removeBitMask(motorstoppBitmask_);
	adapter->removeBitMask(motorleftBitmask_);

	adapter->setBitMask(motorrightBitmask__);
    LOG_DEBUG << " Motor bewegt sich nach rechts. \n";

    return 0;

}
uint8_t Motor::left(){
	adapter->removeBitMask(motorstoppBitmask_);
	adapter->removeBitMask(motorrightBitmask__);

	adapter->setBitMask(motorleftBitmask_);
    LOG_DEBUG << " Motor bewegt sich nach links. \n";

    return 0;
}
uint8_t Motor::slow(){
	adapter->removeBitMask(motorstoppBitmask_);

	adapter->setBitMask(motorlangsamBitmask_);
    LOG_DEBUG << " Motor bewegt sich langsam. \n";

    return 0;
}
uint8_t Motor::fast(){
	adapter->removeBitMask(motorstoppBitmask_);
	adapter->removeBitMask(motorlangsamBitmask_);
    LOG_DEBUG << "Der Motor l�uft jetzt schnell. \n";

    return 0;
}
uint8_t Motor::stop(){
	adapter->setBitMask(motorstoppBitmask_);
    LOG_DEBUG << " Motor stoppt.";

    return 0;
}
uint8_t Motor::openSwitch(){
	adapter->setBitMask(weicheaufBitmask_);
    LOG_DEBUG << "Die Weiche ist jetzt offen. \n";

    return 0;
}
uint8_t Motor::closedSwitch(){
	adapter->removeBitMask(weicheaufBitmask_);
    LOG_DEBUG << "Die Weiche ist jetzt geschlossen. \n";

    return 0;
}

