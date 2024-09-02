#include "deus.h"
#include "logic_shapes.h"
#include "utils.h"

//There is kind of no way to make these functions shorter,
//as every case is a bit different and there are a lot of them.
//Edit: there actually is - just use a lookup table, dumbass.
//Man, now it's hardly readable, but it'll be easier to expand I guess.

//A lookup table for the amount of cells to set when moving left/right.
//Literally a lookup table... for a lookup 3D array, smh.
static const int moveLeftRightSetsAmount[STATE_AMOUNT] = {
    1, 1, //BAR_HORIZONTAL
    4, 4, //BAR_VERTICAL
    3, 2, 3, 2, //J_0/180, J_90/270
    3, 2, 3, 2, //L_0/180, L_90/270
    2, 3, 2, 3, //S_0/180, S_90/270
    2, //SQR
    2, 3, 2, 3, //T_0/180, T_90/270
    2, 3, 2, 3 //Z_0/180, Z_90/270
};

//Same here, but for moveDown.
static const int moveDownSetsAmount[STATE_AMOUNT] = {
    4, 4, //BAR_HORIZONTAL
    1, 1, //BAR_VERTICAL,
    2, 3, 2, 3, //J_0/180, J_90/270
    2, 3, 2, 3, //L_0/180, L_90/270
    3, 2, 3, 2, //S_0/180, S_90/270
    2, //SQR
    3, 2, 3, 2, //T_0/180, T_90/270
    3, 2, 3, 2 //Z_0/180, Z_90/270
};

//A lookup table for new positions when moving left
static const int moveLeftChecks[STATE_AMOUNT][maxTileSize][2] = {
    { {-1, 0} }, //BAR_HORIZONTAL
    { {-1, 0} }, //-||-
    { {-1, 0}, {-1, 1}, {-1, 2}, {-1, 3} }, //BAR_VERTICAL
    { {-1, 0}, {-1, 1}, {-1, 2}, {-1, 3} }, //-||-

    { {0, -2}, {0, -1}, {-1, 0} }, //J_0
    { {-1, 0}, {-1, 1} }, //J_90
    { {-2, 0}, {-2, 1}, {-2, 2} }, //J_180
    { {-1, 0}, {-3, -1} }, //J_270
    
    { {-2, -2}, {-2, -1}, {-2, 0} }, //L_0
    { {-1, 0}, {-1, -1} }, //L_90
    { {-1, 0}, {0, 1}, {0, 2} }, //L_180
    { {-1, 0}, {-3, 1} }, //L_270
    
    { {-1, 0},  {0, -1} }, //S_0
    { {-1, 0}, {-1, 1}, {0, 2} }, //S_90
    { {-1, 0},  {0, -1} }, //S_180
    { {-1, 0}, {-1, 1}, {0, 2} }, //S_270
    
    { {-1, 0}, {-1, 1} }, //SQR
    
    { {-2, -1}, {-1, 0} }, //T_0
    { {-1, 0}, {0, -1}, {0, 1} }, //T_90
    { {-1, 0}, {-2, 1} }, //T_180
    { {-2, -1}, {-2, 0}, {-2, 1} }, //T_270
    
    { {-1, 0}, {0, 1} }, //Z_0
    { {-1, 0}, {-1, 1}, {0, -1} }, //Z_90
    { {-1, 0}, {0, 1} }, //Z_180
    { {-1, 0}, {-1, 1}, {0, -1} } //Z_270
};

//A lookup table for new positions when moving right
static const int moveRightChecks[STATE_AMOUNT][maxTileSize][2] = {
    { {4, 0} }, //BAR_HORIZONTAL
    { {4, 0} }, //-||-
    { {1, 0}, {1, 1}, {1, 2}, {1, 3} }, //BAR_VERTICAL
    { {1, 0}, {1, 1}, {1, 2}, {1, 3} }, //-||-
    
    { {2, -2}, {2, -1}, {2, 0} }, //J_0
    { {1, 0}, {3, 1} }, //J_90
    { {1, 0}, {0, 1}, {0, 2} }, //J_180
    { {1, 0}, {1, -1} }, //J_270

    { {1, 0}, {0, -1}, {0, -2} }, //L_0
    { {1, 0}, {3, -1} }, //L_90
    { {2, 0}, {2, 1}, {2, 2} }, //L_180
    { {1, 0}, {1, 1} }, //L_270

    { {2, 0}, {3, -1} }, //S_0
    { {1, 0}, {2, 1}, {2, 2} }, //S_90
    { {2, 0}, {3, -1} }, //S_180
    { {1, 0}, {2, 1}, {2, 2} },  //S_270

    { {2, 0}, {2, 1} }, //SQR

    { {1, 0}, {2, -1} }, //T_0
    { {2, -1}, {2, 0}, {2, 1} }, //T_90
    { {1, 0}, {2, 1} }, //T_180
    { {1, 0}, {0, -1}, {0, 1} }, //T_270

    { {3, 1}, {2, 0} }, //Z_0
    { {2, 0}, {2, -1}, {1, 1} }, //Z_90
    { {3, 1}, {2, 0} }, //Z_180
    { {2, 0}, {2, -1}, {1, 1} }, //Z_270
};

//A lookup table for new positions when moving down
static const int moveDownChecks[STATE_AMOUNT][maxTileSize][2] = {
    { {0, 1}, {1, 1}, {2, 1}, {3, 1} }, //BAR_HORIZONTAL
    { {0, 1}, {1, 1}, {2, 1}, {3, 1} }, //-||-
    { {0, 4} }, //BAR_VERTICAL
    { {0, 4} }, //-||-

    { {0, 1}, {1, 1} }, //J_0
    { {0, 2}, {1, 2}, {2, 2} }, //J_90
    { {0, 1}, {-1, 3} }, //J_180
    { {0, 1}, {-1, 0}, {-2, 0} }, //J_270

    { {0, 1}, {-1, 1} }, //L_0
    { {0, 1}, {1, 0}, {2, 0} }, //L_90
    { {0, 1}, {1, 3} }, //L_180
    { {0, 2}, {-1, 2}, {-2, 2} }, //L_270

    { {0, 1}, {1, 1}, {2, 0} }, //S_0
    { {0, 2}, {1, 3} }, //S_90
    { {0, 1}, {1, 1}, {2, 0} }, //S_180
    { {0, 2}, {1, 3} }, //S_270

    { {0, 2}, {1, 2} }, //SQR

    { {0, 1}, {-1, 0}, {1, 0} }, //T_0
    { {0, 1}, {1, 2} }, //T_90
    { {-1, 2}, {0, 2}, {1, 2} }, //T_180
    { {0, 1}, {-1, 2} }, //T_270

    { {0, 1}, {1, 2}, {2, 2} }, //Z_0
    { {0, 2}, {1, 1} }, //Z_90
    { {0, 1}, {1, 2}, {2, 2} }, //Z_180
    { {0, 2}, {1, 1} }, //Z_270
};

//A lookup table for zeroing old positions after moving left
static const int moveLeftSets[STATE_AMOUNT][maxTileSize][2] = {
    { {3, 0} }, //BAR_HORIZONTAL
    { {3, 0} }, //-||-
    { {0, 0}, {0, 1}, {0, 2}, {0, 3} }, //BAR_VERTICAL
    { {0, 0}, {0, 1}, {0, 2}, {0, 3} }, //-||-

    { {1, -2}, {1, -1}, {1, 0} }, //J_0
    { {0, 0}, {2, 1} }, //J_90
    { {0, 0}, {-1, 1}, {-1, 2} }, //J_180
    { {0, 0}, {0, -1} }, //J_270

    { {-1, -2}, {-1, -1}, {0, 0} }, //L_0
    { {2, -1}, {0, 0} }, //L_90
    { {1, 0}, {1, 1}, {1, 2 } }, //L_180
    { {0, 0}, {0, 1} }, //L_270

    { {1, 0}, {2, -1} }, //S_0
    { {0, 0}, {1, 1}, {1, 2} }, //S_90
    { {1, 0}, {2, -1} }, //S_180
    { {0, 0}, {1, 1}, {1, 2} }, //S_270

    { {1, 0}, {1, 1} }, //SQR

    { {0, 0}, {1, -1} }, //T_0
    { {1, -1}, {1, 0}, {1, 1} }, //T_90
    { {0, 0}, {1, 1} }, //T_180
    { {-1, -1}, {0, 0}, {-1, 1} }, //T_270

    { {1, 0}, {2, 1} }, //Z_0
    { {0, 1}, {1, 0}, {1, -1} }, //Z_90
    { {1, 0}, {2, 1} }, //Z_180
    { {0, 1}, {1, 0}, {1, -1} } //Z_270
};

//A lookup table for zeroing old positions after moving right
static const int moveRightSets[STATE_AMOUNT][maxTileSize][2] = {
    { {0, 0} }, //BAR_HORIZONTAL
    { {0, 0} }, //-||-
    { {0, 0}, {0, 1}, {0, 2}, {0, 3} }, //BAR_VERTICAL
    { {0, 0}, {0, 1}, {0, 2}, {0, 3} }, //-||-

    { {1, -2}, {1, -1}, {0, 0} }, //J_0
    { {0, 0}, {0, 1} }, //J_90
    { {-1, 0}, {-1, 1}, {-1, 2} }, //J_180
    { {0, 0}, {-2, -1} }, //J_270

    { {-1, -2}, {-1, -1}, {-1, 0} }, //L_0
    { {0, -1}, {0, 0} }, //L_90
    { {0, 0}, {1, 1}, {1, 2} }, //L_180
    { {0, 0}, {-2, 1} }, //L_270

    { {0, 0}, {1, -1} }, //S_0
    { {0, 0}, {0, 1}, {1, 2} }, //S_90
    { {0, 0}, {1, -1} }, //S_180
    { {0, 0}, {0, 1}, {1, 2} }, //S_270

    { {0, 0}, {0, 1} }, //SQR

    { {0, 0}, {-1, -1} }, //T_0
    { {0, 0}, {1, -1}, {1, 1} }, //T_90
    { {0, 0}, {-1, 1} }, //T_180
    { {-1, -1}, {-1, 0}, {-1, 1} }, //T_270

    { {0, 0}, {1, 1} }, //Z_0
    { {0, 0}, {0, 1}, {1, -1} }, //Z_90
    { {0, 0}, {1, 1} }, //Z_180
    { {0, 0}, {0, 1}, {1, -1} }, //Z_270
};

static const int moveDownSets[STATE_AMOUNT][maxTileSize][2] = {
    { {0, 0}, {1, 0}, {2, 0}, {3, 0} }, //BAR_HORIZONTAL
    { {0, 0}, {1, 0}, {2, 0}, {3, 0} }, //-||-
    { {0, 0} }, //BAR_VERTICAL
    { {0, 0} }, //-||-

    { {0, 0}, {1, -2} }, //J_0
    { {0, 0}, {1, 1}, {2, 1} }, //J_90
    { {0, 0}, {-1, 0} }, //J_180
    { {-2, -1}, {-1, -1}, {0, -1} }, //J_270

    { {0, 0}, {-1, -2} }, //L_0
    { {0, -1}, {1, -1}, {2, -1} }, //L_90
    { {0, 0}, {1, 0} }, //L_180
    { {-2, 1}, {-1, 1}, {0, 0} }, //L_270

    { {0, 0}, {1, -1}, {2, -1} }, //S_0
    { {0, 0}, {1, 1} }, //S_90
    { {0, 0}, {1, -1}, {2, -1} }, //S_180
    { {0, 0}, {1, 1} }, //S_270

    { {0, 0}, {1, 0} }, //SQR

    { {-1, -1}, {0, -1}, {1, -1} }, //T_0
    { {0, 0}, {1, -1} }, //T_90
    { {-1, 1}, {0, 0}, {1, 1} }, //T_180
    { {0, 0}, {-1, -1} }, //T_270

    { {0, 0}, {1, 0}, {2, 1} }, //Z_0
    { {0, 0}, {1, -1} }, //Z_90
    { {0, 0}, {1, 0}, {2, 1} }, //Z_180
    { {0, 0}, {1, -1} }, //Z_270
};

status_t moveLeft(int** grid, Tile* tile) {
    if(tile == NULL) return MEMORY_FAILURE;
    int x = tile->position.x;
    int y = tile->position.y;
    if(x < 0 || y < 0 || tile->state == STATE_UNKNOWN) return FAILURE;

    Point new[occupiedAmount[tile->state]];
    Point old[occupiedAmount[tile->state]];
    
    for(int i = 0; i < moveLeftRightSetsAmount[tile->state]; i++) {
        new[i].x = x + moveLeftChecks[tile->state][i][0];
        if(new[i].x < 0) return INDEXOUTOFRANGE;
        new[i].y = y + moveLeftChecks[tile->state][i][1];
        //if the position isn't free, then movement isn't allowed
        if(
            grid[new[i].y][new[i].x] != 0 &&
            grid[new[i].y][new[i].x] != GHOST
        ) return FAILURE;
        
        old[i].x = x + moveLeftSets[tile->state][i][0];
        old[i].y = y + moveLeftSets[tile->state][i][1];
    }
    for(int i = 0; i < moveLeftRightSetsAmount[tile->state]; i++) {
        grid[new[i].y][new[i].x] = -tile->color;
        grid[old[i].y][old[i].x] = 0;
    }
    tile->position.x--;
    return SUCCESS;
}

status_t moveRight(int** grid, Tile* tile, const int gridWidth) {
    if(tile == NULL) return MEMORY_FAILURE;
    int x = tile->position.x;
    int y = tile->position.y;
    if(x < 0 || y < 0 || tile->state == STATE_UNKNOWN) return FAILURE;
    
    Point new[occupiedAmount[tile->state]];
    Point old[occupiedAmount[tile->state]]; 
    for(int i = 0; i < moveLeftRightSetsAmount[tile->state]; i++) {
        new[i].x = x + moveRightChecks[tile->state][i][0];
        if(new[i].x >= gridWidth) return INDEXOUTOFRANGE;
        new[i].y = y + moveRightChecks[tile->state][i][1];

        if(
            grid[new[i].y][new[i].x] != 0 &&
            grid[new[i].y][new[i].x] != GHOST
        ) return FAILURE;
        old[i].x = x + moveRightSets[tile->state][i][0];
        old[i].y = y + moveRightSets[tile->state][i][1];
    }
    for(int i = 0; i < moveLeftRightSetsAmount[tile->state]; i++) {
        grid[new[i].y][new[i].x] = -tile->color;
        grid[old[i].y][old[i].x] = 0;
    }
    tile->position.x++;
    return SUCCESS;
}

status_t moveDown(int** grid, Tile* tile, const int gridHeight) {
    if(tile == NULL) return MEMORY_FAILURE;
    int x = tile->position.x;
    int y = tile->position.y;
    if(x < 0 || y < 0 || tile->state == STATE_UNKNOWN) return FAILURE;

    Point new[occupiedAmount[tile->state]];
    Point old[occupiedAmount[tile->state]];

    for(int i = 0; i < moveDownSetsAmount[tile->state]; i++) {
        new[i].x = x + moveDownChecks[tile->state][i][0];
        new[i].y = y + moveDownChecks[tile->state][i][1];
        if(new[i].y >= gridHeight) return INDEXOUTOFRANGE;

        if(
            grid[new[i].y][new[i].x] != 0 &&
            grid[new[i].y][new[i].x] != GHOST
        ) return FAILURE;

        old[i].x = x + moveDownSets[tile->state][i][0];
        old[i].y = y + moveDownSets[tile->state][i][1];
    }
    for(int i = 0; i < moveDownSetsAmount[tile->state]; i++) {
        grid[new[i].y][new[i].x] = -tile->color;
        grid[old[i].y][old[i].x] = 0;
    }
    tile->position.y++;
    return SUCCESS;
}

//Inefficient, but works
void dropHard(ProgramParameters* parameters) {
    int distanceFallen = 0;
    while(moveDown(
        parameters->grid,
        parameters->currentTile,
        parameters->gridSize.height
    ) == SUCCESS) { distanceFallen++; }

    parameters->currentTile->position.x = -1;
    parameters->currentTile->position.y = -1;

    parameters->score += POINTS_HARDDROP * distanceFallen;
}