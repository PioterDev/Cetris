#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <SDL.h>

#include "config.h"
#include "deus.h"
#include "logging.h"
#include "utils.h"

Tile* loadTile(SDL_Renderer* renderer, TileColor color, TileShape shape, Point* coordinates, const int gridWidth, const int flags, FILE* debug) {
    Tile* tile = malloc(sizeof(Tile));
    if(tile == NULL) return NULL;

    tile->texture = NULL;
    #ifdef DEBUG
    if(flags & TILELOAD_NOTEXTURE) {
        logToStream(debug, LOGLEVEL_DEBUG, "[loadTile] Skipping texture loading");
    }
    #endif

    switch(shape) {
        case BAR:
            tile->position.y = 0;
            tile->position.x = (gridWidth >> 1) - 2;
            tile->state = BAR_HORIZONTAL_UP;
            break;
        case J:
            tile->position.y = 2;
            tile->position.x = (gridWidth >> 1) - 1;
            tile->state = J_0;
            break;
        case L:
            tile->position.y = 2;
            tile->position.x = (gridWidth >> 1) - 1;
            tile->state = L_0;
            break;
        case S:
            tile->position.y = 1;
            tile->position.x = (gridWidth >> 1) - 1;
            tile->state = S_0;
            break;
        case SQUARE:
            tile->position.y = 0;
            tile->position.x = (gridWidth >> 1) - 1;
            tile->state = SQR;
            break;
        case T:
            tile->position.y = 1;
            tile->position.x = (gridWidth >> 1) - 1;
            tile->state = T_0;
            break;
        case Z:
            tile->position.y = 1;
            tile->position.x = (gridWidth >> 1) - 1;
            tile->state = Z_0;
            break;
        case BACKGROUND:
            tile->position.y = -1;
            tile->position.x = -1;
            tile->state = STATE_UNKNOWN;
            break;
        default: goto failure;
    }

    if(coordinates != NULL) {
        tile->rect.x = coordinates->x;
        tile->rect.y = coordinates->y;
    }
    else {
        tile->rect.x = 0;
        tile->rect.y = 0;
    }
    tile->angle = 0;
    tile->color = color;
    tile->shape = shape;

    return tile;

    failure:
        if(!(flags & TILELOAD_NOTEXTURE) && tile->texture != NULL) {
            SDL_DestroyTexture(tile->texture);
        }
        free(tile);
        return NULL;
}

void freeTile(Tile* tile) {
    if(tile == NULL) return;
    if(tile->texture != NULL) SDL_DestroyTexture(tile->texture);
    free(tile);
}

Tile* loadTileRandom(SDL_Renderer* renderer, Point* coordinates, const int gridWidth, const int flags, FILE* debug) {
    TileColor color = rand() % (tileColorAmount - 1) + 1;
    TileShape shape = rand() % (tileColorAmount - 1) + 2;
    #ifdef DEBUG
    snprintf(loggingBuffer, loggingBufferSize, "[loadTileRandom] Color: %d, Shape: %d", color, shape);
    logToStream(debug, LOGLEVEL_DEBUG, NULL);
    #endif
    return loadTile(renderer, color, shape, coordinates, gridWidth, flags, debug);
}

void centerTileHorizontally(Tile* tile, ProgramParameters* programParameters) {
    tile->rect.x = programParameters->screenSize.width / 2 - tile->rect.w / 4;
}

void centerTileVertically(Tile* tile, ProgramParameters* programParameters) {
    tile->rect.y = programParameters->screenSize.height / 2 - tile->rect.h / 4;
}

void centerTile(Tile* tile, ProgramParameters* programParameters) {
    centerTileVertically(tile, programParameters);
    centerTileHorizontally(tile, programParameters);
}

void printTile(Tile* tile, FILE* stream) {
    if(tile == NULL) {
        logToStream(stream, LOGLEVEL_INFO, "[printTile] Nothing to print");
        return;
    }
    snprintf(loggingBuffer, loggingBufferSize, 
        "Tile color: %d\nTile shape: %d\nTile state: %d\nPosition: (%d, %d)\n", 
        tile->color, tile->shape, tile->state, tile->position.x, tile->position.y
    );
    logToStream(stream, LOGLEVEL_INFO, NULL);
}