#include <stdio.h>
#include <stdlib.h>

#include "deus.h"
#include "logic_shapes.h"
#include "logging.h"
#include "tiles.h"
#include "tile_queue.h"
#include "utils.h"

static const char gameEndReasonNames[REASON_AMOUNT][32] = {
    "User exit",
    "Tile loading failed"
};

status_t loadTileIntoGrid(int** grid, const Tile* tile, const Size gridSize) {
    if(tile == NULL) return MEMORY_FAILURE;
    if(tile->state == STATE_UNKNOWN) return FAILURE;
#ifdef DEBUG
    snprintf(
        loggingBuffer, loggingBufferSize, "[loadTileIntoGrid] Attempting to load a %s %s tile...", 
        colorNames[tile->color],
        shapeNames[tile->shape]
    );
    logToStream(defaultStream, LOGLEVEL_DEBUG, NULL);    
#endif
    int n = occupiedAmount[tile->state];
    Point positions[n];
    for(int i = 0; i < n; i++) {
        positions[i].x = tile->position.x + basePositions[tile->state][i][0];
        positions[i].y = tile->position.y + basePositions[tile->state][i][1];
        if(positions[i].x < 0 || positions[i].y < 0) return INDEXOUTOFRANGE;
        if((unsigned int)positions[i].x >= gridSize.width || (unsigned int)positions[i].y >= gridSize.height) return INDEXOUTOFRANGE;

        int a = grid[positions[i].y][positions[i].x];
        if(a != 0 && a != GHOST) return FAILURE;
    }

    for(int i = 0; i < n; i++) {
        grid[positions[i].y][positions[i].x] = -1 * tile->color;
    }
#ifdef DEBUG
    logToStream(defaultStream, LOGLEVEL_DEBUG, "[loadTileIntoGrid] Successfully loaded tile into grid.");
#endif
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
    enqueueTile(&parameters->tileQueue, loadTileRandom(renderer, NULL, TILELOAD_NOTEXTURE, parameters->log)); */
}

void onGameEnd(ProgramParameters* parameters, GameEndReason reason) {
#ifdef DEBUG
    snprintf(loggingBuffer, loggingBufferSize, "onGameEnd event triggered, reason: %s, ending the game...", gameEndReasonNames[reason]);
    logToStream(parameters->log, LOGLEVEL_INFO, NULL);
#else
    logToStream(parameters->log, LOGLEVEL_INFO, "onGameEnd event triggered, ending the game...");
#endif
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
    logToStream(parameters->log, LOGLEVEL_INFO, "Attempting to create a game matrix...");
    parameters->grid = zeroMatrix(parameters->gridSize);
    if(parameters->grid == NULL) {
        logToStream(parameters->log, LOGLEVEL_ERROR, "Error allocating memory for the game matrix.");
        return MEMORY_FAILURE;
    }
    logToStream(parameters->log, LOGLEVEL_INFO, "Game matrix successfully created!");
    logToStream(parameters->log, LOGLEVEL_INFO, "Attempting to load a random tile...");
    parameters->currentTile = loadTileRandom(renderer, NULL, parameters->gridSize.width, TILELOAD_NOTEXTURE, parameters->log);
    if(parameters->currentTile == NULL) return MEMORY_FAILURE;
    logToStream(parameters->log, LOGLEVEL_INFO, "Attempting to fill the tile queue...");
    for(unsigned int i = 0; i < tileQueuedAmount; i++) {
        Tile* tmp = loadTileRandom(renderer, NULL, parameters->gridSize.width, TILELOAD_NOTEXTURE, parameters->log);
        if(tmp == NULL) return MEMORY_FAILURE;
        enqueueTile(&parameters->tileQueue, tmp);
    }
    logToStream(parameters->log, LOGLEVEL_INFO, "Tile queue filled successfully!");

    loadTileIntoGrid(parameters->grid, parameters->currentTile, parameters->gridSize);

    playMusic(parameters);
    
    parameters->level = 1;
    parameters->flags.playing = true;
    logToStream(parameters->log, LOGLEVEL_INFO, "Game started!");
    
    return SUCCESS;
}