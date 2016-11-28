/*
 * ContextMotor.cpp
 *
 *  Created on: 28.11.2016
 *      Author: abs949
 */

#include "ContextMotor.h"

ContextMotor* ContextMotor::instance_ = NULL;

ContextMotor* ContextMotor::getInstance() {
	if (instance_ == NULL) {
		instance_ = new ContextMotor();
	}
	return instance_;
}
