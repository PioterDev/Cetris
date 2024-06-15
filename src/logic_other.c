#include <stdio.h>
#include <stdlib.h>

#include "deus.h"
#include "logic_shapes.h"
#include "logging.h"
#include "tiles.h"
#include "tile_queue.h"
#include "utils.h"

status_t loadTileIntoGrid(int** tetrisGrid, const Tile* tile) {
    if(tile == NULL)return MEMORY_FAILURE;
    int n = getOccupiedAmount(tile->state);
    if(n == -1)return FAILURE;
    Point positions[n];
    if(getPositions(tile->state, tile->position, positions) == FAILURE) return FAILURE;
    if(checkPositions(tetrisGrid, positions, n) == FAILURE) return FAILURE;
    setPositions(tetrisGrid, positions, n, -1 * tile->color);
    return SUCCESS;
}

static inline void shiftDown(int** tetrisGrid, const Size tetrisGridSize, const int n, const int startHeight) {
    for(int i = startHeight; i > n - 1; i--) {
        for(unsigned int j = 0; j < tetrisGridSize.width; j++) {
            tetrisGrid[i][j] = tetrisGrid[i - n][j];
        }
    }
    for(int i = 0; i < n; i++) {
        memset(tetrisGrid[i], 0, sizeof(tetrisGrid[i][0]) * tetrisGridSize.width);
    }
}

void onPlacement(ProgramParameters* parameters) {
    int howManyFull = 0, lowestFull = parameters->tetrisGridSize.height - 1;
    for(unsigned int i = parameters->tetrisGridSize.height - 1; i > 0; i--) {
        char isRowFull = true;
        for(unsigned int j = 0; j < parameters->tetrisGridSize.width; j++) {
            if(parameters->tetrisGrid[i][j] == 0 || parameters->tetrisGrid[i][j] == GHOST) {
                isRowFull = false;
                lowestFull--;
                break;
            }
        }
        if(isRowFull) {
            howManyFull++;
            break;
        }
    }
    if(lowestFull != 0) {
        int i = lowestFull - 1;
        while(true) {
            char isFull = true;
            for(unsigned int j = 0; j < parameters->tetrisGridSize.width; j++) {
                if(parameters->tetrisGrid[i][j] == 0 || parameters->tetrisGrid[i][j] == GHOST) {
                    isFull = false;
                    break;
                }
            }
            if(!isFull)break;
            i--;
            howManyFull++;
        }
        for(int i = 0; i < howManyFull; i++) {
            memset(parameters->tetrisGrid[lowestFull - i], 0, sizeof(parameters->tetrisGrid[lowestFull][0]) * parameters->tetrisGridSize.width);
        }
        shiftDown(parameters->tetrisGrid, parameters->tetrisGridSize, howManyFull, lowestFull);
        absMatrix(parameters->tetrisGrid, parameters->tetrisGridSize); //TODO: make it update only rows where the tile fell
    }

    switch(howManyFull) {
        case 0:
            parameters->combo = 0;
            break;
        case 1:
            parameters->score += POINTS_SINGLE * parameters->level;
            parameters->combo++;
            break;
        case 2:
            parameters->score += POINTS_DOUBLE * parameters->level;
            parameters->combo++;
            break;
        case 3:
            parameters->score += POINTS_TRIPLE * parameters->level;
            parameters->combo++;
            break;
        case 4:
            parameters->score += POINTS_TETRIS * parameters->level;
            parameters->combo++;
            break;
    }

    parameters->score += POINTS_COMBO_MULTIPLIER * parameters->combo * parameters->level;
    /* freeTile(parameters->currentTile);

    dequeueTile(&parameters->tileQueue, &parameters->currentTile);
    enqueueTile(&parameters->tileQueue, loadTileRandom(renderer, NULL, TILELOAD_NOTEXTURE, parameters->debugLog)); */
}

void onGameEnd(ProgramParameters* parameters) {
    freeTile(parameters->currentTile);
    parameters->currentTile = NULL;
    flushTileQueue(&parameters->tileQueue);
    stopMusic();
    freeMatrix(parameters->tetrisGrid, parameters->tetrisGridSize.height);
    parameters->tetrisGrid = NULL;
    parameters->score = 0;
    parameters->combo = 0;
    parameters->level = 0;
    parameters->flags.playing = false;
    logToStream(parameters->generallog, LOGLEVEL_INFO, "onGameEnd event triggered, ending the game...");
}

status_t onGameStart(ProgramParameters* parameters, SDL_Renderer* renderer) {
    logToStream(parameters->generallog, LOGLEVEL_INFO, "Attempting to create a game matrix...");
    parameters->tetrisGrid = zeroMatrix(parameters->tetrisGridSize);
    if(parameters->tetrisGrid == NULL) {
        logToStream(parameters->errorlog, LOGLEVEL_ERROR, "Error allocating memory for the game matrix.");
        return MEMORY_FAILURE;
    }
    logToStream(parameters->generallog, LOGLEVEL_INFO, "Game matrix successfully created!");

    parameters->currentTile = loadTileRandom(renderer, NULL, TILELOAD_NOTEXTURE, parameters->debugLog);
    if(parameters->currentTile == NULL) return MEMORY_FAILURE;
    for(unsigned int i = 0; i < tileQueuedAmount; i++) {
        Tile* tmp = loadTileRandom(renderer, NULL, TILELOAD_NOTEXTURE, parameters->debugLog);
        if(tmp == NULL) return MEMORY_FAILURE;
        enqueueTile(&parameters->tileQueue, tmp);
    }
    
    loadTileIntoGrid(parameters->tetrisGrid, parameters->currentTile);

    playMusic(parameters);
    
    parameters->level = 1;
    parameters->flags.playing = true;
    logToStream(parameters->generallog, LOGLEVEL_INFO, "Game started!");
    
    return SUCCESS;
}