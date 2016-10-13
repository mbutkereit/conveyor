/*
 * Parameter.h
 *
 *  Created on: 10.10.2016
 *      Author: marvin
 */

#ifndef PARAMETER_H_
#define PARAMETER_H_
#include "ConfigManagement/ParameterBase.h"
#include <string>
template<class T>
class Parameter : public ParameterBase
{
public:
    T value;
    Parameter(T v ,std::string name);
    ~Parameter();
    void showValue();
    operator T() const
    {
        return value;
    } // cast
};
#endif /* PARAMETER_H_ */
