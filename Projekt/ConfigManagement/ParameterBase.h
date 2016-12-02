#ifndef PARAMETERBASE_H_
#define PARAMETERBASE_H_

#include <string>
#include "ConfigManagement/ParameterList.h"
//todo find a better solution
class ParameterList;

/**
 * @file
 * @section DESCRIPTION
 *
 * Eine Klasse um den Motor des Foerderbandes an zu sprechen.
 */
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
