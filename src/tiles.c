#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <SDL.h>

#include "config.h"
#include "deus.h"
#include "logging.h"
#include "logic_shapes.h"
#include "utils.h"

const int defaultStates[SHAPE_AMOUNT] = {
    BAR_HORIZONTAL_UP, //BAR
    J_90, //J
    L_270, //L
    S_0, //S
    SQR, //SQUARE
    T_180, //T
    Z_0 //Z
};

const char shapeNames[SHAPE_AMOUNT][8] = {
    "Bar",
    "J",
    "L",
    "S",
    "Square",
    "T",
    "Z"
};

const char colorNames[COLOR_AMOUNT][12] = {
    "Background",
    "Aqua",
    "Blue",
    "Green",
    "Magenta",
    "Orange",
    "Red",
    "Yellow"
};

const char stateNames[STATE_AMOUNT][32] = {
    //BAR
    "Horizontal up",
    "Horizontal down",
    "Vertical left",
    "Vertical right",
    //J
    "Base",
    "Rotated by 90 degrees",
    "Rotated by 180 degrees",
    "Rotated by 270 degrees",
    //L
    "Base",
    "Rotated by 90 degrees",
    "Rotated by 180 degrees",
    "Rotated by 270 degrees",
    //S
    "Base",
    "Rotated by 90 degrees",
    "Rotated by 180 degrees",
    "Rotated by 270 degrees",
    //SQR
    "Square",
    //T
    "Base",
    "Rotated by 90 degrees",
    "Rotated by 180 degrees",
    "Rotated by 270 degrees",
    //Z
    "Base",
    "Rotated by 90 degrees",
    "Rotated by 180 degrees",
    "Rotated by 270 degrees",
};

status_t setDefaultTileParameters(Tile* tile, const int gridWidth) {
    switch(tile->shape) {
        case BAR:
            tile->position.y = 0;
            tile->position.x = (gridWidth >> 1) - 2;
            tile->state = BAR_HORIZONTAL_UP;
            break;
        case J:
            tile->position.y = 0;
            tile->position.x = (gridWidth >> 1) - 1;
            tile->state = J_90;
            break;
        case L:
            tile->position.y = 0;
            tile->position.x = (gridWidth >> 1) + 1;
            tile->state = L_270;
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
            tile->position.y = 0;
            tile->position.x = (gridWidth >> 1) - 1;
            tile->state = T_180;
            break;
        case Z:
            tile->position.y = 0;
            tile->position.x = (gridWidth >> 1) - 1;
            tile->state = Z_0;
            break;
        default: return FAILURE;
    }
    return SUCCESS;
}

Tile* loadTile(TileColor color, TileShape shape, Point* coordinates, const int gridWidth) {
    Tile* tile = malloc(sizeof(Tile));
    if(tile == NULL) return NULL;

    tile->color = color;
    tile->shape = shape;
    if(setDefaultTileParameters(tile, gridWidth) != SUCCESS) goto failure;

    if(coordinates != NULL) {
        tile->rect.x = coordinates->x;
        tile->rect.y = coordinates->y;
    }
    else {
        tile->rect.x = 0;
        tile->rect.y = 0;
    }

    return tile;

    failure:
        free(tile);
        return NULL;
}

void freeTile(Tile* tile) {
    if(tile == NULL) return;
    free(tile);
}

Tile* loadTileRandom(Point* coordinates, const int gridWidth, FILE* debug) {
    TileColor color = rand() % (tileColorAmount - 1) + (BACKGROUND + 1); //Actual colors start at BACKGROUND + 1
    TileShape shape = rand() % (tileColorAmount - 1);
#ifdef DEBUG
    snprintf(loggingBuffer, loggingBufferSize, "[loadTileRandom] Color: %s, Shape: %s", colorNames[color], shapeNames[shape]);
    logToStream(debug, LOGLEVEL_DEBUG, NULL);
#endif
    return loadTile(color, shape, coordinates, gridWidth);
}

void printTile(Tile* tile, FILE* stream) {
    if(tile == NULL) {
        logToStream(stream, LOGLEVEL_DEBUG, "[printTile] Nothing to print");
        return;
    }
    snprintf(loggingBuffer, loggingBufferSize, 
        "Tile color: %s\nTile shape: %s\nTile state: %s\nPosition: (%d, %d)\n", 
        colorNames[tile->color], shapeNames[tile->shape], stateNames[tile->state],
        tile->position.x, tile->position.y
    );
    logToStream(stream, LOGLEVEL_DEBUG, NULL);
}