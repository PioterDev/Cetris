#include "structs_unions_defines.h"
#include "utils.h"

status_t loadTileIntoGrid(char** tetrisGrid, Tile* tile) {
    int x = tile->position.x;
    int y = tile->position.y;
    switch(tile->state) {
        case BAR_HORIZONTAL_UP: {
            tetrisGrid[y][x]     = -1 * tile->color;
            tetrisGrid[y][x + 1] = -1 * tile->color;
            tetrisGrid[y][x + 2] = -1 * tile->color;
            tetrisGrid[y][x + 3] = -1 * tile->color;
            return SUCCESS;
        }
        case J_0: {
            tetrisGrid[y]    [x]     = -1 * tile->color;
            tetrisGrid[y]    [x + 1] = -1 * tile->color;
            tetrisGrid[y - 1][x + 1] = -1 * tile->color;
            tetrisGrid[y - 2][x + 1] = -1 * tile->color;
            return SUCCESS;
        }
        case L_0: {
            tetrisGrid[y]    [x]     = -1 * tile->color;
            tetrisGrid[y]    [x - 1] = -1 * tile->color;
            tetrisGrid[y - 1][x - 1] = -1 * tile->color;
            tetrisGrid[y - 2][x - 1] = -1 * tile->color;
            return SUCCESS;
        }
        case S_0: {
            tetrisGrid[y]    [x]     = -1 * tile->color;
            tetrisGrid[y]    [x + 1] = -1 * tile->color;
            tetrisGrid[y - 1][x + 1] = -1 * tile->color;
            tetrisGrid[y - 1][x + 2] = -1 * tile->color;
            return SUCCESS;
        }
        case SQR: {
            tetrisGrid[y]    [x]     = -1 * tile->color;
            tetrisGrid[y]    [x + 1] = -1 * tile->color;
            tetrisGrid[y + 1][x]     = -1 * tile->color;
            tetrisGrid[y + 1][x + 1] = -1 * tile->color;
            return SUCCESS;
        }
        case T_0: {
            tetrisGrid[y]    [x]     = -1 * tile->color;
            tetrisGrid[y - 1][x - 1] = -1 * tile->color;
            tetrisGrid[y - 1][x]     = -1 * tile->color;
            tetrisGrid[y - 1][x + 1] = -1 * tile->color;
            return SUCCESS;
        }
        case Z_0: {
            tetrisGrid[y]    [x]     = -1 * tile->color;
            tetrisGrid[y]    [x + 1] = -1 * tile->color;
            tetrisGrid[y + 1][x + 1] = -1 * tile->color;
            tetrisGrid[y + 1][x + 2] = -1 * tile->color;
            return SUCCESS;
        }
        default: return FAILURE;
    }
    return FAILURE;
}

/**
 * FIXME:
 * Movement can eat up through tiles
 */
status_t moveLeft(char** tetrisGrid, Tile* tile) {

    int x = tile->position.x;
    int y = tile->position.y;
    if(x == -1 || y == -1)return FAILURE;
    switch(tile->state) {
        case BAR_HORIZONTAL_UP:
        case BAR_HORIZONTAL_DOWN: {
            if(x > 0) {
                tetrisGrid[y][x - 1] = -1 * tile->color;

                tetrisGrid[y][x + 3] = 0;

                tile->position.x--;
                return SUCCESS;
            }
            break;
        }
        case BAR_VERTICAL_LEFT:
        case BAR_VERTICAL_RIGHT: {
            if(x > 0) {
                tetrisGrid[y]    [x - 1] = -1 * tile->color;
                tetrisGrid[y + 1][x - 1] = -1 * tile->color;
                tetrisGrid[y + 2][x - 1] = -1 * tile->color;
                tetrisGrid[y + 3][x - 1] = -1 * tile->color;

                tetrisGrid[y]    [x] = 0;
                tetrisGrid[y + 1][x] = 0;
                tetrisGrid[y + 2][x] = 0;
                tetrisGrid[y + 3][x] = 0;
                
                tile->position.x--;
                return SUCCESS;
            }
            break;
        }
        case J_0: {
            if(x > 0) {
                tetrisGrid[y - 2][x]     = -1 * tile->color;
                tetrisGrid[y - 1][x]     = -1 * tile->color;
                tetrisGrid[y]    [x - 1] = -1 * tile->color;

                tetrisGrid[y - 2][x + 1] = 0;
                tetrisGrid[y - 1][x + 1] = 0;
                tetrisGrid[y]    [x + 1] = 0;

                tile->position.x--;
                return SUCCESS;
            }
            break;
        }
        case J_90: {
            if(x > 0) {
                tetrisGrid[y]    [x - 1] = -1 * tile->color;
                tetrisGrid[y + 1][x - 1] = -1 * tile->color;
                
                tetrisGrid[y]    [x]     = 0;
                tetrisGrid[y + 1][x + 2] = 0;

                tile->position.x--;
                return SUCCESS;
            }
            break;
        }
        case J_180: {
            if(x > 2) {
                tetrisGrid[y]    [x - 2] = -1 * tile->color;
                tetrisGrid[y + 1][x - 2] = -1 * tile->color;
                tetrisGrid[y + 2][x - 2] = -1 * tile->color;

                tetrisGrid[y]    [x]     = 0;
                tetrisGrid[y + 1][x - 1] = 0;
                tetrisGrid[y + 2][x - 1] = 0;
                
                tile->position.x--;
                return SUCCESS;
            }
            break;
        }
        case J_270: {
            if(x > 2) {
                tetrisGrid[y]    [x - 1] = -1 * tile->color;
                tetrisGrid[y - 1][x - 3] = -1 * tile->color;

                tetrisGrid[y]    [x] = 0;
                tetrisGrid[y - 1][x] = 0;
                
                tile->position.x--;
                return SUCCESS;
            }
            break;
        }
        case L_0: {
            if(x > 1) {
                tetrisGrid[y - 2][x - 2] = -1 * tile->color;
                tetrisGrid[y - 1][x - 2] = -1 * tile->color;
                tetrisGrid[y]    [x - 2] = -1 * tile->color;

                tetrisGrid[y - 2][x - 1] = 0;
                tetrisGrid[y - 1][x - 1] = 0;
                tetrisGrid[y][x] = 0;

                tile->position.x--;
                return SUCCESS;
            }
            break;
        }
        case L_90: {
            if(x > 0) {
                tetrisGrid[y]    [x - 1] = -1 * tile->color;
                tetrisGrid[y - 1][x - 1] = -1 * tile->color;
                
                tetrisGrid[y - 1][x + 2] = 0;
                tetrisGrid[y]    [x]     = 0;
                
                tile->position.x--;
                return SUCCESS;
            }
            break;
        }
        case L_180: {
            if(x > 0) {
                tetrisGrid[y]    [x - 1] = -1 * tile->color;
                tetrisGrid[y + 1][x]     = -1 * tile->color;
                tetrisGrid[y + 2][x]     = -1 * tile->color;
                
                tetrisGrid[y]    [x + 1] = 0;
                tetrisGrid[y + 1][x + 1] = 0;
                tetrisGrid[y + 2][x + 1] = 0;
            
                tile->position.x--;
                return SUCCESS;
            }
            break;
        }
        case L_270: {
            if(x > 2) {
                tetrisGrid[y]    [x - 1] = -1 * tile->color;
                tetrisGrid[y + 1][x - 3] = -1 * tile->color;
                
                tetrisGrid[y]    [x] = 0;
                tetrisGrid[y + 1][x] = 0;

                tile->position.x--;
                return SUCCESS;
            }
            break;
        }
        case S_0:
        case S_180: {
            if(x > 0) {
                tetrisGrid[y][x - 1] = -1 * tile->color;
                tetrisGrid[y - 1][x] = -1 * tile->color;

                tetrisGrid[y]    [x + 1] = 0;
                tetrisGrid[y - 1][x + 2] = 0;
            
                tile->position.x--;
                return SUCCESS;
            }
            break;
        }
        case S_90:
        case S_270: {
            if(x > 0) {
                tetrisGrid[y]    [x - 1] = -1 * tile->color;
                tetrisGrid[y + 1][x - 1] = -1 * tile->color;
                tetrisGrid[y + 2][x]     = -1 * tile->color;

                tetrisGrid[y][x] = 0;
                tetrisGrid[y + 1][x + 1] = 0;
                tetrisGrid[y + 2][x + 1] = 0;
                
                tile->position.x--;
                return SUCCESS;
            }
            break;
        }
        case SQR: {
            if(x > 0) {
                tetrisGrid[y]    [x - 1] = -1 * tile->color;
                tetrisGrid[y + 1][x - 1] = -1 * tile->color;
                
                tetrisGrid[y]    [x + 1] = 0;
                tetrisGrid[y + 1][x + 1] = 0;

                tile->position.x--;
                return SUCCESS;
            }
            break;
        }
        case T_0: {
            if(x > 1) {
                tetrisGrid[y - 1][x - 2] = -1 * tile->color;
                tetrisGrid[y]    [x - 1] = -1 * tile->color;

                tetrisGrid[y]    [x]     = 0;
                tetrisGrid[y - 1][x + 1] = 0;

                tile->position.x--;
                return SUCCESS;
            }
            break;
        }
        case T_90: {
            if(x > 0) {
                tetrisGrid[y][x - 1] = -1 * tile->color;
                tetrisGrid[y - 1][x] = -1 * tile->color;
                tetrisGrid[y + 1][x] = -1 * tile->color;

                tetrisGrid[y - 1][x + 1] = 0;
                tetrisGrid[y]    [x + 1] = 0;
                tetrisGrid[y + 1][x + 1] = 0;
            
                tile->position.x--;
                return SUCCESS;
            }
            break;
        }
        case T_180: {
            if(x > 1) {
                tetrisGrid[y]    [x - 1] = -1 * tile->color;
                tetrisGrid[y + 1][x - 2] = -1 * tile->color;

                tetrisGrid[y]    [x] = 0;
                tetrisGrid[y + 1][x + 1] = 0;
                
                tile->position.x--;
                return SUCCESS;           
            }
            break;
        }
        case T_270: {
            if(x > 1) {
                tetrisGrid[y - 1][x - 2] = -1 * tile->color;
                tetrisGrid[y]    [x - 2] = -1 * tile->color;
                tetrisGrid[y + 1][x - 2] = -1 * tile->color;

                tetrisGrid[y - 1][x - 1] = 0;
                tetrisGrid[y]    [x]     = 0;
                tetrisGrid[y + 1][x - 1] = 0;

                tile->position.x--;
                return SUCCESS;
            }
            break;
        }

        case Z_0:
        case Z_180: {
            if(x > 0) {
                tetrisGrid[y][x - 1] = -1 * tile->color;
                tetrisGrid[y + 1][x] = -1 * tile->color;

                tetrisGrid[y]    [x + 1] = 0;
                tetrisGrid[y + 1][x + 2] = 0;

                tile->position.x--;
                return SUCCESS;
            }
            break;
        }
        case Z_90:
        case Z_270: {
            if(x > 0) {
                tetrisGrid[y]    [x - 1] = -1 * tile->color;
                tetrisGrid[y + 1][x - 1] = -1 * tile->color;
                tetrisGrid[y - 1][x]     = -1 * tile->color;

                tetrisGrid[y + 1][x] = 0;
                tetrisGrid[y][x + 1] = 0;
                tetrisGrid[y - 1][x + 1] = 0;
                
                tile->position.x--;
                return SUCCESS;
            }
            break;
        }
        default: return FAILURE;
    }
    return FAILURE;
}

/**
 * FIXME: 
 * Movement can eat up through tiles
 * L_0 going out of bounds
 * S_0, S_90 duplicating tiles
 * Z_0, Z_90 similar issue
 */
status_t moveRight(char** tetrisGrid, Tile* tile) {
    int x = tile->position.x;
    int y = tile->position.y;
    if(x == -1 || y == -1)return FAILURE;
    switch(tile->state) {
        case BAR_HORIZONTAL_UP:
        case BAR_HORIZONTAL_DOWN: {
            if(x < GridWidth - 4) {
                tetrisGrid[y][x + 4] = -1 * tile->color;

                tetrisGrid[y][x] = 0;

                tile->position.x++;
                return SUCCESS;
            }
            break;
        }
        case BAR_VERTICAL_LEFT:
        case BAR_VERTICAL_RIGHT: {
            if(x < GridWidth - 1) {
                tetrisGrid[y]    [x + 1] = -1 * tile->color;
                tetrisGrid[y + 1][x + 1] = -1 * tile->color;
                tetrisGrid[y + 2][x + 1] = -1 * tile->color;
                tetrisGrid[y + 3][x + 1] = -1 * tile->color;

                tetrisGrid[y]    [x] = 0;
                tetrisGrid[y + 1][x] = 0;
                tetrisGrid[y + 2][x] = 0;
                tetrisGrid[y + 3][x] = 0;
                
                tile->position.x++;
                return SUCCESS;
            }
            break;
        }
        case J_0: {
            if(x < GridWidth - 2) {
                tetrisGrid[y - 2][x + 2] = -1 * tile->color;
                tetrisGrid[y - 1][x + 2] = -1 * tile->color;
                tetrisGrid[y]    [x + 2] = -1 * tile->color;

                tetrisGrid[y - 2][x + 1] = 0;
                tetrisGrid[y - 1][x + 1] = 0;
                tetrisGrid[y]    [x]     = 0;

                tile->position.x++;
                return SUCCESS;
            }
            break;
        }
        case J_90: {
            if(x < GridWidth - 3) {
                tetrisGrid[y]    [x + 1] = -1 * tile->color;
                tetrisGrid[y + 1][x + 3] = -1 * tile->color;
                
                tetrisGrid[y]    [x] = 0;
                tetrisGrid[y + 1][x] = 0;

                tile->position.x++;
                return SUCCESS;
            }
            break;
        }
        case J_180: {
            if(x < GridWidth - 1) {
                tetrisGrid[y]    [x + 1] = -1 * tile->color;
                tetrisGrid[y + 1][x]     = -1 * tile->color;
                tetrisGrid[y + 2][x]     = -1 * tile->color;

                tetrisGrid[y]    [x - 1] = 0;
                tetrisGrid[y + 1][x - 1] = 0;
                tetrisGrid[y + 2][x - 1] = 0;
                
                tile->position.x++;
                return SUCCESS;
            }
            break;
        }
        case J_270: {
            if(x < GridWidth - 1) {
                tetrisGrid[y]    [x + 1] = -1 * tile->color;
                tetrisGrid[y - 1][x + 1] = -1 * tile->color;

                tetrisGrid[y]    [x]     = 0;
                tetrisGrid[y - 1][x - 2] = 0;
                
                tile->position.x++;
                return SUCCESS;
            }
            break;
        }
        case L_0: {
            if(x < GridWidth - 1) {
                tetrisGrid[y][x + 1] = -1 * tile->color;
                tetrisGrid[y - 1][x] = -1 * tile->color;
                tetrisGrid[y - 2][x] = -1 * tile->color;

                tetrisGrid[y - 2][x - 1] = 0;
                tetrisGrid[y - 1][x - 1] = 0;
                tetrisGrid[y]    [x - 1] = 0;

                tile->position.x++;
                return SUCCESS;
            }
            break;
        }
        case L_90: {
            if(x < GridWidth - 3) {
                tetrisGrid[y]    [x + 1] = -1 * tile->color;
                tetrisGrid[y - 1][x + 3] = -1 * tile->color;
                
                tetrisGrid[y - 1][x] = 0;
                tetrisGrid[y]    [x] = 0;
                
                tile->position.x++;
                return SUCCESS;
            }
            break;
        }
        case L_180: {
            if(x < GridWidth - 2) {
                tetrisGrid[y]    [x + 2] = -1 * tile->color;
                tetrisGrid[y + 1][x + 2] = -1 * tile->color;
                tetrisGrid[y + 2][x + 2] = -1 * tile->color;
                
                tetrisGrid[y]    [x]     = 0;
                tetrisGrid[y + 1][x + 1] = 0;
                tetrisGrid[y + 2][x + 1] = 0;
            
                tile->position.x++;
                return SUCCESS;
            }
            break;
        }
        case L_270: {
            if(x < GridWidth - 1) {
                tetrisGrid[y]    [x + 1] = -1 * tile->color;
                tetrisGrid[y + 1][x + 1] = -1 * tile->color;
                
                tetrisGrid[y]    [x]     = 0;
                tetrisGrid[y + 1][x - 2] = 0;

                tile->position.x++;
                return SUCCESS;
            }
            break;
        }
        case S_0:
        case S_180: {
            if(x < GridWidth - 3) {
                tetrisGrid[y]    [x + 2] = -1 * tile->color;
                tetrisGrid[y - 1][x + 3] = -1 * tile->color;

                tetrisGrid[y]    [x] = 0;
                tetrisGrid[y + 1][x + 1] = 0;
            
                tile->position.x++;
                return SUCCESS;
            }
            break;
        }
        case S_90:
        case S_270: {
            if(x < GridWidth - 2) {
                tetrisGrid[y]    [x + 1] = -1 * tile->color;
                tetrisGrid[y + 1][x + 2] = -1 * tile->color;
                tetrisGrid[y + 2][x + 2]     = -1 * tile->color;

                tetrisGrid[y][x] = 0;
                tetrisGrid[y + 1][x] = 0;
                tetrisGrid[y + 2][x + 1] = 0;
                
                tile->position.x++;
                return SUCCESS;
            }
            break;
        }
        case SQR: {
            if(x < GridWidth - 1) {
                tetrisGrid[y]    [x + 1] = -1 * tile->color;
                tetrisGrid[y + 1][x + 1] = -1 * tile->color;
                
                tetrisGrid[y]    [x - 1] = 0;
                tetrisGrid[y + 1][x - 1] = 0;

                tile->position.x++;
                return SUCCESS;
            }
            break;
        }
        case T_0: {
            if(x < GridWidth - 2) {
                tetrisGrid[y][x + 1] = -1 * tile->color;
                tetrisGrid[y - 1][x + 2] = -1 * tile->color;

                tetrisGrid[y]    [x]     = 0;
                tetrisGrid[y - 1][x - 1] = 0;

                tile->position.x++;
                return SUCCESS;
            }
            break;
        }
        case T_90: {
            if(x < GridWidth - 2) {
                tetrisGrid[y - 1][x + 2] = -1 * tile->color;
                tetrisGrid[y]    [x + 2] = -1 * tile->color;
                tetrisGrid[y + 1][x + 2] = -1 * tile->color;

                tetrisGrid[y]    [x] = 0;
                tetrisGrid[y - 1][x + 1] = 0;
                tetrisGrid[y + 1][x + 1] = 0;
            
                tile->position.x++;
                return SUCCESS;
            }
            break;
        }
        case T_180: {
            if(x < GridWidth - 2) {
                tetrisGrid[y]    [x + 1] = -1 * tile->color;
                tetrisGrid[y + 1][x + 2] = -1 * tile->color;

                tetrisGrid[y]    [x] = 0;
                tetrisGrid[y + 1][x - 1] = 0;
                
                tile->position.x++;
                return SUCCESS;           
            }
            break;
        }
        case T_270: {
            if(x < GridWidth - 1) {
                tetrisGrid[y]    [x + 1] = -1 * tile->color;
                tetrisGrid[y - 1][x]     = -1 * tile->color;
                tetrisGrid[y + 1][x]     = -1 * tile->color;

                tetrisGrid[y - 1][x - 1] = 0;
                tetrisGrid[y]    [x - 1] = 0;
                tetrisGrid[y + 1][x - 1] = 0;

                tile->position.x++;
                return SUCCESS;
            }
            break;
        }

        case Z_0:
        case Z_180: {
            if(x < GridWidth - 3) {
                tetrisGrid[y + 1][x + 3] = -1 * tile->color;
                tetrisGrid[y]    [x + 2] = -1 * tile->color;

                tetrisGrid[y]    [x]     = 0;
                tetrisGrid[y + 1][x + 1] = 0;

                tile->position.x++;
                return SUCCESS;
            }
            break;
        }
        case Z_90:
        case Z_270: {
            if(x < GridWidth - 2) {
                tetrisGrid[y]    [x + 2] = -1 * tile->color;
                tetrisGrid[y - 1][x + 2] = -1 * tile->color;
                tetrisGrid[y + 1][x + 1] = -1 * tile->color;

                tetrisGrid[y]    [x] = 0;
                tetrisGrid[y]    [x + 1] = 0;
                tetrisGrid[y - 1][x + 1] = 0;
                
                tile->position.x++;
                return SUCCESS;
            }
            break;
        }
        default: return FAILURE;
    }
    return FAILURE;
}

status_t moveDown(char** tetrisGrid, Tile* tile) {
    int x = tile->position.x;
    int y = tile->position.y;
    if(x == -1 || y == -1)return FAILURE;
    switch(tile->state) {
        case BAR_HORIZONTAL_UP:
        case BAR_HORIZONTAL_DOWN: {
            if(y < GridHeight - 1 && tetrisGrid[y + 1][x] == 0 && tetrisGrid[y + 1][x + 1] == 0 && tetrisGrid[y + 1][x + 2] == 0 && tetrisGrid[y + 1][x + 3] == 0) {
                tetrisGrid[y + 1][x]     = -1 * tile->color;
                tetrisGrid[y + 1][x + 1] = -1 * tile->color;
                tetrisGrid[y + 1][x + 2] = -1 * tile->color;
                tetrisGrid[y + 1][x + 3] = -1 * tile->color;

                tetrisGrid[y][x] = 0;
                tetrisGrid[y][x + 1] = 0;
                tetrisGrid[y][x + 2] = 0;
                tetrisGrid[y][x + 3] = 0;

                tile->position.y++;
                return SUCCESS;
            }
            break;
        }
        case BAR_VERTICAL_LEFT:
        case BAR_VERTICAL_RIGHT: {
            if(y < GridHeight - 4 && tetrisGrid[y + 4][x] == 0) {
                tetrisGrid[y + 4][x] = -1 * tile->color;

                tetrisGrid[y][x] = 0;

                tile->position.y++;

                return SUCCESS;                
            }
            break;
        }
        case J_0: {
            if(y < GridHeight - 1 && tetrisGrid[y + 1][x] == 0 && tetrisGrid[y + 1][x + 1] == 0) {
                tetrisGrid[y + 1][x]     = -1 * tile->color;
                tetrisGrid[y + 1][x + 1] = -1 * tile->color;

                tetrisGrid[y][x]         = 0;
                tetrisGrid[y - 2][x + 1] = 0;

                tile->position.y++;

                return SUCCESS;
            }
            break;
        }
        case J_90: {
            if(y < GridHeight - 2 && tetrisGrid[y + 2][x] == 0 && tetrisGrid[y + 2][x + 1] == 0 && tetrisGrid[y + 2][x + 2] == 0) {
                tetrisGrid[y + 2][x]     = -1 * tile->color;
                tetrisGrid[y + 2][x + 1] = -1 * tile->color;
                tetrisGrid[y + 2][x + 2] = -1 * tile->color;

                tetrisGrid[y][x]         = 0;
                tetrisGrid[y + 1][x + 1] = 0;
                tetrisGrid[y + 1][x + 2] = 0;

                tile->position.y++;

                return SUCCESS;
            }
            break;
        }
        case J_180: {
            if(y < GridHeight - 3 && tetrisGrid[y + 1][x] == 0 && tetrisGrid[y + 3][x - 1] == 0) {
                tetrisGrid[y + 1][x]     = -1 * tile->color;
                tetrisGrid[y + 3][x - 1] = -1 * tile->color;

                tetrisGrid[y][x]     = 0;
                tetrisGrid[y][x - 1] = 0;

                tile->position.y++;

                return SUCCESS;
            }
            break;
        }
        case J_270: {
            if(y < GridHeight - 1 && tetrisGrid[y + 1][x] == 0 && tetrisGrid[y][x - 1] == 0 && tetrisGrid[y][x - 2] == 0) {
                tetrisGrid[y + 1][x]     = -1 * tile->color;
                tetrisGrid[y]    [x - 1] = -1 * tile->color;
                tetrisGrid[y]    [x - 2] = -1 * tile->color;

                tetrisGrid[y + 1][x - 2] = 0;
                tetrisGrid[y + 1][x - 1] = 0;
                tetrisGrid[y + 1][x]     = 0;

                tile->position.y++;

                return SUCCESS;
            }
            break;
        }
        case L_0: {
            if(y < GridHeight - 1 && tetrisGrid[y + 1][x] == 0 && tetrisGrid[y + 1][x - 1] == 0) {
                tetrisGrid[y + 1][x]     = -1 * tile->color;
                tetrisGrid[y + 1][x - 1] = -1 * tile->color;

                tetrisGrid[y][x]         = 0;
                tetrisGrid[y - 2][x - 1] = 0;

                tile->position.y++;

                return SUCCESS;
            }
            break;
        }
        case L_90: {
            if(y < GridHeight - 1 && tetrisGrid[y + 1][x] == 0 && tetrisGrid[y][x + 1] == 0 && tetrisGrid[y][x + 2] == 0) {
                tetrisGrid[y + 1][x]     = -1 * tile->color;
                tetrisGrid[y]    [x + 1] = -1 * tile->color;
                tetrisGrid[y]    [x + 2] = -1 * tile->color;

                tetrisGrid[y - 1][x] = 0;
                tetrisGrid[y - 1][x + 1] = 0;
                tetrisGrid[y - 1][x + 2] = 0;

                tile->position.y++;

                return SUCCESS;
            }
            break;
        }
        case L_180: {
            if(y < GridHeight - 3 && tetrisGrid[y + 1][x] == 0 && tetrisGrid[y + 3][x + 1] == 0) {
                tetrisGrid[y + 1][x]     = -1 * tile->color;
                tetrisGrid[y + 3][x + 1] = -1 * tile->color;

                tetrisGrid[y][x]     = 0;
                tetrisGrid[y][x + 1] = 0;

                tile->position.y++;

                return SUCCESS;
            }
            break;
        }
        case L_270: {
            if(y < GridHeight - 2 && tetrisGrid[y + 2][x] == 0 && tetrisGrid[y + 2][x + 1] == 0 && tetrisGrid[y + 2][x + 2] == 0) {
                tetrisGrid[y + 2][x]     = -1 * tile->color;
                tetrisGrid[y + 2][x + 1] = -1 * tile->color;
                tetrisGrid[y + 2][x + 2] = -1 * tile->color;

                tetrisGrid[y + 1][x + 2] = 0;
                tetrisGrid[y + 1][x + 1] = 0;
                tetrisGrid[y][x] = 0;

                tile->position.y++;

                return SUCCESS;
            }
            break;
        }
        case S_0:
        case S_180: {
            if(y < GridHeight - 1 && tetrisGrid[y + 1][x] == 0 && tetrisGrid[y + 1][x + 1] == 0 && tetrisGrid[y][x + 2] == 0) {
                tetrisGrid[y + 1][x]     = -1 * tile->color;
                tetrisGrid[y + 1][x + 1] = -1 * tile->color;
                tetrisGrid[y]    [x + 2] = -1 * tile->color;

                tetrisGrid[y][x] = 0;
                tetrisGrid[y - 1][x + 1] = 0;
                tetrisGrid[y - 1][x + 2] = 0;

                tile->position.y++;

                return SUCCESS;
            }
            break;
        }
        case S_90:
        case S_270: {
            if(y < GridHeight - 3 && tetrisGrid[y + 2][x] == 0 && tetrisGrid[y + 3][x + 1] == 0) {
                tetrisGrid[y + 2][x]     = -1 * tile->color;
                tetrisGrid[y + 3][x + 1] = -1 * tile->color;

                tetrisGrid[y]    [x]     = 0;
                tetrisGrid[y + 1][x + 1] = 0;
            
                tile->position.y++;

                return SUCCESS;
            }
            break;
        }
        case SQR: {
            if(y < GridHeight - 2 && tetrisGrid[y + 2][x] == 0 && tetrisGrid[y + 2][x + 1] == 0) {
                tetrisGrid[y + 2][x]     = -1 * tile->color;
                tetrisGrid[y + 2][x + 1] = -1 * tile->color;

                tetrisGrid[y][x]     = 0;
                tetrisGrid[y][x + 1] = 0;
            
                tile->position.y++;

                return SUCCESS;
            }
            break;
        }
        case T_0: {
            if(y < GridHeight - 1 && tetrisGrid[y + 1][x] == 0 && tetrisGrid[y][x - 1] == 0 && tetrisGrid[y][x + 1] == 0) {
                tetrisGrid[y + 1][x]     = -1 * tile->color;
                tetrisGrid[y]    [x - 1] = -1 * tile->color;
                tetrisGrid[y]    [x + 1] = -1 * tile->color;

                tetrisGrid[y - 1][x - 1] = 0;
                tetrisGrid[y - 1][x]     = 0;
                tetrisGrid[y - 1][x + 1] = 0;
            
                tile->position.y++;

                return SUCCESS;
            }
            break;
        }
        case T_90: {
            if(y < GridHeight - 2 && tetrisGrid[y + 1][x] == 0 && tetrisGrid[y + 2][x + 1] == 0) {
                tetrisGrid[y + 1][x]     = -1 * tile->color;
                tetrisGrid[y + 2][x + 1] = -1 * tile->color;

                tetrisGrid[y]    [x]     = 0;
                tetrisGrid[y - 1][x + 1] = 0;
            
                tile->position.y++;

                return SUCCESS;
            }
            break;
        }
        case T_180: {
            if(y < GridHeight - 2 && tetrisGrid[y + 2][x - 1] == 0 && tetrisGrid[y + 2][x] == 0 && tetrisGrid[y + 2][x + 1] == 0) {
                tetrisGrid[y + 2][x - 1] = -1 * tile->color;
                tetrisGrid[y + 2][x]     = -1 * tile->color;
                tetrisGrid[y + 2][x + 1] = -1 * tile->color;

                tetrisGrid[y + 1][x - 1] = 0;
                tetrisGrid[y]    [x]     = 0;
                tetrisGrid[y + 1][x + 1] = 0;
            
                tile->position.y++;

                return SUCCESS;
            }
            break;
        }
        case T_270: {
            if(y < GridHeight - 2 && tetrisGrid[y + 1][x] == 0 && tetrisGrid[y + 2][x - 1] == 0) {
                tetrisGrid[y + 1][x]     = -1 * tile->color;
                tetrisGrid[y + 2][x - 1] = -1 * tile->color;

                tetrisGrid[y]    [x]     = 0;
                tetrisGrid[y - 1][x - 1] = 0;
            
                tile->position.y++;

                return SUCCESS;
            }
            break;
        }
        case Z_0:
        case Z_180: {
            if(y < GridHeight - 2 && tetrisGrid[y + 1][x] == 0 && tetrisGrid[y + 2][x + 1] == 0 && tetrisGrid[y + 2][x + 2] == 0) {
                tetrisGrid[y + 1][x]     = -1 * tile->color;
                tetrisGrid[y + 2][x + 1] = -1 * tile->color;
                tetrisGrid[y + 2][x + 2] = -1 * tile->color;

                tetrisGrid[y]    [x]     = 0;
                tetrisGrid[y]    [x + 1] = 0;
                tetrisGrid[y + 1][x + 2] = 0;
            
                tile->position.y++;

                return SUCCESS;
            }
            break;
        }
        case Z_90:
        case Z_270: {
            if(y < GridHeight - 2 && tetrisGrid[y + 2][x] == 0 && tetrisGrid[y + 1][x + 1] == 0) {
                tetrisGrid[y + 2][x]     = -1 * tile->color;
                tetrisGrid[y + 1][x + 1] = -1 * tile->color;

                tetrisGrid[y]    [x]     = 0;
                tetrisGrid[y - 1][x + 1] = 0;

                tile->position.y++;

                return SUCCESS;
            }
            break;
        }
        default: return FAILURE;
    }
    return FAILURE;
}

//You know what? Since I implemented moveDown and it works better,
//is newer and I'm tired, I'm just gonna replace dropHard with it.
//And now I realize I can't do this because moveDown doesn't
//set appropiate values in the matrix.
//...gonna have to rewrite dropHard from scratch. Eh.
status_t dropHard(char** tetrisGrid, Tile* tile) {
    while(moveDown(tetrisGrid, tile) == SUCCESS);
    return SUCCESS;
}
/**
 * FIXME: rewrite this shit from scratch 
 */
status_t dropHardOld(char** tetrisGrid, Tile* tile, const int tetrisGridHeight, FILE* debug) {
    //Convention is as follows:
    //(x, y) point to left-most square of the current tile.
    //If there are multiple, it points to the top-most one.
    //For T, J and L-shape, it points to the tile that "sticks out".
    int x = tile->position.x;
    int y = tile->position.y;
    switch(tile->state) {
        case BAR_HORIZONTAL_UP:
        case BAR_HORIZONTAL_DOWN: {

        }
    }
    switch(tile->shape) {
        case BAR: {
            if(tetrisGrid[y][x + 1] == -1 * tile->color) {
                //[][][][]
                int z[4] = {y + 1, y + 1, y + 1, y + 1};
 
                while(z[0] < tetrisGridHeight && tetrisGrid[z[0]][x]     == 0)z[0]++;
                while(z[1] < tetrisGridHeight && tetrisGrid[z[1]][x + 1] == 0)z[1]++;
                while(z[2] < tetrisGridHeight && tetrisGrid[z[2]][x + 2] == 0)z[2]++;
                while(z[3] < tetrisGridHeight && tetrisGrid[z[3]][x + 3] == 0)z[3]++;

                int min = Min(z, 4);
                min--;

                tetrisGrid[min][x]     = tile->color;
                tetrisGrid[min][x + 1] = tile->color;
                tetrisGrid[min][x + 2] = tile->color;
                tetrisGrid[min][x + 3] = tile->color;
                
                tetrisGrid[y][x]     = 0;
                tetrisGrid[y][x + 1] = 0;
                tetrisGrid[y][x + 2] = 0;
                tetrisGrid[y][x + 3] = 0;
            }
            else if(tetrisGrid[y + 1][x] == -1 * tile->color) {
                //[]
                //[]
                //[]
                //[]
                int z = y + 4;
                while(z < tetrisGridHeight && tetrisGrid[z][x] == 0)z++;
                z--;

                tetrisGrid[z]    [x] = tile->color;
                tetrisGrid[z + 1][x] = tile->color;
                tetrisGrid[z + 2][x] = tile->color;
                tetrisGrid[z + 3][x] = tile->color;

                tetrisGrid[y]    [x] = 0;
                tetrisGrid[y + 1][x] = 0;
                tetrisGrid[y + 2][x] = 0;
                tetrisGrid[y + 3][x] = 0;
            }
            break;
        }
        case J: {
            if(tetrisGrid[y][x + 1] == -1 * tile->color) {
                //  []
                //  []
                //[][]
                int z[2] = {y + 1, y + 1};

                while(z[0] < tetrisGridHeight && tetrisGrid[z[0]][x]     == 0)z[0]++;
                while(z[1] < tetrisGridHeight && tetrisGrid[z[1]][x + 1] == 0)z[1]++;

                int min = z[0] < z[1] ? z[0] : z[1];
                min--;

                tetrisGrid[min]     [x]     = tile->color;
                tetrisGrid[min]     [x + 1] = tile->color;
                tetrisGrid[min - 1] [x + 1] = tile->color;
                tetrisGrid[min - 2] [x + 1] = tile->color;

                tetrisGrid[y]    [x]     = 0;
                tetrisGrid[y]    [x + 1] = 0;
                tetrisGrid[y - 1][x + 1] = 0;
                tetrisGrid[y - 2][x + 1] = 0;
            }
            else if(tetrisGrid[y][x - 1] == -1 * tile->color) {
                //[][]
                //[]
                //[]
                int z[2] = {y + 1, y + 3};
        
                while(z[0] < tetrisGridHeight && tetrisGrid[z[0]][x]     == 0)z[0]++;
                while(z[1] < tetrisGridHeight && tetrisGrid[z[1]][x - 1] == 0)z[1]++;

                if(z[0] < z[1]) {
                    z[0]--;

                    tetrisGrid[z[0]]    [x]     = tile->color;
                    tetrisGrid[z[0]]    [x - 1] = tile->color;
                    tetrisGrid[z[0] + 1][x - 1] = tile->color;
                    tetrisGrid[z[0] + 2][x - 1] = tile->color;
                }
                else if(z[1] < z[0]) {
                    z[1]--;

                    tetrisGrid[z[1]]    [x - 1] = tile->color;
                    tetrisGrid[z[1] - 1][x - 1] = tile->color;
                    tetrisGrid[z[1] - 2][x - 1] = tile->color;
                    tetrisGrid[z[1] - 2][x]     = tile->color;
                }

                tetrisGrid[y]    [x]     = 0;
                tetrisGrid[y]    [x - 1] = 0;
                tetrisGrid[y - 1][x - 1] = 0;
                tetrisGrid[y - 2][x - 1] = 0;
            }
            else if(tetrisGrid[y + 1][x] == -1 * tile->color) {
                //[]
                //[][][]
                int z[3] = {y + 2, y + 2, y + 2};

                while(z[0] < tetrisGridHeight && tetrisGrid[z[0]][x]     == 0)z[0]++;
                while(z[1] < tetrisGridHeight && tetrisGrid[z[1]][x + 1] == 0)z[1]++;
                while(z[2] < tetrisGridHeight && tetrisGrid[z[2]][x + 2] == 0)z[2]++;

                int min = Min(z, 3);
                min--;

                tetrisGrid[min - 1] [x] = tile->color;
                tetrisGrid[min]     [x] = tile->color;
                tetrisGrid[min]     [x] = tile->color;
                tetrisGrid[min]     [x] = tile->color;

                tetrisGrid[y]    [x]     = 0;
                tetrisGrid[y + 1][x]     = 0;
                tetrisGrid[y + 1][x + 1] = 0;
                tetrisGrid[y + 1][x + 2] = 0;
            }
            else if(tetrisGrid[y - 1][x] == -1 * tile->color) {
                //[][][]
                //    []
                int z[3] = {y + 1, y + 1, y + 1};

                while(z[0] < tetrisGridHeight && tetrisGrid[z[0]][x]     == 0)z[0]++;
                while(z[1] < tetrisGridHeight && tetrisGrid[z[1]][x - 1] == 0)z[1]++;
                while(z[2] < tetrisGridHeight && tetrisGrid[z[2]][x - 2] == 0)z[2]++;
                
                int minIndex = MinIndex(z, 3);
                if(minIndex == 0) {
                    z[0]--;

                    tetrisGrid[z[0]]    [x]     = tile->color;
                    tetrisGrid[z[0] - 1][x]     = tile->color;
                    tetrisGrid[z[0] - 1][x - 1] = tile->color;
                    tetrisGrid[z[0] - 1][x - 2] = tile->color;
                }
                else {
                    z[minIndex]--;

                    tetrisGrid[z[minIndex] + 1][x]     = tile->color;
                    tetrisGrid[z[minIndex]]    [x]     = tile->color;
                    tetrisGrid[z[minIndex]]    [x - 1] = tile->color;
                    tetrisGrid[z[minIndex]]    [x - 2] = tile->color;
                }

                tetrisGrid[y]    [x]     = 0;
                tetrisGrid[y - 1][x]     = 0;
                tetrisGrid[y - 1][x - 1] = 0;
                tetrisGrid[y - 1][x - 2] = 0;
            }
            break;
        }
        case L: {
            if(tetrisGrid[y][x - 1] == -1 * tile->color) {
                //[]
                //[]
                //[][]
                int z[2] = {y + 1, y + 1};

                while(z[0] < tetrisGridHeight && tetrisGrid[z[0]][x]     == 0)z[0]++;
                while(z[1] < tetrisGridHeight && tetrisGrid[z[1]][x - 1] == 0)z[1]++;

                int min = z[0] < z[1] ? z[0] : z[1];
                min--;

                tetrisGrid[min]     [x]     = tile->color;
                tetrisGrid[min]     [x - 1] = tile->color;
                tetrisGrid[min - 1] [x - 1] = tile->color;
                tetrisGrid[min - 2] [x - 1] = tile->color;

                tetrisGrid[y]    [x]     = 0;
                tetrisGrid[y]    [x - 1] = 0;
                tetrisGrid[y - 1][x - 1] = 0;
                tetrisGrid[y - 2][x - 1] = 0;
            }
            else if(tetrisGrid[y][x + 1] == -1 * tile->color) {
                //[][]
                //  []
                //  []
                int z[2] = {y + 1, y + 3};

                while(z[0] < tetrisGridHeight && tetrisGrid[z[0]][x]     == 0)z[0]++;
                while(z[1] < tetrisGridHeight && tetrisGrid[z[1]][x + 1] == 0)z[1]++;

                if(z[0] < z[1]) {
                    z[0]--;

                    tetrisGrid[z[0]]    [x]     = tile->color;
                    tetrisGrid[z[0]]    [x + 1] = tile->color;
                    tetrisGrid[z[0] + 1][x + 1] = tile->color;
                    tetrisGrid[z[0] + 2][x + 1] = tile->color;
                }
                else if(z[1] < z[0]) {
                    z[1]--;

                    tetrisGrid[z[1]]    [x + 1] = tile->color;
                    tetrisGrid[z[1] - 1][x + 1] = tile->color;
                    tetrisGrid[z[1] - 2][x + 1] = tile->color;
                    tetrisGrid[z[1] - 2][x]     = tile->color;
                }

                tetrisGrid[y]    [x]     = 0;
                tetrisGrid[y]    [x + 1] = 0;
                tetrisGrid[y + 1][x + 1] = 0;
                tetrisGrid[y + 2][x + 1] = 0;
            }
            else if(tetrisGrid[y - 1][x] == -1 * tile->color) {
                //[][][]
                //[]
                int z[3] = {y + 1, y, y};

                while(z[0] < tetrisGridHeight && tetrisGrid[z[0]][x]     == 0)z[0]++;
                while(z[1] < tetrisGridHeight && tetrisGrid[z[1]][x + 1] == 0)z[1]++;
                while(z[2] < tetrisGridHeight && tetrisGrid[z[2]][x + 2] == 0)z[2]++;

                int minIndex = MinIndex(z, 3);
                if(minIndex == 0) {
                    z[0]--;

                    tetrisGrid[z[0]]    [x]     = tile->color;
                    tetrisGrid[z[0] - 1][x]     = tile->color;
                    tetrisGrid[z[0] - 1][x + 1] = tile->color;
                    tetrisGrid[z[0] - 1][x + 2] = tile->color;
                }
                else {
                    z[minIndex]--;

                    tetrisGrid[z[minIndex] + 1] [x]     = tile->color;
                    tetrisGrid[z[minIndex]]     [x]     = tile->color;
                    tetrisGrid[z[minIndex]]     [x + 1] = tile->color;
                    tetrisGrid[z[minIndex]]     [x + 2] = tile->color;
                }

                tetrisGrid[y]    [x]     = 0;
                tetrisGrid[y - 1][x]     = 0;
                tetrisGrid[y - 1][x + 1] = 0;
                tetrisGrid[y - 1][x + 2] = 0;
            }
            else if(tetrisGrid[y + 1][x] == -1 * tile->color) {
                //    []
                //[][][]
                int z[3] = {y + 2, y + 2, y + 2};

                while(z[0] < tetrisGridHeight && tetrisGrid[z[0]][x]     == 0)z[0]++;
                while(z[1] < tetrisGridHeight && tetrisGrid[z[1]][x - 1] == 0)z[1]++;
                while(z[2] < tetrisGridHeight && tetrisGrid[z[2]][x - 2] == 0)z[2]++;

                int min = Min(z, 3);
                min--;

                tetrisGrid[min - 1][x]     = tile->color;
                tetrisGrid[min]    [x]     = tile->color;
                tetrisGrid[min]    [x - 1] = tile->color;
                tetrisGrid[min]    [x - 2] = tile->color;

                tetrisGrid[y]    [x]     = 0;
                tetrisGrid[y + 1][x]     = 0;
                tetrisGrid[y + 1][x - 1] = 0;
                tetrisGrid[y + 1][x - 2] = 0;
            }
            break;
        }
        case S: {
            if(tetrisGrid[y][x + 1] == -1 * tile->color) {
                //  [][]
                //[][]
                int z[3] = {y + 1, y + 1, y};

                while(z[0] < tetrisGridHeight && tetrisGrid[z[0]][x]     == 0)z[0]++;
                while(z[1] < tetrisGridHeight && tetrisGrid[z[1]][x + 1] == 0)z[1]++;
                while(z[2] < tetrisGridHeight && tetrisGrid[z[2]][x + 2] == 0)z[2]++;

                int min = Min(z, 3);
                if(min == z[2]) {
                    min--;

                    tetrisGrid[min]    [x + 2] = tile->color; //upper right one
                    tetrisGrid[min]    [x + 1] = tile->color;
                    tetrisGrid[min + 1][x + 1] = tile->color;
                    tetrisGrid[min + 1][x]     = tile->color;
                }
                else {
                    min--;

                    tetrisGrid[min]    [x]     = tile->color;
                    tetrisGrid[min]    [x + 1] = tile->color;
                    tetrisGrid[min - 1][x + 1] = tile->color;
                    tetrisGrid[min - 1][x + 2] = tile->color;
                }

                tetrisGrid[y]    [x]     = 0;
                tetrisGrid[y]    [x + 1] = 0;
                tetrisGrid[y - 1][x + 1] = 0;
                tetrisGrid[y - 1][x + 2] = 0;
            }
            else if(tetrisGrid[y + 1][x] == -1 * tile->color) {
                //[]
                //[][]
                //  []
                int z[2] = {y + 3, y + 3};

                while(z[0] < tetrisGridHeight && tetrisGrid[z[0]][x]     == 0)z[0]++;
                while(z[1] < tetrisGridHeight && tetrisGrid[z[1]][x + 1] == 0)z[1]++;

                if(z[0] < z[1]) {
                    z[0]--;

                    tetrisGrid[z[0] + 1][x + 1] = tile->color; //bottom one
                    tetrisGrid[z[0]]    [x + 1] = tile->color;
                    tetrisGrid[z[0]]    [x]     = tile->color;
                    tetrisGrid[z[0] - 1][x]     = tile->color;
                }
                else if(z[1] < z[0]) {
                    z[1]--;

                    tetrisGrid[z[1]]    [x + 1] = tile->color;
                    tetrisGrid[z[1] - 1][x + 1] = tile->color;
                    tetrisGrid[z[1] - 1][x]     = tile->color;
                    tetrisGrid[z[1] - 2][x]     = tile->color;
                }

                tetrisGrid[y]    [x]     = 0;
                tetrisGrid[y + 1][x]     = 0;
                tetrisGrid[y + 1][x + 1] = 0;
                tetrisGrid[y + 2][x + 1] = 0;
            }
            break;
        }
        case SQUARE: {
            //[][]
            //[][]
            int z[2] = {y + 2, y + 2};

            while(z[0] < tetrisGridHeight && tetrisGrid[z[0]][x]     == 0)z[0]++;
            while(z[1] < tetrisGridHeight && tetrisGrid[z[1]][x + 1] == 0)z[1]++;

            int min = z[0] < z[1] ? z[0] : z[1];
            min--;

            tetrisGrid[min]    [x]      = tile->color;
            tetrisGrid[min]    [x + 1]  = tile->color;
            tetrisGrid[min - 1][x]      = tile->color;
            tetrisGrid[min - 1][x + 1]  = tile->color;

            tetrisGrid[y]    [x]     = 0;
            tetrisGrid[y]    [x + 1] = 0;
            tetrisGrid[y + 1][x]     = 0;
            tetrisGrid[y + 1][x + 1] = 0;
            break;
        }
        case T: {
            if(tetrisGrid[y][x - 1] == -1 * tile->color) {
                //[]
                //[][]
                //[]
                int z[2] = {y + 2, y + 1};

                while(z[0] < tetrisGridHeight && tetrisGrid[z[0]][x - 1] == 0)z[0]++;
                while(z[1] < tetrisGridHeight && tetrisGrid[z[1]][x]     == 0)z[1]++;

                if(z[0] < z[1]) {
                    z[0]--;
                    tetrisGrid[z[0]]    [x - 1] = tile->color;
                    tetrisGrid[z[0] - 1][x - 1] = tile->color;
                    tetrisGrid[z[0] - 1][x]     = tile->color;
                    tetrisGrid[z[0] - 2][x - 1] = tile->color;
                }
                else if(z[1] < z[0]) {
                    z[1]--;

                    tetrisGrid[z[1] + 1][x - 1] = tile->color;
                    tetrisGrid[z[1]]    [x - 1] = tile->color;
                    tetrisGrid[z[1]]    [x]     = tile->color;
                    tetrisGrid[z[1] - 1][x - 1] = tile->color;
                }
                
                tetrisGrid[y + 1][x - 1] = 0;
                tetrisGrid[y]    [x - 1] = 0;
                tetrisGrid[y]    [x]     = 0;
                tetrisGrid[y - 1][x - 1] = 0;
            }
            else if(tetrisGrid[y][x + 1] == -1 * tile->color) {
                //  []
                //[][]
                //  []
                int z[2] = {y + 2, y + 1};

                while(z[0] < tetrisGridHeight && tetrisGrid[z[0]][x + 1] == 0)z[0]++;
                while(z[1] < tetrisGridHeight && tetrisGrid[z[1]][x]     == 0)z[1]++;
                
                if(z[0] < z[1]) {
                    z[0]--;

                    tetrisGrid[z[0]]    [x + 1] = tile->color;
                    tetrisGrid[z[0] - 1][x + 1] = tile->color;
                    tetrisGrid[z[0] - 1][x]     = tile->color;
                    tetrisGrid[z[0] - 2][x + 1] = tile->color;
                }
                else if(z[1] < z[0]) {
                    z[1]--;

                    tetrisGrid[z[1] + 1][x + 1] = tile->color;
                    tetrisGrid[z[1]]    [x + 1] = tile->color;
                    tetrisGrid[z[1]]    [x]     = tile->color;
                    tetrisGrid[z[1] - 1][x + 1] = tile->color;
                }
                
                tetrisGrid[y + 1][x + 1] = 0;
                tetrisGrid[y]    [x + 1] = 0;
                tetrisGrid[y]    [x]     = 0;
                tetrisGrid[y - 1][x + 1] = 0;
            }
            else if(tetrisGrid[y - 1][x] == -1 * tile->color) {
                //[][][]
                //  []
                int z[3] = {y, y + 1, y};

                while(z[0] < tetrisGridHeight && tetrisGrid[z[0]][x - 1] == 0)z[0]++;
                while(z[1] < tetrisGridHeight && tetrisGrid[z[1]][x]     == 0)z[1]++;
                while(z[2] < tetrisGridHeight && tetrisGrid[z[2]][x + 1] == 0)z[2]++;

                int min = Min(z, 3);
                min--;

                tetrisGrid[min]    [x]     = tile->color;
                tetrisGrid[min - 1][x - 1] = tile->color;
                tetrisGrid[min - 1][x]     = tile->color;
                tetrisGrid[min - 1][x + 1] = tile->color;

                tetrisGrid[y]    [x]     = 0;
                tetrisGrid[y - 1][x - 1] = 0;
                tetrisGrid[y - 1][x]     = 0;
                tetrisGrid[y - 1][x + 1] = 0;
            }
            else if(tetrisGrid[y + 1][x] == -1 * tile->color) {
                //  []
                //[][][]
                int z[3] = {y + 2, y + 2, y + 2};

                while(z[0] < tetrisGridHeight && tetrisGrid[z[0]][x - 1] == 0)z[0]++;
                while(z[1] < tetrisGridHeight && tetrisGrid[z[1]][x]     == 0)z[1]++;
                while(z[2] < tetrisGridHeight && tetrisGrid[z[2]][x + 1] == 0)z[2]++;

                int min = Min(z, 3);
                min--;

                tetrisGrid[min - 1][x]     = tile->color;
                tetrisGrid[min]    [x]     = tile->color;
                tetrisGrid[min]    [x - 1] = tile->color;
                tetrisGrid[min]    [x + 1] = tile->color;

                tetrisGrid[y]    [x]     = 0;
                tetrisGrid[y + 1][x - 1] = 0;
                tetrisGrid[y + 1][x]     = 0;
                tetrisGrid[y + 1][x + 1] = 0;
            }
            break;
        }
        case Z: {
            if(tetrisGrid[y][x + 1] == -1 * tile->color) {
                //[][]
                //  [][]
                int z[3] = {y + 1, y + 1, y + 1};
                
                while(z[0] < tetrisGridHeight && tetrisGrid[z[0]][x]     == 0)z[0]++;
                while(z[1] < tetrisGridHeight && tetrisGrid[z[1]][x + 1] == 0)z[1]++;
                while(z[2] < tetrisGridHeight && tetrisGrid[z[2]][x + 2] == 0)z[2]++;

                int min = Min(z, 3);
                if(min == z[0]) {
                    min--;

                    tetrisGrid[min]    [x]     = tile->color; //upper left one
                    tetrisGrid[min]    [x + 1] = tile->color;
                    tetrisGrid[min + 1][x + 1] = tile->color;
                    tetrisGrid[min + 1][x + 2] = tile->color;
                }
                else {
                    min--;

                    tetrisGrid[min]    [x + 2] = tile->color; //bottom right one
                    tetrisGrid[min]    [x + 1] = tile->color;
                    tetrisGrid[min - 1][x + 1] = tile->color;
                    tetrisGrid[min - 1][x]     = tile->color;
                }

                tetrisGrid[y]    [x]     = 0;
                tetrisGrid[y]    [x + 1] = 0;
                tetrisGrid[y + 1][x + 1] = 0;
                tetrisGrid[y + 1][x + 2] = 0;
            }
            else if(tetrisGrid[y + 1][x] == -1 * tile->color) {
                //  []
                //[][]
                //[]
                int z[2] = {y + 2, y + 1};

                while(z[0] < tetrisGridHeight && tetrisGrid[z[0]][x]     == 0)z[0]++;
                while(z[1] < tetrisGridHeight && tetrisGrid[z[1]][x + 1] == 0)z[1]++;

                if(z[0] < z[1]) {
                    z[0]--;

                    tetrisGrid[z[0]]    [x]     = tile->color; //bottom one
                    tetrisGrid[z[0] - 1][x]     = tile->color;
                    tetrisGrid[z[0] - 1][x + 1] = tile->color;
                    tetrisGrid[z[0] - 2][x + 1] = tile->color;
                }
                else if(z[1] < z[0]) {
                    z[1]--;

                    tetrisGrid[z[1]]    [x]     = tile->color;
                    tetrisGrid[z[1]]    [x + 1] = tile->color;
                    tetrisGrid[z[1] - 1][x + 1] = tile->color;
                    tetrisGrid[z[1] + 1][x]     = tile->color;
                }

                tetrisGrid[y]    [x]     = 0;
                tetrisGrid[y + 1][x]     = 0;
                tetrisGrid[y]    [x + 1] = 0;
                tetrisGrid[y - 1][x + 1] = 0;
            }
            break;
        }
        default: return FAILURE;
    }
    return SUCCESS;
}

/**
 * FIXME:
 * BAR_VERTICAL_RIGHT not rotating properly
 * S, Z even worse
 */
status_t rotateClockwise(char** tetrisGrid, Tile* tile) {
    int x = tile->position.x;
    int y = tile->position.y;
    switch(tile->state) {
        case BAR_HORIZONTAL_UP: {
            if(y > 1 && tetrisGrid[y - 1][x + 2] == 0 && tetrisGrid[y + 1][x + 2] == 0 && tetrisGrid[y + 2][x + 2] == 0) {
                tetrisGrid[y - 1][x + 2] = -1 * tile->color;
                tetrisGrid[y + 1][x + 2] = -1 * tile->color;
                tetrisGrid[y + 2][x + 2] = -1 * tile->color;

                tetrisGrid[y][x]     = 0;
                tetrisGrid[y][x + 1] = 0;
                tetrisGrid[y][x + 3] = 0;

                tile->state = BAR_VERTICAL_RIGHT;
                
                tile->position.x = x + 2;
                tile->position.y = y - 1;

                return SUCCESS;
            }
            break;
        }
        case BAR_HORIZONTAL_DOWN: {
            if(y > 2 && tetrisGrid[y - 2][x + 1] == 0 && tetrisGrid[y - 1][x + 1] == 0 && tetrisGrid[y + 1][x + 1] == 0) {
                tetrisGrid[y - 2][x + 1] = -1 * tile->color;
                tetrisGrid[y - 1][x + 1] = -1 * tile->color;
                tetrisGrid[y + 1][x + 1] = -1 * tile->color;

                tetrisGrid[y][x]     = 0;
                tetrisGrid[y][x + 2] = 0;
                tetrisGrid[y][x + 3] = 0;

                tile->state = BAR_VERTICAL_LEFT;
                
                tile->position.x = x + 1;
                tile->position.y = y - 2;

                return SUCCESS;
            }
            break;
        }
        case BAR_VERTICAL_LEFT: {
            if(x > 1 && tetrisGrid[y + 1][x - 1] == 0 && tetrisGrid[y + 1][x + 1] == 0 && tetrisGrid[y + 1][x + 2] == 0) {
                tetrisGrid[y + 1][x - 1] = -1 * tile->color;
                tetrisGrid[y + 1][x + 1] = -1 * tile->color;
                tetrisGrid[y + 1][x + 2] = -1 * tile->color;

                tetrisGrid[y]    [x] = 0;
                tetrisGrid[y + 2][x] = 0;
                tetrisGrid[y + 3][x] = 0;

                tile->state = BAR_HORIZONTAL_UP;

                tile->position.x = x - 1;
                tile->position.y = y + 1;

                return SUCCESS;
            }
            break;
        }
        case BAR_VERTICAL_RIGHT: {
            if(x < 2 && tetrisGrid[y + 2][x - 2] == 0 && tetrisGrid[y + 2][x - 1] == 0 && tetrisGrid[y + 2][x + 1] == 0) {
                tetrisGrid[y + 2][x - 2] = -1 * tile->color;
                tetrisGrid[y + 2][x - 1] = -1 * tile->color;
                tetrisGrid[y + 2][x + 1] = -1 * tile->color;

                tetrisGrid[y]    [x] = 0;
                tetrisGrid[y + 1][x] = 0;
                tetrisGrid[y + 3][x] = 0;

                tile->state = BAR_HORIZONTAL_DOWN;

                tile->position.x = x - 2;
                tile->position.y = y + 2;

                return SUCCESS;
            }
            break;
        }
        case J_0: {
            if(y > 1 && tetrisGrid[y - 2][x] == 0 && tetrisGrid[y - 1][x] == 0 && tetrisGrid[y - 1][x + 2] == 0) {
                tetrisGrid[y - 2][x]     = -1 * tile->color;
                tetrisGrid[y - 1][x]     = -1 * tile->color;
                tetrisGrid[y - 1][x + 2] = -1 * tile->color;

                tetrisGrid[y]    [x]     = 0;
                tetrisGrid[y]    [x + 1] = 0;
                tetrisGrid[y - 2][x + 1] = 0;

                tile->state = J_90;

                tile->position.y -= 2;

                return SUCCESS;
            }
            break;
        }
        case J_90: {
            if(tetrisGrid[y][x + 1] == 0 && tetrisGrid[y][x + 2] == 0 && tetrisGrid[y + 2][x + 1] == 0) {
                tetrisGrid[y]    [x + 1] = -1 * tile->color;
                tetrisGrid[y]    [x + 2] = -1 * tile->color;
                tetrisGrid[y + 2][x + 1] = -1 * tile->color;

                tetrisGrid[y]    [x]     = 0;
                tetrisGrid[y + 1][x]     = 0;
                tetrisGrid[y + 1][x + 2] = 0;

                tile->state = J_180;

                tile->position.x += 2;

                return SUCCESS;
            }
            break;
        }
        case J_180: {
            if(x > 1 && tetrisGrid[y + 1][x - 2] == 0 && tetrisGrid[y + 1][x] == 0 && tetrisGrid[y + 2][x] == 0) {
                tetrisGrid[y + 1][x - 2] = -1 * tile->color;
                tetrisGrid[y + 1][x]     = -1 * tile->color;
                tetrisGrid[y + 2][x]     = -1 * tile->color;

                tetrisGrid[y]    [x]     = 0;
                tetrisGrid[y]    [x - 1] = 0;
                tetrisGrid[y + 2][x - 1] = 0;

                tile->state = J_270;

                tile->position.y += 2;

                return SUCCESS;
            }
            break;
        }
        case J_270: {
            if(y > 1 && tetrisGrid[y][x - 1] == 0 && tetrisGrid[y][x - 2] == 0 && tetrisGrid[y - 2][x - 1] == 0) {
                tetrisGrid[y]    [x - 1] = -1 * tile->color;
                tetrisGrid[y]    [x - 2] = -1 * tile->color;
                tetrisGrid[y - 2][x - 1] = -1 * tile->color;

                tetrisGrid[y]    [x]     = 0;
                tetrisGrid[y - 1][x]     = 0;
                tetrisGrid[y - 1][x - 2] = 0;

                tile->state = J_0;

                tile->position.x -= 2;

                return SUCCESS;
            }
            break;
        }
        case L_0: {
            if(x > 1 && tetrisGrid[y][x - 2] == 0 && tetrisGrid[y - 1][x - 2] == 0 && tetrisGrid[y - 1][x] == 0) {
                tetrisGrid[y]    [x - 2] = -1 * tile->color;
                tetrisGrid[y - 1][x - 2] = -1 * tile->color;
                tetrisGrid[y - 1][x]     = -1 * tile->color;

                tetrisGrid[y]    [x]     = 0;
                tetrisGrid[y][x - 1]     = 0;
                tetrisGrid[y - 2][x - 1] = 0;

                tile->state = L_90;

                tile->position.x -= 2;

                return SUCCESS;
            }
            break;
        }
        case L_90: {
            if(y > 1 && tetrisGrid[y][x + 1] == 0 && tetrisGrid[y - 2][x + 1] == 0 && tetrisGrid[y - 2][x] == 0) {
                tetrisGrid[y]    [x + 1] = -1 * tile->color;
                tetrisGrid[y - 2][x + 1] = -1 * tile->color;
                tetrisGrid[y - 2][x]     = -1 * tile->color;

                tetrisGrid[y]    [x]     = 0;
                tetrisGrid[y - 1][x]     = 0;
                tetrisGrid[y - 1][x + 2] = 0;

                tile->state = L_180;

                tile->position.y -= 2;

                return SUCCESS;
            }
            break;
        }
        case L_180: {
            if(x < GridWidth - 2 && tetrisGrid[y][x + 2] == 0 && tetrisGrid[y + 1][x + 2] == 0 && tetrisGrid[y + 1][x] == 0) {
                tetrisGrid[y]    [x + 2] = -1 * tile->color;
                tetrisGrid[y + 1][x + 2] = -1 * tile->color;
                tetrisGrid[y + 1][x]     = -1 * tile->color;

                tetrisGrid[y]    [x]     = 0;
                tetrisGrid[y]    [x + 1] = 0;
                tetrisGrid[y + 2][x + 1] = 0;

                tile->state = L_270;

                tile->position.x += 2;

                return SUCCESS;
            }
            break;
        }
        case L_270: {
            if(y < GridHeight - 2 && tetrisGrid[y + 2][x] == 0 && tetrisGrid[y + 2][x - 1] == 0 && tetrisGrid[y][x - 1] == 0) {
                tetrisGrid[y + 2][x]     = -1 * tile->color;
                tetrisGrid[y + 2][x - 1] = -1 * tile->color;
                tetrisGrid[y]    [x - 1] = -1 * tile->color;

                tetrisGrid[y]    [x]     = 0;
                tetrisGrid[y + 1][x]     = 0;
                tetrisGrid[y + 1][x - 2] = 0;

                tile->state = L_0;

                tile->position.y += 2;

                return SUCCESS;
            }
            break;
        }
        case S_0: {
            if(y < GridHeight - 2 && tetrisGrid[y + 1][x + 2] == 0 && tetrisGrid[y][x + 2] == 0) {
                tetrisGrid[y + 1][x + 2] = -1 * tile->color;
                tetrisGrid[y]    [x + 2] = -1 * tile->color;

                tetrisGrid[y]    [x]     = 0;
                tetrisGrid[y - 1][x + 2] = 0;

                tile->state = S_90;

                tile->position.x += 1;
                tile->position.y -= 1;

                return SUCCESS;
            }
            break;
        }
        case S_90: {
            if(x > 0 && tetrisGrid[y + 2][x] == 0 && tetrisGrid[y + 2][x - 1] == 0) {
                tetrisGrid[y + 2][x]     = -1 * tile->color;
                tetrisGrid[y + 2][x - 1] = -1 * tile->color;

                tetrisGrid[y]    [x]     = 0;
                tetrisGrid[y + 2][x + 1] = 0;

                tile->state = S_180;

                tile->position.x -= 1;
                tile->position.y += 2;

                return SUCCESS;
            }
            break;
        }
        case S_180: {
            if(y > 1 && tetrisGrid[y + 1][x] == 0 && tetrisGrid[y + 2][x] == 0) {
                tetrisGrid[y + 1][x] = -1 * tile->color;
                tetrisGrid[y + 2][x] = -1 * tile->color;

                tetrisGrid[y]    [x]     = 0;
                tetrisGrid[y - 1][x + 2] = 0;

                tile->state = S_270;

                tile->position.y -= 2;

                return SUCCESS;
            }
            break;
        }
        case S_270: {
            if(x < GridWidth - 2 && tetrisGrid[y][x + 1] == 0 && tetrisGrid[y][x + 1] == 0) {
                tetrisGrid[y][x + 1] = -1 * tile->color;
                tetrisGrid[y][x + 2] = -1 * tile->color;

                tetrisGrid[y]    [x]     = 0;
                tetrisGrid[y + 2][x + 1] = 0;

                tile->state = S_0;

                tile->position.y += 1;

                return SUCCESS;
            }
            break;
        }
        case SQR: return SUCCESS;
        case T_0: {
            if(y > 1 && tetrisGrid[y - 2][x] == 0) {
                tetrisGrid[y - 2][x] = -1 * tile->color;

                tetrisGrid[y - 1][x + 1] = 0;

                tile->state = T_90;

                tile->position.x -= 1;
                tile->position.y -= 1;

                return SUCCESS;
            }
            break;
        }
        case T_90: {
            if(x < GridWidth - 2 && tetrisGrid[y][x + 2] == 0) {
                tetrisGrid[y][x + 2] = -1 * tile->color;

                tetrisGrid[y + 1][x + 1] = 0;

                tile->state = T_180;

                tile->position.x += 1;
                tile->position.y -= 1;

                return SUCCESS;
            }
            break;
        }
        case T_180: {
            if(y < GridHeight - 2 && tetrisGrid[y + 2][x] == 0) {
                tetrisGrid[y + 2][x] = -1 * tile->color;

                tetrisGrid[y + 1][x - 1] = 0;

                tile->state = T_270;

                tile->position.x += 1;
                tile->position.y += 1;

                return SUCCESS;
            }
            break;
        }
        case T_270: {
            if(x > 1 && tetrisGrid[y][x - 2] == 0) {
                tetrisGrid[y][x - 2] = -1 * tile->color;

                tetrisGrid[y - 1][x - 1] = 0;

                tile->state = T_0;

                tile->position.x -= 1;
                tile->position.y += 1;

                return SUCCESS;
            }
            break;
        }
        case Z_0: {
            if(y < GridHeight - 2 && tetrisGrid[y][x + 2] == 0 && tetrisGrid[y + 2][x + 1] == 0) {
                tetrisGrid[y][x + 2] = -1 * tile->color;
                tetrisGrid[y + 2][x + 1] = -1 * tile->color;

                tetrisGrid[y][x] = 0;
                tetrisGrid[y][x + 1] = 0;

                tile->state = Z_90;

                tile->position.x += 1;
                tile->position.y += 2;

                return SUCCESS;
            }
            break;
        }
        case Z_90: {
            if(x > 0 && tetrisGrid[y][x - 1] == 0 && tetrisGrid[y + 1][x + 1] == 0) {
                tetrisGrid[y]    [x - 1] = -1 * tile->color;
                tetrisGrid[y + 2][x + 1] = -1 * tile->color;

                tetrisGrid[y - 1][x + 1] = 0;
                tetrisGrid[y]    [x + 1] = 0;

                tile->state = Z_180;

                tile->position.x -= 1;

                return SUCCESS;
            }
            break;
        }
        case Z_180: {
            if(y > 0 && tetrisGrid[y - 1][x + 1] == 0 && tetrisGrid[y + 1][x] == 0) {
                tetrisGrid[y - 1][x + 1] = -1 * tile->color;
                tetrisGrid[y + 1][x]     = -1 * tile->color;

                tetrisGrid[y + 1][x + 1] = 0;
                tetrisGrid[y + 1][x + 2] = 0;

                tile->state = Z_270;

                //position statys the same

                return SUCCESS;
            }
            break;
        }
        case Z_270: {
            if(x < GridWidth - 2 && tetrisGrid[y - 1][x] == 0 && tetrisGrid[y][x + 2] == 0) {
                tetrisGrid[y - 1][x]     = -1 * tile->color;
                tetrisGrid[y]    [x + 2] = -1 * tile->color;

                tetrisGrid[y][x] = 0;
                tetrisGrid[y + 1][x] = 0;

                tile->state = Z_0;

                tile->position.y -= 1;

                return SUCCESS;
            }
            break;
        }
        default: return FAILURE;
    }
    return FAILURE;
}