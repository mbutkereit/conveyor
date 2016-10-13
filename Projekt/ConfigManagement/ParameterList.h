/*
 * ParameterList.h
 *
 *  Created on: 10.10.2016
 *      Author: marvin
 */

#ifndef PARAMETERLIST_H_
#define PARAMETERLIST_H_

#include <vector>
#include "ConfigManagement/ParameterBase.h"
//todo find a better implementation
class ParameterBase;

class ParameterList
{
public:
    static ParameterList& getParameterList()
    {
        static ParameterList plist;
        return plist;
    }
    void showParameters();
    void addElement(ParameterBase* parameterBase);
private:
    std::vector<ParameterBase*> v;
};

#endif /* PARAMETERLIST_H_ */
