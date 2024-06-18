#include <windows.h>

#include "deus.h"


//Another interesting case study after input.c.
//This function, supposed to run on a separate CPU core
//in fact ran on the same core as the rest of the program,
//not fullfilling its purpose of being
//an independent timer.
//This is because the Windows scheduler prefers
//that threads spawned by a process run on the same core
//as the rest of the process.
//That's why this idea gets abandoned
//in favor of less convenient but actually
//independent timing source,
//a.k.a QueryPerformanceCounter directly.
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