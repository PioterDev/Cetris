#include <stdlib.h>
#include <stdbool.h>
#include <windows.h>
#include <SDL.h>
#include <SDL_image.h>

#include "structs_unions_defines.h"

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
        
        SDL_SetRenderDrawColor(renderer, backgroundColor->rgba[0], backgroundColor->rgba[1], backgroundColor->rgba[2], backgroundColor->rgba[3]); //background color
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

        char** tetrisGrid = programParameters->tetrisGrid;
        for(int i = 0; i < programParameters->tetrisGridHeight; i++) {
            for(int j = 0; j < programParameters->tetrisGridWidth; j++) {
                char color = tetrisGrid[i][j];
                if(color > 0) {
                    current.y = backgroundTile->rect.y + j * current.h;
                    current.x = backgroundTile->rect.x + i * current.w;
                    SDL_RenderCopy(renderer, baseTextures[color - 1], NULL, &current);
                }
            }
        }
        SDL_Rect rect;
        for(size_t i = 0; i < parameters->tilesAmount; i++) {
            if(parameters->tiles[i] != NULL) {
                rect = parameters->tiles[i]->rect;
                if(scalingFactor > 0) {
                    rect.h *= scalingFactor;
                    rect.w *= scalingFactor;
                }
                else if(scalingFactor < 0) {
                    rect.h /= abs(scalingFactor);
                    rect.w /= abs(scalingFactor);
                }
                SDL_RenderCopy(renderer, parameters->tiles[i]->texture, NULL, &rect);
            }
        }
        end = timer->QuadPart;
        if(end - start == 0) {
            Sleep(1);
            end = timer->QuadPart;
        }

        //formula: [ticks per second (probably 10^7) / FPS - time elapsed for input processing]
        delta = frameTime - (end - start) - overhead;
        delta = delta * 1000 / frequency;
        if(delta > 0)Sleep(delta);

        SDL_RenderPresent(renderer);
        
        overhead = timer->QuadPart - start - frameTime;
    
        ReleaseMutex(parameters->tilesMutex);
        ReleaseMutex(parameters->renderMutex);
    }

    return SUCCESS;
}