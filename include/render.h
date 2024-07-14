#ifndef RENDER_H
#define RENDER_H


#include <windows.h>

/**
 * @brief Calculates the scaling factor based on current parameters.
 * 
 * @param parameters pointer to ProgramParameters struct
 */
void calculateScalingFactor(ProgramParameters* parameters);

/**
 * @brief Renders to the screen using given parameters.
 * Meant to be used in a separate thread.
 * 
 * @param parameters renderThreadParameters struct
 * @return exit status
 */
DWORD WINAPI renderScreen(void* parameters);


#endif