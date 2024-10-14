#include <stdio.h>
#include <stdarg.h>

#include "date.h"
#include "deus.h"

FILE* defaultStream = NULL;

static const char logLevels[LOGLEVEL_AMOUNT][8] = {
    "Info",
    "Warning",
    "Error",
    "Debug",
    "Fatal",
    "Trace"
};

int logToStream(FILE* stream, const LogLevel loglevel, const char* log, ...) {
    if(stream == NULL || log == NULL) return 0;
    char date[20] = {0};
    int rc = 0;
    ISO8601(date, 20);

    rc += fprintf(stream, "[%s] [%s] ", date, logLevels[loglevel]);
    va_list args;
    va_start(args, log);
    rc += vfprintf(stream, log, args);
    va_end(args);
    rc += fprintf(stream, "\n");
    fflush(stream);

    return rc;
}