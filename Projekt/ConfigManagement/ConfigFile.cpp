/*
 * ConfigFile.cpp
 *
 *  Created on: 10.10.2016
 *      Author: marvin
 */

#include <ConfigManagement/ConfigFile.h>
#include "ConfigFile.h"

#include <fstream>
#include <iostream>
#include <Logger/Logger.h>

#include <stdlib.h>
#include <ConfigManagement/ParameterBase.h>
#include <ConfigManagement/Parameter.h>
#include <ConfigManagement/ParameterList.h>

std::string trim(std::string const& source, char const* delims = " \t\r\n")
{
    std::string result(source);
    std::string::size_type index = result.find_last_not_of(delims);
    if (index != std::string::npos)
        result.erase(++index);

    index = result.find_first_not_of(delims);
    if (index != std::string::npos)
        result.erase(0, index);
    else
        result.erase();
    return result;
}

ConfigFile::ConfigFile(std::string const& configFileName)
{
    std::ifstream file; // neuen Lese-Stream erzeugen
    file.open(configFileName.c_str(), std::ios_base::in); // Datei_1.txt öffnen

    if (!file) // Fehler beim Öffnen?
        LOG_ERROR << "Eingabe-Datei kann nicht geöffnet werden\n";
    else { // falls es geklappt hat ...

        std::string line;
        std::string name;
        std::string value;
        std::string inSection;
        int posEqual;
        LOG_DEBUG << "--------- READ CONFIG ------ \n";
        while (std::getline(file, line)) {

            if (!line.length())
                continue;

            if (line[0] == '#')
                continue;
            if (line[0] == ';')
                continue;

            if (line[0] == '[') {
                inSection = trim(line.substr(1, line.find(']') - 1));
                continue;
            }

            posEqual = line.find('=');
            name = trim(line.substr(0, posEqual));
            value = trim(line.substr(posEqual + 1));
            LOG_DEBUG << "--------- \n";
            LOG_DEBUG << inSection << "\n";
            LOG_DEBUG << name + " :" << value + "\n";
            if (atoi(value.c_str())) {
                LOG_DEBUG <<"int :: "<< atoi(value.c_str()) << "\n";
            }
            if (atol(value.c_str())) {
                LOG_DEBUG <<"Long Int :: "<< atol(value.c_str()) << "\n";
            }

            if (atof(value.c_str())) {
                LOG_DEBUG <<"Double :: "<< atof(value.c_str()) << "\n";
                new Parameter<double>(atof(value.c_str()),"name");

                ParameterList::getParameterList().showParameters();
            }
        }
    }
}

void ConfigFile::refresehConfig(){
    //check change date.
    std::ifstream file; // neuen Lese-Stream erzeugen
    file.open("config.txt", std::ios_base::in); // Datei_1.txt öffnen

    if (!file) // Fehler beim Öffnen?
        LOG_ERROR << "Eingabe-Datei kann nicht geöffnet werden\n";
    else { // falls es geklappt hat ...

        std::string line;
        std::string name;
        std::string value;
        std::string inSection;
        int posEqual;
        LOG_DEBUG << "--------- READ CONFIG ------ \n";
        while (std::getline(file, line)) {

            if (!line.length())
                continue;

            if (line[0] == '#')
                continue;
            if (line[0] == ';')
                continue;

            if (line[0] == '[') {
                inSection = trim(line.substr(1, line.find(']') - 1));
                continue;
            }

            posEqual = line.find('=');
            name = trim(line.substr(0, posEqual));
            value = trim(line.substr(posEqual + 1));
            LOG_DEBUG << "--------- \n";
            LOG_DEBUG << inSection << "\n";
            LOG_DEBUG << name + " :" << value + "\n";
            // todo find a way to store it in a list.
        }
    }
    file.close();
}

