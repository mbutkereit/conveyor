/*
 * ContextMotor.cpp
 *
 *  Created on: 29.11.2016
 *      Author: abs949
 */

#include "ContextMotor.h"
using namespace std;

ContextMotor* ContextMotor::instance_ = NULL;

ContextMotor* ContextMotor::getInstance() {
	if (instance_ == NULL) {
		instance_ = new ContextMotor(ContextTimer::getInstance());
	}
	return instance_;
}
