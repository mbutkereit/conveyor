#ifndef LOGGER_H_
#define LOGGER_H_

#include <iostream>
#include <fstream>
#include <string>

/// This is an enum class
#define LOG_DEBUG  \
    if(Logger::getLogger().getLoggingLevel() < DEBUG);   \
    else Logger::getLogger().log()

/// This is an enum class
#define LOG_WARNING  \
if(Logger::getLogger().getLoggingLevel() < WARNING);  \
else Logger::getLogger().log()

/// This is an enum class
#define LOG_ERROR  \
if(Logger::getLogger().getLoggingLevel() < ERROR);  \
else Logger::getLogger().log()

/// This is an enum class
enum LOG_LEVEL
{
    ERROR, ///<
    WARNING, ///<
    DEBUG ///<
};

/**
 * @file
 * @section DESCRIPTION
 *
 * Eine Klasse um den Adapter(Ports) zugriff zu Managen.
 */
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
    std::ofstream logfile; ///<
    LOG_LEVEL logginglevel_; ///<

    Logger();
    ~Logger();
};

#endif /* LOGGER_H_ */
