#ifndef RENDER_H
#define RENDER_H


#include <windows.h>
#include <SDL.h>

#include "utils.h"

/**
 * @brief Renders to the screen using given parameters.
 * 
 * @param parameters renderThreadParameters struct
 * @return exit status
 */
DWORD WINAPI renderScreen(void* parameters);


#endif