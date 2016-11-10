/*
 * Parameter.cpp
 *
 *  Created on: 10.10.2016
 *      Author: marvin


#include <ConfigManagement/Parameter.h>
#include <string>
#include <Logger/Logger.h>

template<class T>
Parameter<T>::Parameter(T v, std::string name) :
        value(v), ParameterBase(name)
{
    this->plist.addElement(this);
}

template<class T>
Parameter<T>::~Parameter()
{
    // TODO Auto-generated destructor stub
}

template<class T>
void Parameter<T>::showValue()
{
    LOG_DEBUG << "\n--------- \n Der wert ist :" << this->value ;
    LOG_DEBUG << "\n Der name ist: " << this->name;
}

template class Parameter<double> ;
template class Parameter<std::string>; */

