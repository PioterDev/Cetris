#include "structs_unions_defines.h"
#include "utils.h"

status_t moveLeft(int** tetrisGrid, Tile* tile) {
    if(tile == NULL)return MEMORY_FAILURE;
    int x = tile->position.x;
    int y = tile->position.y;
    if(x < 0 || y < 0)return FAILURE;
    switch(tile->state) {
        case BAR_HORIZONTAL_UP:
        case BAR_HORIZONTAL_DOWN: {
            if(x > 0 && tetrisGrid[y][x - 1] == 0) {
                tetrisGrid[y][x - 1] = -1 * tile->color;

                tetrisGrid[y][x + 3] = 0;

                tile->position.x--;
                return SUCCESS;
            }
            break;
        }
        case BAR_VERTICAL_LEFT:
        case BAR_VERTICAL_RIGHT: {
            if(x > 0 && tetrisGrid[y][x - 1] == 0 && tetrisGrid[y + 1][x - 1] == 0 && tetrisGrid[y + 2][x - 1] == 0 && tetrisGrid[y + 3][x - 1] == 0) {
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
            if(x > 0 && tetrisGrid[y - 2][x] == 0 && tetrisGrid[y - 1][x] == 0 && tetrisGrid[y][x - 1] == 0) {
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
            if(x > 0 && tetrisGrid[y][x - 1] == 0 && tetrisGrid[y + 1][x - 1] == 0) {
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
            if(x > 1 && tetrisGrid[y][x - 2] == 0 && tetrisGrid[y + 1][x - 2] == 0 && tetrisGrid[y + 2][x - 2] == 0) {
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
            if(x > 2 && tetrisGrid[y][x - 1] == 0 && tetrisGrid[y - 1][x - 3] == 0) {
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
            if(x > 1 && tetrisGrid[y - 2][x - 2] == 0 && tetrisGrid[y - 1][x - 2] == 0 && tetrisGrid[y][x - 2] == 0) {
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
            if(x > 0 && tetrisGrid[y][x - 1] == 0 && tetrisGrid[y - 1][x - 1] == 0) {
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
            if(x > 0 && tetrisGrid[y][x - 1] == 0 && tetrisGrid[y + 1][x] == 0 && tetrisGrid[y + 2][x] == 0) {
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
            if(x > 2 && tetrisGrid[y][x - 1] == 0 && tetrisGrid[y + 1][x - 3] == 0) {
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
            if(x > 0 && tetrisGrid[y][x - 1] == 0 && tetrisGrid[y -1][x] == 0) {
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
            if(x > 0 && tetrisGrid[y][x - 1] == 0 && tetrisGrid[y + 1][x - 1] == 0 && tetrisGrid[y + 2][x] == 0) {
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
            if(x > 0 && tetrisGrid[y][x - 1] == 0 && tetrisGrid[y + 1][x - 1] == 0) {
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
            if(x > 1 && tetrisGrid[y - 1][x - 2] == 0 && tetrisGrid[y][x - 1] == 0) {
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
            if(x > 0 && tetrisGrid[y][x - 1] == 0 && tetrisGrid[y - 1][x] == 0 && tetrisGrid[y + 1][x] == 0) {
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
            if(x > 1 && tetrisGrid[y][x - 1] == 0 && tetrisGrid[y + 1][x - 2] == 0) {
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
            if(x > 1 && tetrisGrid[y - 1][x - 2] == 0 && tetrisGrid[y][x - 2] == 0 && tetrisGrid[y + 1][x - 2] == 0) {
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
            if(x > 0 && tetrisGrid[y][x - 1] == 0 && tetrisGrid[y + 1][x] == 0) {
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
            if(x > 0 && tetrisGrid[y][x - 1] == 0 && tetrisGrid[y + 1][x - 1] == 0 && tetrisGrid[y - 1][x] == 0) {
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

status_t moveRight(int** tetrisGrid, Tile* tile, const int tetrisGridWidth) {
    if(tile == NULL)return MEMORY_FAILURE;
    int x = tile->position.x;
    int y = tile->position.y;
    if(x < 0 || y < 0)return FAILURE;
    switch(tile->state) {
        case BAR_HORIZONTAL_UP:
        case BAR_HORIZONTAL_DOWN: {
            if(x < tetrisGridWidth - 4 && tetrisGrid[y][x + 4] == 0) {
                tetrisGrid[y][x + 4] = -1 * tile->color;

                tetrisGrid[y][x] = 0;

                tile->position.x++;
                return SUCCESS;
            }
            break;
        }
        case BAR_VERTICAL_LEFT:
        case BAR_VERTICAL_RIGHT: {
            if(x < tetrisGridWidth - 1 && tetrisGrid[y][x + 1] == 0 && tetrisGrid[y + 1][x + 1] == 0 && tetrisGrid[y + 2][x + 1] == 0 && tetrisGrid[y + 3][x + 1] == 0) {
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
            if(x < tetrisGridWidth - 2 && tetrisGrid[y - 2][x + 2] == 0 && tetrisGrid[y - 1][x + 2] == 0 && tetrisGrid[y][x + 2] == 0) {
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
            if(x < tetrisGridWidth - 3 && tetrisGrid[y][x + 1] == 0 && tetrisGrid[y + 1][x + 3] == 0) {
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
            if(x < tetrisGridWidth - 1 && tetrisGrid[y][x + 1] == 0 && tetrisGrid[y + 1][x] == 0 && tetrisGrid[y + 2][x] == 0) {
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
            if(x < tetrisGridWidth - 1 && tetrisGrid[y][x + 1] == 0 && tetrisGrid[y - 1][x + 1] == 0) {
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
            if(x < tetrisGridWidth - 1 && tetrisGrid[y][x + 1] == 0 && tetrisGrid[y - 1][x] == 0 && tetrisGrid[y - 2][x] == 0) {
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
            if(x < tetrisGridWidth - 3 && tetrisGrid[y][x + 1] == 0 && tetrisGrid[y - 1][x + 3] == 0) {
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
            if(x < tetrisGridWidth - 2 && tetrisGrid[y][x + 2] == 0 && tetrisGrid[y + 1][x + 2] == 0 && tetrisGrid[y + 2][x + 2] == 0) {
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
            if(x < tetrisGridWidth - 1 && tetrisGrid[y][x + 1] == 0 && tetrisGrid[y + 1][x + 1] == 0) {
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
            if(x < tetrisGridWidth - 3 && tetrisGrid[y][x + 2] == 0 && tetrisGrid[y - 1][x + 3] == 0) {
                tetrisGrid[y]    [x + 2] = -1 * tile->color;
                tetrisGrid[y - 1][x + 3] = -1 * tile->color;

                tetrisGrid[y]    [x] = 0;
                tetrisGrid[y - 1][x + 1] = 0;
            
                tile->position.x++;
                return SUCCESS;
            }
            break;
        }
        case S_90:
        case S_270: {
            if(x < tetrisGridWidth - 2 && tetrisGrid[y][x + 1] == 0 && tetrisGrid[y + 1][x + 2] == 0 && tetrisGrid[y + 2][x + 2] == 0) {
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
            if(x < tetrisGridWidth - 2 && tetrisGrid[y][x + 2] == 0 && tetrisGrid[y + 1][x + 2] == 0) {
                tetrisGrid[y]    [x + 2] = -1 * tile->color;
                tetrisGrid[y + 1][x + 2] = -1 * tile->color;
                
                tetrisGrid[y]    [x] = 0;
                tetrisGrid[y + 1][x] = 0;

                tile->position.x++;
                return SUCCESS;
            }
            break;
        }
        case T_0: {
            if(x < tetrisGridWidth - 2 && tetrisGrid[y][x + 1] == 0 && tetrisGrid[y - 1][x + 2] == 0) {
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
            if(x < tetrisGridWidth - 2 && tetrisGrid[y - 1][x + 2] == 0 && tetrisGrid[y][x + 2] == 0 && tetrisGrid[y + 1][x + 2] == 0) {
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
            if(x < tetrisGridWidth - 2 && tetrisGrid[y][x + 1] == 0 && tetrisGrid[y + 1][x + 2] == 0) {
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
            if(x < tetrisGridWidth - 1 && tetrisGrid[y][x + 1] == 0 && tetrisGrid[y - 1][x] == 0 && tetrisGrid[y + 1][x] == 0) {
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
            if(x < tetrisGridWidth - 3 && tetrisGrid[y + 1][x + 3] == 0 && tetrisGrid[y][x + 2] == 0) {
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
            if(x < tetrisGridWidth - 2 && tetrisGrid[y][x + 2] == 0 && tetrisGrid[y - 1][x + 2] == 0 && tetrisGrid[y + 1][x + 1] == 0) {
                tetrisGrid[y]    [x + 2] = -1 * tile->color;
                tetrisGrid[y - 1][x + 2] = -1 * tile->color;
                tetrisGrid[y + 1][x + 1] = -1 * tile->color;

                tetrisGrid[y]    [x] = 0;
                tetrisGrid[y + 1][x] = 0;
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

status_t moveDown(int** tetrisGrid, Tile* tile, const int tetrisGridHeight) {
    if(tile == NULL)return MEMORY_FAILURE;
    int x = tile->position.x;
    int y = tile->position.y;
    if(x < 0 || y < 0)return FAILURE;
    switch(tile->state) {
        case BAR_HORIZONTAL_UP:
        case BAR_HORIZONTAL_DOWN: {
            if(y < tetrisGridHeight - 1 && tetrisGrid[y + 1][x] == 0 && tetrisGrid[y + 1][x + 1] == 0 && tetrisGrid[y + 1][x + 2] == 0 && tetrisGrid[y + 1][x + 3] == 0) {
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
            if(y < tetrisGridHeight - 4 && tetrisGrid[y + 4][x] == 0) {
                tetrisGrid[y + 4][x] = -1 * tile->color;

                tetrisGrid[y][x] = 0;

                tile->position.y++;

                return SUCCESS;                
            }
            break;
        }
        case J_0: {
            if(y < tetrisGridHeight - 1 && tetrisGrid[y + 1][x] == 0 && tetrisGrid[y + 1][x + 1] == 0) {
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
            if(y < tetrisGridHeight - 2 && tetrisGrid[y + 2][x] == 0 && tetrisGrid[y + 2][x + 1] == 0 && tetrisGrid[y + 2][x + 2] == 0) {
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
            if(y < tetrisGridHeight - 3 && tetrisGrid[y + 1][x] == 0 && tetrisGrid[y + 3][x - 1] == 0) {
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
            if(y < tetrisGridHeight - 1 && tetrisGrid[y + 1][x] == 0 && tetrisGrid[y][x - 1] == 0 && tetrisGrid[y][x - 2] == 0) {
                tetrisGrid[y + 1][x]     = -1 * tile->color;
                tetrisGrid[y]    [x - 1] = -1 * tile->color;
                tetrisGrid[y]    [x - 2] = -1 * tile->color;

                tetrisGrid[y - 1][x - 2] = 0;
                tetrisGrid[y - 1][x - 1] = 0;
                tetrisGrid[y - 1][x]     = 0;

                tile->position.y++;

                return SUCCESS;
            }
            break;
        }
        case L_0: {
            if(y < tetrisGridHeight - 1 && tetrisGrid[y + 1][x] == 0 && tetrisGrid[y + 1][x - 1] == 0) {
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
            if(y < tetrisGridHeight - 1 && tetrisGrid[y + 1][x] == 0 && tetrisGrid[y][x + 1] == 0 && tetrisGrid[y][x + 2] == 0) {
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
            if(y < tetrisGridHeight - 3 && tetrisGrid[y + 1][x] == 0 && tetrisGrid[y + 3][x + 1] == 0) {
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
            if(y < tetrisGridHeight - 2 && tetrisGrid[y + 2][x] == 0 && tetrisGrid[y + 2][x - 1] == 0 && tetrisGrid[y + 2][x - 2] == 0) {
                tetrisGrid[y + 2][x]     = -1 * tile->color;
                tetrisGrid[y + 2][x - 1] = -1 * tile->color;
                tetrisGrid[y + 2][x - 2] = -1 * tile->color;

                tetrisGrid[y + 1][x - 2] = 0;
                tetrisGrid[y + 1][x - 1] = 0;
                tetrisGrid[y][x] = 0;

                tile->position.y++;

                return SUCCESS;
            }
            break;
        }
        case S_0:
        case S_180: {
            if(y < tetrisGridHeight - 1 && tetrisGrid[y + 1][x] == 0 && tetrisGrid[y + 1][x + 1] == 0 && tetrisGrid[y][x + 2] == 0) {
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
            if(y < tetrisGridHeight - 3 && tetrisGrid[y + 2][x] == 0 && tetrisGrid[y + 3][x + 1] == 0) {
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
            if(y < tetrisGridHeight - 2 && tetrisGrid[y + 2][x] == 0 && tetrisGrid[y + 2][x + 1] == 0) {
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
            if(y < tetrisGridHeight - 1 && tetrisGrid[y + 1][x] == 0 && tetrisGrid[y][x - 1] == 0 && tetrisGrid[y][x + 1] == 0) {
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
            if(y < tetrisGridHeight - 2 && tetrisGrid[y + 1][x] == 0 && tetrisGrid[y + 2][x + 1] == 0) {
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
            if(y < tetrisGridHeight - 2 && tetrisGrid[y + 2][x - 1] == 0 && tetrisGrid[y + 2][x] == 0 && tetrisGrid[y + 2][x + 1] == 0) {
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
            if(y < tetrisGridHeight - 2 && tetrisGrid[y + 1][x] == 0 && tetrisGrid[y + 2][x - 1] == 0) {
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
            if(y < tetrisGridHeight - 2 && tetrisGrid[y + 1][x] == 0 && tetrisGrid[y + 2][x + 1] == 0 && tetrisGrid[y + 2][x + 2] == 0) {
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
            if(y < tetrisGridHeight - 2 && tetrisGrid[y + 2][x] == 0 && tetrisGrid[y + 1][x + 1] == 0) {
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

//Extremely inefficient, but works for now
void dropHard(int** tetrisGrid, Tile* tile, const Size tetrisGridSize) {
    while(moveDown(tetrisGrid, tile, tetrisGridSize.height) == SUCCESS);

    tile->position.x = -1;
    tile->position.y = -1;

    absMatrix(tetrisGrid, tetrisGridSize);
}