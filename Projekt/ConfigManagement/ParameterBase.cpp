/*
 * ParameterBase.cpp
 *
 *  Created on: 10.10.2016
 *      Author: marvin
 */

#include <ConfigManagement/ParameterBase.h>

ParameterBase::ParameterBase(std::string n): name(n),plist(ParameterList::getParameterList())
{


}

ParameterBase::~ParameterBase()
{
    // TODO Auto-generated destructor stub
}

