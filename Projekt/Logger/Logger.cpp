#include "Logger/Logger.h"

LOG_LEVEL Logger::getLoggingLevel()
{
    return this->logginglevel_;
}

Logger::Logger(): logginglevel_(ERROR)
{
   logfile.open("/tmp/wi_logging.txt", std::ios_base::app);

}

Logger::~Logger()
{
    logfile.close();
}

std::ofstream& Logger::log()
{
    this->mutex.lock();
    logfile << this->currentDateTime() << ' ';
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
