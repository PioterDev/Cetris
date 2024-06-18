#include <stdio.h>
#include <stdlib.h>

#include "deus.h"
#include "logic_shapes.h"
#include "logging.h"
#include "tiles.h"
#include "tile_queue.h"
#include "utils.h"

status_t loadTileIntoGrid(int** grid, const Tile* tile) {
    if(tile == NULL) return MEMORY_FAILURE;
    int n = getOccupiedAmount(tile->state);
    if(n == -1) return FAILURE;
    Point positions[n];
    if(getPositions(tile->state, tile->position, positions) == FAILURE) return FAILURE;
    if(checkPositions(grid, positions, n) == FAILURE) return FAILURE;
    setPositions(grid, positions, n, -1 * tile->color);
    return SUCCESS;
}

void togglePause(ProgramParameters* parameters) {
    toggleMusic(!parameters->flags.paused);
    if(parameters->flags.paused) {
        parameters->flags.paused = false;
        /* for(int i = 0; i < tileColorAmount; i++) {
            SDL_SetTextureAlphaMod(parameters->baseTextures[i], 255);
        } */
    }
    else {
        parameters->flags.paused = true;
        /* for(int i = 0; i < tileColorAmount; i++) {
            SDL_SetTextureAlphaMod(parameters->baseTextures[i], 50);
        } */
    }
}

static inline void shiftDown(int** grid, const Size gridSize, const int n, const int startHeight) {
    for(int i = startHeight; i > n - 1; i--) {
        for(unsigned int j = 0; j < gridSize.width; j++) {
            grid[i][j] = grid[i - n][j];
        }
    }
    for(int i = 0; i < n; i++) {
        memset(grid[i], 0, sizeof(grid[i][0]) * gridSize.width);
    }
}

void onPlacement(ProgramParameters* parameters) {
    int howManyFull = 0, lowestFull = parameters->gridSize.height - 1;
    for(unsigned int i = parameters->gridSize.height - 1; i > 0; i--) {
        char isRowFull = true;
        for(unsigned int j = 0; j < parameters->gridSize.width; j++) {
            if(parameters->grid[i][j] == 0 || parameters->grid[i][j] == GHOST) {
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
            int isFull = true;
            for(unsigned int j = 0; j < parameters->gridSize.width; j++) {
                if(parameters->grid[i][j] == 0 || parameters->grid[i][j] == GHOST) {
                    isFull = false;
                    break;
                }
            }
            if(!isFull)break;
            i--;
            howManyFull++;
        }
        for(int i = 0; i < howManyFull; i++) {
            memset(parameters->grid[lowestFull - i], 0, sizeof(parameters->grid[lowestFull][0]) * parameters->gridSize.width);
        }
        shiftDown(parameters->grid, parameters->gridSize, howManyFull, lowestFull);
        absMatrix(parameters->grid, parameters->gridSize); //TODO: make it update only rows where the tile fell
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
            parameters->score += POINTS_QUAD * parameters->level;
            parameters->combo++;
            break;
    }

    parameters->score += POINTS_COMBO_MULTIPLIER * parameters->combo * parameters->level;
    /* freeTile(parameters->currentTile);

    dequeueTile(&parameters->tileQueue, &parameters->currentTile);
    enqueueTile(&parameters->tileQueue, loadTileRandom(renderer, NULL, TILELOAD_NOTEXTURE, parameters->debugLog)); */
}

void onGameEnd(ProgramParameters* parameters) {
    logToStream(parameters->generallog, LOGLEVEL_INFO, "onGameEnd event triggered, ending the game...");
    if(parameters->currentTile != NULL) {
        freeTile(parameters->currentTile);
        parameters->currentTile = NULL;
    }
    if(parameters->heldTile != NULL) {
        freeTile(parameters->heldTile);
        parameters->heldTile = NULL;
    }
    flushTileQueue(&parameters->tileQueue);
    stopMusic();
    freeMatrix(parameters->grid, parameters->gridSize.height);
    parameters->grid = NULL;
    parameters->score = parameters->combo = parameters->level = 0;
    parameters->flags.playing = false;
}

status_t onGameStart(ProgramParameters* parameters, SDL_Renderer* renderer) {
    logToStream(parameters->generallog, LOGLEVEL_INFO, "Attempting to create a game matrix...");
    parameters->grid = zeroMatrix(parameters->gridSize);
    if(parameters->grid == NULL) {
        logToStream(parameters->errorlog, LOGLEVEL_ERROR, "Error allocating memory for the game matrix.");
        return MEMORY_FAILURE;
    }
    logToStream(parameters->generallog, LOGLEVEL_INFO, "Game matrix successfully created!");

    parameters->currentTile = loadTileRandom(renderer, NULL, parameters->gridSize.width, TILELOAD_NOTEXTURE, parameters->debugLog);
    if(parameters->currentTile == NULL) return MEMORY_FAILURE;
    for(unsigned int i = 0; i < tileQueuedAmount; i++) {
        Tile* tmp = loadTileRandom(renderer, NULL, parameters->gridSize.width, TILELOAD_NOTEXTURE, parameters->debugLog);
        if(tmp == NULL) return MEMORY_FAILURE;
        enqueueTile(&parameters->tileQueue, tmp);
    }
    
    loadTileIntoGrid(parameters->grid, parameters->currentTile);

    playMusic(parameters);
    
    parameters->level = 1;
    parameters->flags.playing = true;
    logToStream(parameters->generallog, LOGLEVEL_INFO, "Game started!");
    
    return SUCCESS;
}