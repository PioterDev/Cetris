#ifndef CLOCK_THREAD_H
#define CLOCK_THREAD_H


#include <windows.h>

//Not used, see clock_thread.c for why.

/**
 * @brief A high performance clock. Counts how much time passed since it started.
 * Its accuracy is based on Windows' QueryPerformanceFrequency() (most likely 10^7 ticks / second).
 * 
 * @param params clockThreadParameters struct
 * @return exit status
 */
DWORD WINAPI highPerformanceClockThread(void* params);


#endif