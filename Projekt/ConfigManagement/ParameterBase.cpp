#include "ConfigManagement/ParameterBase.h"

ParameterBase::ParameterBase(std::string n): name(n),plist(ParameterList::getParameterList()){}

ParameterBase::~ParameterBase(){}
