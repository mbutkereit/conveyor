#ifndef CONFIGFILE_H_
#define CONFIGFILE_H_

#include <string>
#include <map>
#include <fstream>
#include <iostream>
#include <stdlib.h>

#include "Logger/Logger.h"
#include "ConfigManagement/ParameterBase.h"
#include "ConfigManagement/Parameter.h"
#include "ConfigManagement/ParameterList.h"

/**
 * @file
 * @section DESCRIPTION
 *
 * Eine Klasse um den Motor des Foerderbandes an zu sprechen.
 */
class ConfigFile {

public:
  ConfigFile(std::string const& configFile);
  void refresehConfig();
};

#endif /* CONFIGFILE_H_ */
