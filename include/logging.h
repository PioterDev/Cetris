#ifndef LOGGING_H
#define LOGGING_H

/**
 * @brief Logs given message to the specified stream in the following format: "[<Current date in ISO8601> message]"
 * 
 * @param stream stream
 * @param log message to log
 */
void logToStream(FILE* stream, const char* log);


#endif