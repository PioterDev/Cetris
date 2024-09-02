#include "deus.h"
#include "logic_shapes.h"
#include "utils.h"

//A lookup table for the amount of cells to set when rotating.
static const int rotateSetsAmount[STATE_AMOUNT] = {
    3, 3, 3, 3, //BAR
    3, 3, 3, 3, //J
    3, 3, 3, 3, //L
    2, 2, 2, 2, //S
    0, //SQR
    1, 1, 1, 1, //T
    2, 2, 2, 2 //Z
};

//A lookup table for new positions when rotating clockwise
static const int rotateClockwiseChecks[STATE_AMOUNT][maxTileSize][2] = {
    { {2, -1}, {2, 1}, {2, 2} }, //BAR_HORIZONTAL_UP
    { {1, -2}, {1, -1}, {1, 1} }, //BAR_HORIZONTAL_DOWN
    { {-1, 1}, {1, 1}, {2, 1} }, //BAR_VERTICAL_LEFT
    { {-2, 2}, {-1, 2}, {1, 2} }, //BAR_VERTICAL_RIGHT

    { {0, -2}, {0, -1}, {2, -1} }, //J_0
    { {1, 0}, {2, 0}, {1, 2} }, //J_90
    { {-2, 1}, {0, 1}, {0, 2} }, //J_180
    { {-1, 0}, {-2, 0}, {-1, -2} }, //J_270

    { {-2, 0}, {-2, -1}, {0, -1} }, //L_0
    { {1, 0}, {1, -2}, {0, -2} }, //L_90
    { {2, 0}, {2, 1}, {0, 1} }, //L_180
    { {0, 2}, {-1, 2}, {-1, 0} }, //L_270

    { {2, 1}, {2, 0} }, //S_0
    { {0, 2}, {-1, 2} }, //S_90
    { {0, -1}, {0, -2} }, //S_180
    { {1, 0}, {2, 0} }, //S_270

    { {0} }, //SQR (no checks lol)
    
    { {0, -2} }, //T_0
    { {2, 0} }, //T_90
    { {0, 2} }, //T_180
    { {-2, 0} }, //T_270

    { {2, 0}, {1, 2} }, //Z_0
    { {-1, 0}, {1, 1} }, //Z_90
    { {1, -1}, {0, 1} }, //Z_180
    { {0, -1}, {2, 0} }, //Z_270
};

//A lookup table for zeroing old positions when rotating clockwise
static const int rotateClockwiseSets[STATE_AMOUNT][maxTileSize][2] = {
    { {0, 0}, {1, 0}, {3, 0} }, //BAR_HORIZONTAL_UP
    { {0, 0}, {2, 0}, {3, 0} }, //BAR_HORIZONTAL_DOWN
    { {0, 0}, {0, 2}, {0, 3} }, //BAR_VERTICAL_LEFT
    { {0, 0}, {0, 1}, {0, 3} }, //BAR_VERTICAL_RIGHT

    { {0, 0}, {1, 0}, {1, -2} }, //J_0
    { {0, 0}, {0, 1}, {2, 1} }, //J_90
    { {0, 0}, {-1, 0}, {-1, 2} }, //J_180
    { {0, 0}, {0, -1}, {-2, -1} }, //J_270

    { {0, 0}, {-1, 0}, {-1, -2} }, //L_0
    { {0, 0}, {0, -1}, {2, -1} }, //L_90
    { {0, 0}, {1, 0}, {1, 2} }, //L_180
    { {0, 0}, {0, 1}, {-2, 1} }, //L_270

    { {0, 0}, {2, -1} }, //S_0
    { {0, 0}, {1, 2} }, //S_90
    { {0, 0}, {2, -1} }, //S_180
    { {0, 0}, {1, 2} }, //S_270

    { {0} }, //SQR

    { {1, -1} }, //T_0
    { {1, 1} }, //T_90
    { {-1, 1} }, //T_180
    { {-1, -1} }, //T_270

    { {0, 0}, {1, 0} }, //Z_0
    { {1, -1}, {1, 0} }, //Z_90
    { {1, 1}, {2, 1} }, //Z_180
    { {0, 0}, {0, 1} }, //Z_270
};

//A lookup table for calculating the new position when rotating clockwise
static const int rotateClockwisePositionChange[STATE_AMOUNT][2] = {
    {2, -1}, //BAR_HORIZONTAL_UP
    {1, -2}, //BAR_HORIZONTAL_DOWN
    {-1, 1}, //BAR_VERTICAL_LEFT
    {-2, 2}, //BAR_VERTICAL_RIGHT

    {0, -2}, //J_0
    {2, 0}, //J_90
    {0, 2}, //J_180
    {-2, 0}, //J_270

    {-2, 0}, //L_0
    {0, -2}, //L_90
    {2, 0}, //L_180
    {0, 2}, //L_270

    {1, -1}, //S_0
    {-1, 2}, //S_90
    {0, -2}, //S_180
    {0, 1}, //S_270

    {0, 0}, //SQR

    {-1, -1}, //T_0
    {1, -1}, //T_90
    {1, 1}, //T_180
    {-1, 1}, //T_270

    {1, 1}, //Z_0
    {-1, 0}, //Z_90
    {0, 0}, //Z_180,
    {0, -1}, //Z_270
};

//A lookup table for assigning a new state after rotating clockwise
static const TileState rotateClockwiseNewState[STATE_AMOUNT] = {
    BAR_VERTICAL_RIGHT, BAR_VERTICAL_LEFT,
    BAR_HORIZONTAL_UP, BAR_HORIZONTAL_DOWN, //BAR
    
    J_90, J_180, J_270, J_0, //J

    L_90, L_180, L_270, L_0, //L

    S_90, S_180, S_270, S_0, //S

    SQR, //SQR

    T_90, T_180, T_270, T_0, //T

    Z_90, Z_180, Z_270, Z_0, //Z
};

static const int rotateCounterClockwiseChecks[STATE_AMOUNT][maxTileSize][2] = {
    { {1, -1}, {1, 1}, {1, 2} }, //BAR_HORIZONTAL_UP
    { {2, -2}, {2, -1}, {2, 1} }, //BAR_HORIZONTAL_DOWN
    { {-1, 2}, {1, 2}, {2, 2} }, //BAR_VERTICAL_LEFT
    { {-2, 1}, {-1, 1}, {1, 1} }, //BAR_VERTICAL_RIGHT

    { {2, 0}, {0, -1}, {2, -1} }, //J_0
    { {0, 2}, {1, 2}, {1, 0} }, //J_90
    { {-2, 0}, {-2, 1}, {0, 1} }, //J_180
    { {0, -2}, {-1, -2}, {-1, 0} }, //J_270

    { {0, -2}, {0, -1}, {-2, -1} }, //L_0
    { {1, 0}, {2, 0}, {1, -2} }, //L_90
    { {0, 1}, {0, 2}, {2, 1} }, //L_180
    { {-1, 0}, {-2, 0}, {-1, 2} }, //L_270

    { {0, -1}, {1, 1} }, //S_0
    { {-1, 1}, {1, 0} }, //S_90
    { {1, -2}, {2, 0} }, //S_180
    { {0, 2}, {2, 1} }, //S_270

    { {0} }, //SQR

    { {0, -2} }, //T_0
    { {2, 0} }, //T_90
    { {0, 2} }, //T_180
    { {-2, 0} }, //T_270

    { {0, 1}, {0, 2} }, //Z_0
    { {0, -1}, {-1, -1} }, //Z_90
    { {2, 0}, {2, -1} }, //Z_180
    { {1, 1}, {2, 1} }, //Z_270
};

static const int rotateCounterClockwiseSets[STATE_AMOUNT][maxTileSize][2] = {
    { {0, 0}, {2, 0}, {3, 0} }, //BAR_HORIZONTAL_UP
    { {0, 0}, {1, 0}, {3, 0} }, //BAR_HORIZONTAL_DOWN
    { {0, 0}, {0, 1}, {0, 3} }, //BAR_VERTICAL_LEFT
    { {0, 0}, {0, 2}, {0, 3} }, //BAR_VERTICAL_RIGHT

    { {0, 0}, {1, 0}, {1, -2} }, //J_0
    { {0, 0}, {0, 1}, {2, 1} }, //J_90
    { {0, 0}, {-1, 0}, {-1, 2} }, //J_180
    { {0, 0}, {0, -1}, {-2, -1} }, //J_270

    { {0, 0}, {-1, 0}, {-1, -2} }, //L_0
    { {0, 0}, {0, -1}, {2, -1} }, //L_90
    { {0, 0}, {1, 0}, {1, 2} }, //L_180
    { {0, 0}, {0, 1}, {-2, 1} }, //L_270

    { {1, -1}, {2, -1} }, //S_0
    { {1, 1}, {1, 2} }, //S_90
    { {0, 0}, {1, 0} }, //S_180
    { {0, 0}, {0, 1} }, //S_270

    { {0} }, //SQR

    { {-1, -1} }, //T_0
    { {1, -1} }, //T_90
    { {1, 1} }, //T_180
    { {-1, 1} }, //T_270

    { {0, 0}, {2, 1} }, //Z_0
    { {1, -1}, {0, 1} }, //Z_90
    { {0, 0}, {2, 1} }, //Z_180
    { {1, -1}, {0, 1} }, //Z_270
};

static const int rotateCounterClockwisePositionChange[STATE_AMOUNT][2] = {
    {1, -1}, //BAR_HORIZONTAL_UP
    {2, -2}, //BAR_HORIZONTAL_DOWN
    {-1, 2}, //BAR_VERTICAL_LEFT
    {-2, 1}, //BAR_VERTICAL_RIGHT

    {2, 0}, //J_0
    {0, 2}, //J_90
    {-2, 0}, //J_180
    {0, -2}, //J_270

    {0, -2}, //L_0
    {2, 0}, //L_90
    {0, 2}, //L_180
    {-2, 0}, //L_270

    {0, -1}, //S_0
    {-1, 1}, //S_90
    {1, -2}, //S_180
    {0, 2}, //S_270

    {0, 0}, //SQR

    {1, -1}, //T_0
    {1, 1}, //T_90
    {-1, 1}, //T_180
    {-1, -1}, //T_270

    {0, 1}, //Z_0
    {-1, -1}, //Z_90
    {1, 0}, //Z_180
    {0, 0}, //Z_270
};

static const TileState rotateCounterClockwiseNewState[STATE_AMOUNT] = {
    BAR_VERTICAL_LEFT, BAR_VERTICAL_RIGHT,
    BAR_HORIZONTAL_DOWN, BAR_HORIZONTAL_UP,
    
    J_270, J_0, J_90, J_180,

    L_270, L_0, L_90, L_180,

    S_270, S_0, S_90, S_180,

    SQR,

    T_270, T_0, T_90, T_180,

    Z_270, Z_0, Z_90, Z_270,
};

status_t rotateClockwise(int** grid, Tile* tile, const Size gridSize) {
    if(tile == NULL) return MEMORY_FAILURE;
    int x = tile->position.x;
    int y = tile->position.y;
    if(x < 0 || y < 0 || tile->state == STATE_UNKNOWN) return FAILURE;

    Point new[occupiedAmount[tile->state]];
    Point old[occupiedAmount[tile->state]];

    for(int i = 0; i < rotateSetsAmount[tile->state]; i++) {
        new[i].x = x + rotateClockwiseChecks[tile->state][i][0];
        new[i].y = y + rotateClockwiseChecks[tile->state][i][1];
        if(
            new[i].x < 0 || new[i].x >= (int)gridSize.width ||
            new[i].y < 0 || new[i].y >= (int)gridSize.height
        ) return INDEXOUTOFRANGE;
        if(
            grid[new[i].y][new[i].x] != 0 &&
            grid[new[i].y][new[i].x] != GHOST

        ) return FAILURE;

        old[i].x = x + rotateClockwiseSets[tile->state][i][0];
        old[i].y = y + rotateClockwiseSets[tile->state][i][1];
    }
    for(int i = 0; i < rotateSetsAmount[tile->state]; i++) {
        grid[new[i].y][new[i].x] = -tile->color;
        grid[old[i].y][old[i].x] = 0;
    }
    tile->position.x += rotateClockwisePositionChange[tile->state][0];
    tile->position.y += rotateClockwisePositionChange[tile->state][1];
    tile->state = rotateClockwiseNewState[tile->state];

    return SUCCESS;
}

status_t rotateCounterClockwise(int** grid, Tile* tile, const Size gridSize) {
    if(tile == NULL) return MEMORY_FAILURE;
    int x = tile->position.x;
    int y = tile->position.y;
    if(x < 0 || y < 0 || tile->state == STATE_UNKNOWN) return FAILURE;

    Point new[occupiedAmount[tile->state]];
    Point old[occupiedAmount[tile->state]];

    for(int i = 0; i < rotateSetsAmount[tile->state]; i++) {
        new[i].x = x + rotateCounterClockwiseChecks[tile->state][i][0];
        new[i].y = y + rotateCounterClockwiseChecks[tile->state][i][1];
        if(
            new[i].x < 0 || new[i].x >= (int)gridSize.width ||
            new[i].y < 0 || new[i].y >= (int)gridSize.height
        ) return INDEXOUTOFRANGE;
        if(
            grid[new[i].y][new[i].x] != 0 &&
            grid[new[i].y][new[i].x] != GHOST

        ) return FAILURE;

        old[i].x = x + rotateCounterClockwiseSets[tile->state][i][0];
        old[i].y = y + rotateCounterClockwiseSets[tile->state][i][1];
    }
    for(int i = 0; i < rotateSetsAmount[tile->state]; i++) {
        grid[new[i].y][new[i].x] = -tile->color;
        grid[old[i].y][old[i].x] = 0;
    }
    tile->position.x += rotateCounterClockwisePositionChange[tile->state][0];
    tile->position.y += rotateCounterClockwisePositionChange[tile->state][1];
    tile->state = rotateCounterClockwiseNewState[tile->state];

    return SUCCESS;
}