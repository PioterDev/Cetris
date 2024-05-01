#include <stdlib.h>
#include <stdbool.h>
#include <windows.h>
#include <SDL.h>
#include <SDL_image.h>

#include "structs_unions.h"

DWORD WINAPI renderScreen(void* params) {
    renderThreadParameters* parameters = params;

    while(true) {
        if(*(parameters->renderStatus) == STOP)break;

        WaitForSingleObject(parameters->renderMutex, INFINITE);
        WaitForSingleObject(parameters->rectMutex, INFINITE);
        
        SDL_SetRenderDrawColor(parameters->renderer, 255, 255, 255, 255);
        SDL_RenderClear(parameters->renderer);
        SDL_RenderCopy(parameters->renderer, parameters->texture, NULL, parameters->rects[0]); //change later
        SDL_RenderPresent(parameters->renderer);
    
        ReleaseMutex(parameters->rectMutex);
        ReleaseMutex(parameters->renderMutex);
    }

    return SUCCESS;
}