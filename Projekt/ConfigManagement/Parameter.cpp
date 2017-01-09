/*
#include "ConfigManagement/Parameter.h"

template<class T>
Parameter<T>::Parameter(T v, std::string name) :
        value(v), ParameterBase(name)
{
    this->plist.addElement(this);
}

template<class T>
Parameter<T>::~Parameter(){}

template<class T>
void Parameter<T>::showValue()
{
    LOG_DEBUG << "\n--------- \n Der wert ist :" << this->value ;
    LOG_DEBUG << "\n Der name ist: " << this->name;
}

template class Parameter<double> ;
template class Parameter<std::string>; */

