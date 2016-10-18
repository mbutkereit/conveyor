/*
 * Logger.h
 *
 *  Created on: 09.10.2016
 *      Author: marvin
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include <iostream>
#include <fstream>

#define LOG_DEBUG  \
    if(Logger::getLogger().getLoggingLevel() < DEBUG);   \
    else Logger::getLogger().log()

#define LOG_WARNING  \
if(Logger::getLogger().getLoggingLevel() < WARNING);  \
else Logger::getLogger().log()

#define LOG_ERROR  \
if(Logger::getLogger().getLoggingLevel() < ERROR);  \
else Logger::getLogger().log()

enum LOG_LEVEL
{
    ERROR, WARNING, DEBUG
};

class Logger
{
public:
    static Logger& getLogger()
    {
        static Logger instances;
        return instances;
    }
    std::ofstream& log();
    void setLoggingLevel(LOG_LEVEL level);
    LOG_LEVEL getLoggingLevel();
private:
    std::ofstream logfile;
    LOG_LEVEL logginglevel_;
    Logger();
    ~Logger();
};

#endif /* LOGGER_H_ */
