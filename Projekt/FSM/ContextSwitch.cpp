/*
 * ContextSwitch.cpp
 *
 *  Created on: 28.11.2016
 *      Author: abs949
 */

#include "ContextSwitch.h"

ContextSwitch* ContextSwitch::instance_ = NULL;

ContextSwitch* ContextSwitch::getInstance() {
	if (instance_ == NULL) {
		instance_ = new ContextSwitch();
	}
	return instance_;
}

