/*
 * ConfigFile.h
 *
 *  Created on: 10.10.2016
 *      Author: marvin
 */

#ifndef CONFIGFILE_H_
#define CONFIGFILE_H_

#include <string>
#include <map>

class ConfigFile {

public:
  ConfigFile(std::string const& configFile);
  void refresehConfig();
};

#endif /* CONFIGFILE_H_ */
