#include <stdlib.h>
#include <stdbool.h>
#include <windows.h>
#include <SDL.h>
#include <SDL_image.h>

#include "logging.h"
#include "deus.h"

DWORD WINAPI renderScreen(void* params) {
    renderThreadParameters* parameters = params;
    ProgramParameters* programParameters = parameters->programParameters;
    
    LARGE_INTEGER* timer = programParameters->timer;
    
    SDL_Renderer* renderer = parameters->renderer;
    SDL_Texture** baseTextures = programParameters->baseTextures;
    SDL_Texture** digits = programParameters->digits;
    Point P;
    P.y = 0;

    long long frequency = programParameters->clockFrequency->QuadPart;
    long long start = 0, end = 0, delta = 0, frameTime = 0;
    long long overhead = 0; //if the thread sleeps for too long in one iteration, make it sleep shorter in another iteration
    Color* backgroundColor = parameters->backgroundColor;
    int baseTileSize = 0;
    short scalingFactor = 0;

    

    while(true) {
        if(*(parameters->renderStatus) == STOP)break;

        WaitForSingleObject(parameters->renderMutex, INFINITE);
        WaitForSingleObject(parameters->tilesMutex, INFINITE);
        
        start = timer->QuadPart;

        frameTime = frequency / programParameters->fps;
        baseTileSize = programParameters->baseTileSize;
        scalingFactor = programParameters->scalingFactor;
        
        SDL_SetRenderDrawColor(renderer, backgroundColor->red, backgroundColor->green, backgroundColor->blue, backgroundColor->alpha);
        SDL_RenderClear(renderer);

        SDL_Rect current;
        if(scalingFactor > 0) {
            current.w = baseTileSize * scalingFactor;
            current.h = baseTileSize * scalingFactor;
        }
        else if(scalingFactor < 0) {
            current.w = baseTileSize / abs(scalingFactor);
            current.h = baseTileSize / abs(scalingFactor);
        }

        P.x = (programParameters->screenSize.width >> 1) - ((GridWidth * current.w) >> 1); //the x coordinate of a top-left corner of the game matrix
        if(programParameters->tetrisGrid != NULL) {
            for(unsigned int i = 0; i < programParameters->tetrisGridSize.height; i++) {
                for(unsigned int j = 0; j < programParameters->tetrisGridSize.width; j++) {
                    int color = programParameters->tetrisGrid[i][j];
                    if(color == GHOST) continue; //TODO: ghost
                    if(color < 0) color = abs(color);
                    current.y = P.y + i * current.h;
                    current.x = P.x + j * current.w;
                    if(SDL_RenderCopy(renderer, baseTextures[color], NULL, &current) != 0) {
                        logToStream(programParameters->debugLog, "Error rendering tile", LOGLEVEL_ERROR);
                    }
                }
            }
        }
        else {
            for(unsigned int i = 0; i < programParameters->tetrisGridSize.height; i++) {
                for(unsigned int j = 0; j < programParameters->tetrisGridSize.width; j++) {
                    current.y = P.y + i * current.h;
                    current.x = P.x + j * current.w;
                    if(SDL_RenderCopy(renderer, baseTextures[0], NULL, &current) != 0) {
                        logToStream(programParameters->debugLog, "Error rendering tile", LOGLEVEL_ERROR);
                    }
                }
            }
        }

        size_t tmp = programParameters->score;
        current.x = P.x + GridWidth * current.w;
        current.y = P.y + (GridHeight * 3 / 4) * current.h;
        do {
            int digit = tmp % 10;
            tmp /= 10;
            if(SDL_RenderCopy(renderer, digits[digit], NULL, &current)) {
                logToStream(programParameters->debugLog, "Error rendering digit", LOGLEVEL_ERROR);
            }
            current.x += current.w;

        }
        while(tmp > 0);

        //TODO: preview of next tiles
        /* TileQueueElement* queued = programParameters->tileQueue->head;
        for(size_t i = 0; i < programParameters->tileQueue->size; i++) {
            
        } */

        end = timer->QuadPart;
        if(end - start == 0) {
            Sleep(1);
            end = timer->QuadPart;
        }
        
        ReleaseMutex(parameters->tilesMutex);

        //formula: [ticks per second (probably 10^7) / FPS - time elapsed for input processing]
        delta = frameTime - (end - start) - overhead;
        delta = delta * 1000 / frequency;
        if(delta > 0) Sleep(delta);

        SDL_RenderPresent(renderer);
        
        overhead = timer->QuadPart - start - frameTime;
    
        ReleaseMutex(parameters->renderMutex);
    }

    return SUCCESS;
}