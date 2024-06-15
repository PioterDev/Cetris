#include <stdio.h>

#include "date.h"
#include "deus.h"

char loggingBuffer[loggingBufferSize];

void logToStream(FILE* stream, const LogLevel loglevel, const char* log) {
    if(stream == NULL)return;
    char date[20] = {0};
    char level[8] = {0};
    switch(loglevel) {
        case LOGLEVEL_INFO:
            strcpy(level, "Info");
            break;
        case LOGLEVEL_WARNING:
            strcpy(level, "Warning");
            break;
        case LOGLEVEL_ERROR:
            strcpy(level, "Error");
            break;
        case LOGLEVEL_DEBUG:
            strcpy(level, "Debug");
            break;
        default:
            strcpy(level, "Unknown");
            break;
    }
    ISO8601(date, sizeof(date));
    if(log == NULL) fprintf(stream, "[%s] [%s] %s\n", date, level, loggingBuffer);
    else fprintf(stream, "[%s] [%s] %s\n", date, level, log);
    fflush(stream);
}