#include <stdio.h>

#include "date.h"

//TODO: add log level enum (log, warn, error, etc.)
void logToStream(FILE* stream, const char* log) {
    char date[20] = {0};
    ISO8601(date, sizeof(date));
    fprintf(stream, "[%s] %s\n", date, log);
}