/*
 * ParameterList.cpp
 *
 *  Created on: 10.10.2016
 *      Author: marvin
 */

#include "ConfigManagement/ParameterList.h"
#include <vector>
#include <Logger/Logger.h>

void ParameterList::showParameters()
{
LOG_DEBUG << "Test" << v.size() << "\n";
    for(std::vector<ParameterBase*>::iterator it = v.begin(); it != v.end(); ++it) {
       ParameterBase*  test=  *it;
       LOG_DEBUG << "\n-------\n FOREACH \n ------ \n";
       test->showValue();
    }

}

void ParameterList::addElement(ParameterBase* parameterBase){
    std::vector<ParameterBase*>::iterator it;

    it = this->v.begin();
    this->v.insert(it,parameterBase);
}

