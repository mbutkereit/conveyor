#include "Serializer/InfoMessage.h"

InfoMessage::InfoMessage(){
	this->info.version = MESSAGE_VERSION;
    this->info.typ = MESSAGE_TYPE_INFO;
	this->info.info_a = 0;
	this->info.info_b= 0;
	this->info.info_c = 0;
}

InfoMessage::~InfoMessage(){}

bool InfoMessage::istBand1RutscheVoll(){
	return (this->info.info_a & RUTSCHE_VOLL )== 0 ? false : true;

}
bool InfoMessage::istBand2RutscheVoll(){
	return (this->info.info_b & RUTSCHE_VOLL) == 0 ? false : true;
}
bool InfoMessage::istBand2Frei(){
	return (this->info.info_b & BAND_FREI) == 0 ? true : false;
}

void InfoMessage::update(struct info_package_without_ch* infoUpdate){
#if defined BAND && BAND != 1
	this.info.info_a = infoUpdate->info_a;
#endif
#if defined BAND && BAND != 2
	this->info.info_b = infoUpdate->info_b;
#endif
#if defined BAND && BAND != 3
	this->info.info_c = infoUpdate->info_c;
#endif
}

void InfoMessage::setLBinterruptedBit(){
#if defined BAND && BAND == 1
	this->info.info_a = this->info.info_a |LB_INTERRUPTED ;
#endif
#if defined BAND && BAND == 2
	this->info.info_b = this->info.info_b |LB_INTERRUPTED ;
#endif
#if defined BAND && BAND == 3
	this->info.info_c = this->info.info_c |LB_INTERRUPTED ;
#endif
};

bool InfoMessage::isLbNextConveyorInterrupted(){
#if defined BAND && BAND == 1
	if((this->info.info_b & LB_INTERRUPTED) == 0 ){
		return false;
	}else{
		this->info.info_b = this->info.info_b & ~LB_INTERRUPTED ;
		return true;
	}
#endif
#if defined BAND && BAND == 2
	if(this->info.info_c & LB_INTERRUPTED == 0 ){
		return false;
	}else{
		this->info.info_c = this->info.info_c & ~LB_INTERRUPTED ;
		return true;
	}
#endif
#if defined BAND && BAND == 3
	if(this->info.info_a & LB_INTERRUPTED == 0 ){
		return false;
	}else{
		this->info.info_a = this->info.info_a & ~LB_INTERRUPTED ;
		return true;
	}
#endif
};

struct info_package* InfoMessage::getMessage(){
	return &(this->info);
};

void InfoMessage::setBand1RutscheVoll(){
	this->info.info_a = this->info.info_a | RUTSCHE_VOLL;
}

void InfoMessage::setBand1RutscheLeer(){
   this->info.info_a = this->info.info_a & ~RUTSCHE_VOLL;
}

void InfoMessage::setBand2RutscheVoll(){
	this->info.info_b = this->info.info_b | RUTSCHE_VOLL;
}

void InfoMessage::setBand2RutscheLeer(){
	   this->info.info_b = this->info.info_b & ~RUTSCHE_VOLL;
}

void InfoMessage::setBand2Frei(){
	 this->info.info_b = this->info.info_b & ~BAND_FREI;
}

void InfoMessage::setBand2NichtFrei(){
	this->info.info_b = this->info.info_b | BAND_FREI;
}


void InfoMessage::setESTOP(){
#if defined BAND && BAND == 1
	this->info.info_a = this->info.info_a |ESTOP_BIT ;
#endif
#if defined BAND && BAND == 2
	this->info.info_b = this->info.info_b |ESTOP_BIT ;
#endif
#if defined BAND && BAND == 3
	this->info.info_c = this->info.info_c |ESTOP_BIT ;
#endif
}

void InfoMessage::removeESTOP(){
#if defined BAND && BAND == 1
	this->info.info_a = this->info.info_a & ~ESTOP_BIT ;
#endif
#if defined BAND && BAND == 2
	this->info.info_b = this->info.info_b & ~ESTOP_BIT ;
#endif
#if defined BAND && BAND == 3
	this->info.info_c = this->info.info_c & ~ESTOP_BIT ;
#endif
}

bool InfoMessage::isESTOPGedrueckt(){
	if((this->info.info_a & ESTOP_BIT) ||
			(this->info.info_b & ESTOP_BIT) ||
			(this->info.info_c & ESTOP_BIT)){

		this->info.info_a= this->info.info_a | QUITIERUNGS_BIT;
		this->info.info_b= this->info.info_b | QUITIERUNGS_BIT;
		this->info.info_c= this->info.info_c | QUITIERUNGS_BIT;

		return true;
	}
	return false;
}

bool InfoMessage::wurdeUeberallQuitiert(){
	if((this->info.info_a & QUITIERUNGS_BIT )||
			(this->info.info_b & QUITIERUNGS_BIT) ||
			(this->info.info_c & QUITIERUNGS_BIT)){
		return false;
	}
	return true;
}
