#include <Logger/Logger.h>
#include <iostream>
#include <fstream>
#include <string>

LOG_LEVEL Logger::getLoggingLevel()
{
    return this->logginglevel_;
}

Logger::Logger(): logginglevel_(ERROR)
{
    logfile.open("logging.txt", std::ios_base::app);

}

Logger::~Logger()
{
    logfile.close();
}

std::ofstream& Logger::log()
{
    logfile << __TIME__;
    return logfile;
}

void Logger::setLoggingLevel(LOG_LEVEL level)
{
    this->logginglevel_ = level;
}
