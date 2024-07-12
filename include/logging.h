#ifndef LOGGING_H
#define LOGGING_H


#include "deus.h"

extern char loggingBuffer[loggingBufferSize]; //To be used with snprintf, otherwise just pass a normal char*
extern FILE* defaultStream;

/**
 * @brief Logs given message to the specified stream in the following format: "[<Current date in ISO8601> [<log level>] <message>]"
 * 
 * @param stream stream, it is safe to pass `NULL` here
 * @param logLevel log level
 * @param log message to log, pass `NULL` if loggingBuffer is used
 */
void logToStream(FILE* stream, const LogLevel logLevel, const char* log);


#endif