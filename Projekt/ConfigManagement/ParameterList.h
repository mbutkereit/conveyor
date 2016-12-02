#ifndef PARAMETERLIST_H_
#define PARAMETERLIST_H_

#include <vector>
#include "ConfigManagement/ParameterBase.h"
#include "Logger/Logger.h"

//todo find a better implementation
class ParameterBase;

/**
 * @file
 * @section DESCRIPTION
 *
 * Eine Klasse um den Motor des Foerderbandes an zu sprechen.
 */
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
