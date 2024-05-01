#include <stdlib.h>
#include <stdbool.h>
#include <windows.h>
#include <SDL.h>

#include "structs_unions.h"

//A failed attempt at making an input loop on a separate thread.
//Doesn't work because SDL_PollEvent can
//only be called from the thread that created the window.
//Too bad, but I guess I learned some stuff.
//I'm gonna leave this file as is for
//future generations of C programmers.

DWORD WINAPI inputLoop(void* params) {
    inputThreadParameters* parameters = params;
    SDL_Event event;

    SDL_Rect rect = *(parameters->rects[0]); //remove later
    
    while(true) {
        WaitForSingleObject(parameters->inputMutex, INFINITE);
        WaitForSingleObject(parameters->rectMutex, INFINITE);

        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                *(parameters->inputStatus) = STOP;
                break;
            }
            else if(event.type == SDL_KEYDOWN) { 
                switch(event.key.keysym.sym) {
                    case SDLK_UP:
                        if(rect.y > 10)rect.y -= 10;
                        break;
                    case SDLK_DOWN:
                        if(rect.y < parameters->programParameters->screen_height)rect.y += 10;
                        break;
                    case SDLK_LEFT:
                        if(rect.x > 10)rect.x -= 10;
                        break;
                    case SDLK_RIGHT:
                        if(rect.x < parameters->programParameters->screen_width)rect.x += 10;
                        break;
                }
            }
        }
        
        ReleaseMutex(parameters->rectMutex);
        ReleaseMutex(parameters->inputMutex);
        if(*(parameters->inputStatus) == STOP)break;
        
        Sleep(1);
    }
    return SUCCESS;
}