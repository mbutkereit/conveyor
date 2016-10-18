/*
 * ParameterBase.h
 *
 *  Created on: 10.10.2016
 *      Author: marvin
 */

#ifndef PARAMETERBASE_H_
#define PARAMETERBASE_H_

#include <string>
#include "ConfigManagement/ParameterList.h"
//todo find a better solution
class ParameterList;


class ParameterBase
{
public:
    std::string name;
    ParameterBase(std::string n);
    ParameterList& plist;
    virtual void showValue() = 0;
    virtual ~ParameterBase();
};

#endif /* PARAMETERBASE_H_ */
