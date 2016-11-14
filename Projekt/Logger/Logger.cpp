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
    logfile << __TIME__ << ' ';
    return logfile;
}

void Logger::setLoggingLevel(LOG_LEVEL level)
{
    this->logginglevel_ = level;
}
