#include <windows.h>

#include "structs_unions.h"

DWORD WINAPI highPerformanceClockThread(void* params) {
    clockThreadParameters* parameters = params;
    LARGE_INTEGER* timer = parameters->timer;

    while(*(parameters->clockStatus) != STOP) {
        QueryPerformanceCounter(timer);
        Sleep(1);
    }

    return SUCCESS;
}