#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <float.h>
#include <windows.h>
#include <SDL.h>
#include <SDL_image.h>

#include "deus.h"
#include "logging.h"
#include "logic_shapes.h"

void calculateScalingFactor(ProgramParameters* parameters) {
    double h = (double)parameters->gridSize.height;
    double H = (double)parameters->screenSize.height;

    double w = (double)parameters->gridSize.width;
    double W = (double)parameters->screenSize.width;

    double B = (double)parameters->baseTileSize;
    double hB = h * B, wB = w * B;
    
    double tempH = H / hB, tempW = W / wB;
#ifdef DEBUG
    snprintf(loggingBuffer, loggingBufferSize, "[calculateScalingFactor] %g %g %g %g %g %g", tempH, tempW, hB, wB, H, W);
    logToStream(parameters->log, LOGLEVEL_DEBUG, NULL);
#endif
    if(hB > H || wB > W) { //scale down
#ifdef DEBUG
        logToStream(parameters->log, LOGLEVEL_DEBUG, "[calculateScalingFactor] Scaling down...");
#endif
        if(tempH * wB > W) parameters->scalingFactor = tempW;
        else parameters->scalingFactor = tempH;        
    }
    else if(hB < H || wB < W) { //scale up
#ifdef DEBUG
        logToStream(parameters->log, LOGLEVEL_DEBUG, "[calculateScalingFactor] Scaling up...");
#endif
        if(tempH * wB > W) parameters->scalingFactor = tempW;
        else parameters->scalingFactor = tempH;
    }
    else parameters->scalingFactor = 1.0;
}

DWORD WINAPI renderScreen(void* params) {
    renderThreadParameters* parameters = params;
    ProgramParameters* programParameters = parameters->programParameters;

    logToStream(programParameters->log, LOGLEVEL_DEBUG, "[Render thread] Started the render thread.");
    
    
    Point P;
    P.y = 0;

    SDL_Renderer* renderer = NULL;
    SDL_Texture** baseTextures = NULL;
    SDL_Texture** digits = NULL;


    LARGE_INTEGER start, end;
    long long frequency = programParameters->clockFrequency->QuadPart;
    long long delta = 0, frameTime = 0, overhead = 0; //if the thread sleeps for too long in one iteration, make it sleep shorter in another iteration
    Color* backgroundColor = parameters->backgroundColor;
    int baseTileSize = 0;
    // short scalingFactor = 0;
    double scalingFactor = 1.0;

    while(true) {
        if(*(parameters->renderStatus) == STOP) break;

        WaitForSingleObject(parameters->renderMutex, INFINITE);
        WaitForSingleObject(parameters->tilesMutex, INFINITE);
        
        QueryPerformanceCounter(&start);

        renderer = programParameters->renderer;
        baseTextures = programParameters->baseTextures;
        digits = programParameters->digits;

        if(programParameters->flags.paused) frameTime = frequency / 10; //10 FPS when paused
        else frameTime = frequency / programParameters->fps;
        baseTileSize = programParameters->baseTileSize;
        scalingFactor = programParameters->scalingFactor;
        //all of this copying is required for thread safety
        //as all of those values can change mid execution
        
        SDL_SetRenderDrawColor(renderer, backgroundColor->red, backgroundColor->green, backgroundColor->blue, backgroundColor->alpha);
        SDL_RenderClear(renderer);

        SDL_Rect current;
        current.w = (int) floor((double)baseTileSize * scalingFactor);
        current.h = (int) floor((double)baseTileSize * scalingFactor);

        
        //the x coordinate of a top-left corner of the game matrix
        P.x = (programParameters->screenSize.width / 2) - ((programParameters->gridSize.width * current.w) / 2); 
        if(programParameters->grid != NULL) {
            for(unsigned int i = 0; i < programParameters->gridSize.height; i++) {
                for(unsigned int j = 0; j < programParameters->gridSize.width; j++) {
                    int color = programParameters->grid[i][j];
                    if(color == GHOST) {
                        continue; //TODO: ghost
                    }
                    else if(color < 0) color = abs(color);
                    current.y = P.y + i * current.h;
                    current.x = P.x + j * current.w;
                    if(SDL_RenderCopy(renderer, baseTextures[color], NULL, &current)) {
                        snprintf(loggingBuffer, loggingBufferSize, "[Render thread/Game running] Error rendering tile: %s", SDL_GetError());
                        logToStream(programParameters->log, LOGLEVEL_ERROR, NULL);
                    }
                }
            }
        }
        else {
            for(unsigned int i = 0; i < programParameters->gridSize.height; i++) {
                current.y = P.y + i * current.h;
                for(unsigned int j = 0; j < programParameters->gridSize.width; j++) {
                    current.x = P.x + j * current.w;
                    if(SDL_RenderCopy(renderer, baseTextures[0], NULL, &current)) {
                        snprintf(loggingBuffer, loggingBufferSize, "[Render thread/Game not running] Error rendering tile: %s", SDL_GetError());
                        logToStream(programParameters->log, LOGLEVEL_ERROR, NULL);
                    }
                }
            }
        }

        //hold area rendering
        if(programParameters->heldTile == NULL) {
            for(int i = 0; i < holdSpaceVerticalSize; i++) {
                current.y = P.y + (i + 1) * current.h;
                for(int j = 0; j < maxTileSize; j++) {
                    current.x = P.x + (j - maxTileSize - 1) * current.w;
                    if(SDL_RenderCopy(renderer, baseTextures[0], NULL, &current)) {
                        snprintf(loggingBuffer, loggingBufferSize, "[Render thread/Game not running] Error rendering tile: %s", SDL_GetError());
                        logToStream(programParameters->log, LOGLEVEL_ERROR, NULL);
                    }
                }
            }
        }
        else {
            for(int i = 0; i < holdSpaceVerticalSize; i++) {
                current.y = P.y + (i + 1) * current.h;
                for(int j = 0; j < maxTileSize; j++) {
                    current.x = P.x + (j - maxTileSize - 1) * current.w;
                    int rendered = false;
                    for(int k = 0; k < holdPositionsAmount[programParameters->heldTile->shape]; k++) {
                        if(
                            i == holdPositions[programParameters->heldTile->shape][k][1] &&    
                            j == holdPositions[programParameters->heldTile->shape][k][0]
                        ) {
                            if(SDL_RenderCopy(renderer, baseTextures[programParameters->heldTile->color], NULL, &current)) {
                                snprintf(loggingBuffer, loggingBufferSize, "[Render thread/Game not running] Error rendering tile: %s", SDL_GetError());
                                logToStream(programParameters->log, LOGLEVEL_ERROR, NULL);
                            }
                            rendered = true;
                            break;
                        }
                    }
                    if(!rendered) {
                        if(SDL_RenderCopy(renderer, baseTextures[0], NULL, &current)) {
                            snprintf(loggingBuffer, loggingBufferSize, "[Render thread/Game not running] Error rendering tile: %s", SDL_GetError());
                            logToStream(programParameters->log, LOGLEVEL_ERROR, NULL);
                        }
                    }
                }
            }
        }

        //combo rendering
        unsigned int combo = programParameters->combo;
        size_t c = 1;
        int digitCount = 0;
        if(combo == 0) digitCount++;
        while(c <= combo) {
            digitCount++;
            c *= 10;
        }
        current.x = P.x + (programParameters->gridSize.width + digitCount - 1) * current.w;
        current.y = (P.y - 2 * current.h) + (programParameters->gridSize.height * 3 / 4) * current.h;
        do {
            if(SDL_RenderCopy(renderer, digits[combo % 10], NULL, &current)) {
                snprintf(loggingBuffer, loggingBufferSize, "[Render thread/Combo] Error rendering digit: %s", SDL_GetError());
                logToStream(programParameters->log, LOGLEVEL_ERROR, NULL);
            }
            combo /= 10;
            current.x -= current.w;
        }
        while(combo > 0);

        //score rendering
        size_t score = programParameters->score;
        c = 1;
        digitCount = 0;
        if(score == 0) digitCount++;
        while(c <= score) { //doing multiplication is faster than division
            digitCount++;
            c *= 10;
        }
        current.x = P.x + (programParameters->gridSize.width + digitCount - 1) * current.w;
        current.y = P.y + (programParameters->gridSize.height * 3 / 4) * current.h;
        do {
            if(SDL_RenderCopy(renderer, digits[score % 10], NULL, &current)) {
                snprintf(loggingBuffer, loggingBufferSize, "[Render thread/Score] Error rendering digit: %s", SDL_GetError());
                logToStream(programParameters->log, LOGLEVEL_ERROR, NULL);
            }
            score /= 10;
            current.x -= current.w;
        }
        while(score > 0);

        //TODO: preview of next tiles
        /* TileQueueElement* queued = programParameters->tileQueue->head;
        for(size_t i = 0; i < programParameters->tileQueue->size; i++) {
            
        } */

        QueryPerformanceCounter(&end);

        ReleaseMutex(parameters->tilesMutex);
        
        //formula: [ticks per second (probably 10^7) / FPS - time elapsed for input processing]
        delta = frameTime - (end.QuadPart - start.QuadPart) - overhead;
        delta = delta * 1000 / frequency;
        if(delta > 0) Sleep(delta);

        SDL_RenderPresent(renderer);

        QueryPerformanceCounter(&end);
        overhead = end.QuadPart - start.QuadPart - frameTime;

        ReleaseMutex(parameters->renderMutex);
    }

    return SUCCESS;
}