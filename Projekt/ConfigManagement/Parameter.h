#ifndef PARAMETER_H_
#define PARAMETER_H_

#include <string>
#include "ConfigManagement/ParameterBase.h"
#include "Logger/Logger.h"

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
