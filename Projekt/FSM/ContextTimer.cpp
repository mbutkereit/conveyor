/*
 * ContextTimer.cpp
 *
 *  Created on: 28.11.2016
 *      Author: abs949
 */

#include "ContextTimer.h"

ContextTimer* ContextTimer::instance_ = NULL;

ContextTimer* ContextTimer::getInstance() {
	if (instance_ == NULL) {
		instance_ = new ContextTimer();
	}
	return instance_;
}

