/*
 * ContextSorting.cpp
 *
 *  Created on: 23.11.2016
 *      Author: abs949
 */
#include "ContextSorting.h"
using namespace std;

ContextSorting* ContextSorting::instance_ = NULL;

ContextSorting* ContextSorting::getInstance() {
	if (instance_ == NULL) {
		instance_ = new ContextSorting();
	}
	return instance_;
}



