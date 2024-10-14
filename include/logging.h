#ifndef LOGGING_H
#define LOGGING_H


#include "deus.h"

extern FILE* defaultStream;

/**
 * @brief Logs given message to the specified stream in the following format: "[<Current local date in ISO8601> [<log level>] <message>]"
 * 
 * @param stream stream, it is safe to pass NULL here
 * @param logLevel log level
 * @param log message to log in the printf format
 * 
 * @return number of characters written to the stream
 */
int logToStream(FILE* stream, const LogLevel logLevel, const char* log, ...);


#endif