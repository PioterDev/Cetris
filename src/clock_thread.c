#include <windows.h>

#include "structs_unions_defines.h"

DWORD WINAPI highPerformanceClockThread(void* params) {
    clockThreadParameters* parameters = params;
    LARGE_INTEGER* timer = parameters->timer;
    LARGE_INTEGER start;
    QueryPerformanceCounter(&start);    

    while(*(parameters->clockStatus) != STOP) {
        QueryPerformanceCounter(timer);
        timer->QuadPart -= start.QuadPart;
        Sleep(1);
    }

    return SUCCESS;
}