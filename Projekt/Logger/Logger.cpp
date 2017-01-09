#include "Logger/Logger.h"
#include <iostream>

LOG_LEVEL Logger::getLoggingLevel()
{
    return this->logginglevel_;
}

Logger::Logger(): logginglevel_(ERROR)
{

std::cout <<"Status des Logfiles:" << logfile.is_open();
   logfile.open("/tmp/wi_logging.txt", std::ios_base::app);
   std::cout <<"Status des Logfiles:" << logfile.is_open();


}

Logger::~Logger()
{
    logfile.close();
}

std::ofstream& Logger::log()
{
    this->mutex.lock();
    logfile << this->currentDateTime() << ' ';
    logfile.flush();
    this->mutex.unlock();

    return logfile;
}

void Logger::setLoggingLevel(LOG_LEVEL level)
{
    this->logginglevel_ = level;
}

std::string Logger::currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

    return buf;
}
