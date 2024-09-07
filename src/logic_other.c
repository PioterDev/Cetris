#include <stdio.h>
#include <stdlib.h>

#include "deus.h"
#include "logic_shapes.h"
#include "logging.h"
#include "tiles.h"
#include "tile_queue.h"
#include "utils.h"

status_t loadTileIntoGrid(ProgramParameters* parameters) {
    Tile* tile = parameters->currentTile;
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
    if(n == 0) return BASEOUTOFRANGE;
    Point positions[n];
    for(int i = 0; i < n; i++) {
        positions[i].x = tile->position.x + basePositions[tile->state][i][0];
        positions[i].y = tile->position.y + basePositions[tile->state][i][1];
        if(positions[i].x < 0 || positions[i].y < 0) return INDEXOUTOFRANGE;
        if(
            positions[i].x >= (int)parameters->gridSize.width ||
            positions[i].y >= (int)parameters->gridSize.height
        ) return INDEXOUTOFRANGE;

        int a = parameters->grid[positions[i].y][positions[i].x];
        if(a != 0 && a != GHOST) return FAILURE;
    }

    for(int i = 0; i < n; i++) {
        parameters->grid[positions[i].y][positions[i].x] = -tile->color;
    }
    parameters->flags.tileRecentlyLoaded = true;
#ifdef DEBUG
    logToStream(defaultStream, LOGLEVEL_DEBUG, "[loadTileIntoGrid] Successfully loaded tile into grid.");
#endif
    return SUCCESS;
}

status_t unloadTileFromGrid(int** grid, const Tile* tile) {
    if(tile == NULL) return MEMORY_FAILURE;
    if(tile->state == STATE_UNKNOWN) return FAILURE;
    if(tile->position.x < 0 || tile->position.y < 0) return FAILURE;

    for(int i = 0; i < occupiedAmount[tile->state]; i++) {
        grid[tile->position.y + basePositions[tile->state][i][1]][tile->position.x + basePositions[tile->state][i][0]] = 0;
    }

    return SUCCESS;
}