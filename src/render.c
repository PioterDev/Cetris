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

    long long frequency = programParameters->clockFrequency->QuadPart;
    long long start = 0, end = 0, delta = 0, frameTime = 0;
    long long overhead = 0; //if the thread sleeps for too long in one iteration, make it sleep shorter in another iteration
    Color* backgroundColor = parameters->backgroundColor;
    Tile* backgroundTile = parameters->tiles[0];
    int baseTileSize = 0;
    char scalingFactor = 0;

    while(true) {
        if(*(parameters->renderStatus) == STOP)break;

        WaitForSingleObject(parameters->renderMutex, INFINITE);
        WaitForSingleObject(parameters->tilesMutex, INFINITE);
        
        start = timer->QuadPart;

        frameTime = frequency / programParameters->fps;
        baseTileSize = programParameters->baseTileSize;
        scalingFactor = programParameters->scalingFactor;
        
        SDL_SetRenderDrawColor(renderer, backgroundColor->red, backgroundColor->green, backgroundColor->blue, backgroundColor->alpha); //background color
        SDL_RenderClear(renderer);

        /* SDL_Rect backgroundRect = backgroundTile->rect;
        if(scalingFactor > 0) {
            backgroundRect.h *= scalingFactor;
            backgroundRect.w *= scalingFactor;
        }
        else if(scalingFactor < 0) {
            backgroundRect.h /= abs(scalingFactor);
            backgroundRect.w /= abs(scalingFactor);
        }
        SDL_RenderCopy(renderer, backgroundTile->texture, NULL, &backgroundRect);
        //TODO: remove this in favor of rendering individual blocks */

        SDL_Rect current;
        if(scalingFactor > 0) {
            current.w = baseTileSize * scalingFactor;
            current.h = baseTileSize * scalingFactor;
        }
        else if(scalingFactor < 0) {
            current.w = baseTileSize / abs(scalingFactor);
            current.h = baseTileSize / abs(scalingFactor);
        }


        int** tetrisGrid = programParameters->tetrisGrid;
        for(int i = 0; i < programParameters->tetrisGridSize.height; i++) {
            for(int j = 0; j < programParameters->tetrisGridSize.width; j++) {
                int color = tetrisGrid[i][j];
                if(color == GHOST)continue; //TODO: ghost
                if(color < 0)color = abs(color);
                current.y = backgroundTile->rect.y + i * current.h;
                current.x = backgroundTile->rect.x + j * current.w;
                if(SDL_RenderCopy(renderer, baseTextures[color], NULL, &current) != 0) {
                    logToStream(programParameters->debugLog, "Error rendering tile", LOGLEVEL_ERROR);
                }

            }
        }

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
        if(delta > 0)Sleep(delta);

        SDL_RenderPresent(renderer);
        
        overhead = timer->QuadPart - start - frameTime;
    
        ReleaseMutex(parameters->renderMutex);
    }

    return SUCCESS;
}