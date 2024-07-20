#include <stdio.h>
#include <stdlib.h>

#include "deus.h"
#include "logic_shapes.h"
#include "logging.h"
#include "tiles.h"
#include "tile_queue.h"
#include "utils.h"

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