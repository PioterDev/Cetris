#include <stdlib.h>
#include <stdbool.h>
#include <windows.h>
#include <SDL.h>
#include <SDL_image.h>

#include "structs_unions.h"

DWORD WINAPI renderScreen(void* params) {
    renderThreadParameters* parameters = params;
    long long frequency = parameters->programParameters->clockFrequency->QuadPart;
    LARGE_INTEGER* timer = parameters->programParameters->timer;

    while(true) {
        if(*(parameters->renderStatus) == STOP)break;

        WaitForSingleObject(parameters->renderMutex, INFINITE);
        WaitForSingleObject(parameters->tilesMutex, INFINITE);
        
        long long start = timer->QuadPart;

        SDL_SetRenderDrawColor(parameters->renderer, 20, 20, 20, 255);
        SDL_RenderClear(parameters->renderer);

        for(size_t i = 0; i < parameters->tilesAmount; i++) {
            SDL_RenderCopy(parameters->renderer, parameters->tiles[i]->texture, NULL, &parameters->tiles[i]->rect);
        }
        //formula: [ticks per second (probably 10^7) / FPS - time elapsed for input processing]
        long long delta = (frequency / parameters->programParameters->fps - (timer->QuadPart - start)) * 1000 / frequency;

        Sleep(delta);

        SDL_RenderPresent(parameters->renderer);
    
        ReleaseMutex(parameters->tilesMutex);
        ReleaseMutex(parameters->renderMutex);
    }

    return SUCCESS;
}