#ifndef INPUT_H
#define INPUT_H


#include <windows.h>

#include "utils.h"

//Not used, see input.c for why.

/**
 * @brief A game input loop, intended to run on a separate thread.
 * 
 * @param params inputThreadParameters struct
 * @return exit status
 */
DWORD WINAPI inputLoop(void* params);


#endif