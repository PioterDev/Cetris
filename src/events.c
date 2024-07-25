#include <stdlib.h>

#include "config.h"
#include "deus.h"
#include "logic_other.h"
#include "logging.h"
#include "render.h"
#include "tiles.h"
#include "tile_queue.h"
#include "utils.h"

static const char gameEndReasonNames[REASON_AMOUNT][32] = {
    "User exit",
    "Tile loading failed"
};

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

status_t onGameStart(ProgramParameters* parameters) {
    logToStream(parameters->log, LOGLEVEL_INFO, "Attempting to create a game matrix...");
    parameters->grid = zeroMatrix(parameters->gridSize);
    if(parameters->grid == NULL) {
        logToStream(parameters->log, LOGLEVEL_ERROR, "Error allocating memory for the game matrix.");
        return MEMORY_FAILURE;
    }
    logToStream(parameters->log, LOGLEVEL_INFO, "Game matrix successfully created!");
    logToStream(parameters->log, LOGLEVEL_INFO, "Attempting to load a random tile...");
    parameters->currentTile = loadTileRandom(NULL, parameters->gridSize.width, parameters->log);
    if(parameters->currentTile == NULL) return MEMORY_FAILURE;
    logToStream(parameters->log, LOGLEVEL_INFO, "Attempting to fill the tile queue...");
    for(unsigned int i = 0; i < tileQueuedAmount; i++) {
        Tile* tmp = loadTileRandom(NULL, parameters->gridSize.width, parameters->log);
        if(tmp == NULL) return MEMORY_FAILURE;
        enqueueTile(&parameters->tileQueue, tmp);
    }
    logToStream(parameters->log, LOGLEVEL_INFO, "Tile queue filled successfully!");

    loadTileIntoGrid(parameters);

    playMusic(parameters);
    
    parameters->level = 1;
    parameters->score = 0;
    parameters->flags.playing = true;
    logToStream(parameters->log, LOGLEVEL_INFO, "Game started!");
    
    return SUCCESS;
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
    parameters->combo = parameters->level = 0;
    parameters->flags.playing = false;
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
    if(parameters->flags.holdLocked) parameters->flags.holdLocked = false;
    /* freeTile(parameters->currentTile);

    dequeueTile(&parameters->tileQueue, &parameters->currentTile);
    enqueueTile(&parameters->tileQueue, loadTileRandom(renderer, NULL, TILELOAD_NOTEXTURE, parameters->log)); */
}

status_t onWindowResize(ProgramParameters* parameters, SDL_Window* window) {
    status_t s = SUCCESS;
    
    logToStream(parameters->log, LOGLEVEL_DEBUG, "[onWindowResize] Destroying the renderer...");
    SDL_DestroyRenderer(parameters->renderer);

    logToStream(parameters->log, LOGLEVEL_DEBUG, "[onWindowResize] Attempting to create a new renderer...");
    parameters->renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(parameters->renderer == NULL) {
        s = SDL_RENDERER_FAILURE;
        snprintf(loggingBuffer, loggingBufferSize, "[onWindowResize] Failed to create a new renderer: %s", SDL_GetError());
        logToStream(parameters->log, LOGLEVEL_ERROR, NULL);
        return s;
    }
    logToStream(parameters->log, LOGLEVEL_DEBUG, "[onWindowResize] New renderer successfully created!");

    logToStream(parameters->log, LOGLEVEL_DEBUG, "[onWindowResize] Destroying textures...");
    freeTextures(parameters);
    
    logToStream(parameters->log, LOGLEVEL_DEBUG, "[onWindowResize] Attempting to reload base tile textures...");
    s = loadBaseTextures(parameters);
    if(s != SUCCESS) {
        logToStream(parameters->log, LOGLEVEL_ERROR, "[onWindowResize] Error reloading base tile textures.");
        return s;
    }
    logToStream(parameters->log, LOGLEVEL_DEBUG, "[onWindowResize] Base tile textures successfully loaded!");


    logToStream(parameters->log, LOGLEVEL_DEBUG, "[onWindowResize] Attempting to reload digits...");
    s = loadDigits(parameters);
    if(s != SUCCESS) {
        logToStream(parameters->log, LOGLEVEL_ERROR, "[onWindowResize] Error reloading digit textures.");
        return s;
    }
    calculateScalingFactor(parameters);
    return s;
}

status_t onHold(ProgramParameters* parameters) {
    if(parameters->heldTile != NULL && parameters->flags.holdLocked) return FAILURE;
    unloadTileFromGrid(parameters->grid, parameters->currentTile);
    setDefaultTileParameters(parameters->currentTile, parameters->gridSize.width);
    if(parameters->heldTile != NULL) {
        Tile* tmp = parameters->currentTile;
        parameters->currentTile = parameters->heldTile;
        parameters->heldTile = tmp;
        parameters->flags.holdLocked = true;
    }
    else {
        parameters->heldTile = parameters->currentTile;
        dequeueTile(&parameters->tileQueue, &parameters->currentTile);
        enqueueTile(&parameters->tileQueue, loadTileRandom(NULL, parameters->gridSize.width, parameters->log));
    }
    loadTileIntoGrid(parameters);
    return SUCCESS;
}