#include <stdio.h>

#include "date.h"

void logToStream(FILE* stream, const char* log, const LogLevel loglevel) {
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
            strcpy(level, "Info");
            break;
    }
    ISO8601(date, sizeof(date));
    fprintf(stream, "[%s] [%s] %s\n", date, level, log);
}