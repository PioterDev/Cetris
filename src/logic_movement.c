#include "deus.h"
#include "logic_shapes.h"
#include "utils.h"

//There is kind of no way to make these functions shorter,
//as every case is a bit different and there are a lot of them.

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

//Inefficient, but works
void dropHard(ProgramParameters* parameters) {
    int distance = 0;
    while(moveDown(parameters->tetrisGrid, parameters->currentTile, parameters->tetrisGridSize.height) == SUCCESS) distance++;

    parameters->currentTile->position.x = -1;
    parameters->currentTile->position.y = -1;

    parameters->score += POINTS_HARDDROP * distance;
}

/**
 * FIXME: rewrite this shit from scratch
 * nah, I'm too lazy
 */
/* status_t dropHardOld(int** tetrisGrid, Tile* tile, const int tetrisGridHeight, FILE* debug) {
    //Convention is as follows:
    //(x, y) point to left-most square of the current tile.
    //If there are multiple, it points to the top-most one.
    //For T, J and L-shape, it points to the tile that "sticks out".
    int x = tile->position.x;
    int y = tile->position.y;
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
} */

//You know what? Since I implemented moveDown and it works better,
//is newer and I'm tired, I'm just gonna replace dropHard with it.
//And now I realize I can't do this because moveDown doesn't
//set appropiate values in the matrix.
//...gonna have to rewrite dropHard from scratch. Eh.
/*
status_t dropHard(char** tetrisGrid, Tile* tile, const int tetrisGridHeight) {
    int x = tile->position.x;
    int y = tile->position.y;
    switch(tile->state) {
        case BAR_HORIZONTAL_UP:
        case BAR_HORIZONTAL_DOWN: {
            if(y < tetrisGridHeight - 1 && tetrisGrid[y + 1][x] == 0 && tetrisGrid[y + 1][x + 1] == 0 && tetrisGrid[y + 1][x + 2] == 0 && tetrisGrid[y + 1][x + 3] == 0) {
                int z[4] = {y + 1, y + 1, y + 1, y + 1};
            
                while(z[0] < tetrisGridHeight - 1 && tetrisGrid[z[0]][x]     == 0)z[0]++;
                while(z[1] < tetrisGridHeight - 1 && tetrisGrid[z[1]][x + 1] == 0)z[1]++;
                while(z[2] < tetrisGridHeight - 1 && tetrisGrid[z[2]][x + 2] == 0)z[2]++;
                while(z[3] < tetrisGridHeight - 1 && tetrisGrid[z[3]][x + 3] == 0)z[3]++;
            
                int min = Min(z, 4);

                tetrisGrid[min][x]     = tile->color;
                tetrisGrid[min][x + 1] = tile->color;
                tetrisGrid[min][x + 2] = tile->color;
                tetrisGrid[min][x + 3] = tile->color;
                
                tetrisGrid[y][x]     = 0;
                tetrisGrid[y][x + 1] = 0;
                tetrisGrid[y][x + 2] = 0;
                tetrisGrid[y][x + 3] = 0;

                goto success;
            }
            break;
        }
        case BAR_VERTICAL_LEFT:
        case BAR_VERTICAL_RIGHT: {
            if(y < tetrisGridHeight - 4 && tetrisGrid[y + 4][x] == 0) {
                int z = y + 4;
                while(z < tetrisGridHeight - 1 && tetrisGrid[z][x] == 0)z++;

                tetrisGrid[z]    [x] = tile->color;
                tetrisGrid[z - 1][x] = tile->color;
                tetrisGrid[z - 2][x] = tile->color;
                tetrisGrid[z - 3][x] = tile->color;

                tetrisGrid[y]    [x] = 0;
                tetrisGrid[y + 1][x] = 0;
                tetrisGrid[y + 2][x] = 0;
                tetrisGrid[y + 3][x] = 0;

                goto success;
            }
            break;
        }
        case J_0: {
            if(y < tetrisGridHeight - 1 && tetrisGrid[y + 1][x] == 0 && tetrisGrid[y + 1][x + 1] == 0) {
                int z[2] = {y + 1, y + 1};

                while(z[0] < tetrisGridHeight - 1 && tetrisGrid[z[0]][x]     == 0)z[0]++;
                while(z[1] < tetrisGridHeight - 1 && tetrisGrid[z[1]][x + 1] == 0)z[1]++;

                int min = z[0] < z[1] ? z[0] : z[1];

                tetrisGrid[min]     [x]     = tile->color;
                tetrisGrid[min]     [x + 1] = tile->color;
                tetrisGrid[min - 1] [x + 1] = tile->color;
                tetrisGrid[min - 2] [x + 1] = tile->color;

                tetrisGrid[y]    [x]     = 0;
                tetrisGrid[y]    [x + 1] = 0;
                tetrisGrid[y - 1][x + 1] = 0;
                tetrisGrid[y - 2][x + 1] = 0;

                goto success;
            }
            break;
        }
        case J_90: {
            if(y < tetrisGridHeight - 2 && tetrisGrid[y + 2][x] == 0 && tetrisGrid[y + 2][x + 1] == 0 && tetrisGrid[y + 2][x + 2] == 0) {
                int z[3] = {y + 2, y + 2, y + 2};

                while(z[0] < tetrisGridHeight - 1 && tetrisGrid[z[0]][x]     == 0)z[0]++;
                while(z[1] < tetrisGridHeight - 1 && tetrisGrid[z[1]][x + 1] == 0)z[1]++;
                while(z[2] < tetrisGridHeight - 1 && tetrisGrid[z[2]][x + 2] == 0)z[1]++;

                int min = Min(z, 3);

                tetrisGrid[min]    [x]     = tile->color;
                tetrisGrid[min]    [x + 1] = tile->color;
                tetrisGrid[min]    [x + 2] = tile->color;
                tetrisGrid[min - 1][x]     = tile->color;

                tetrisGrid[y]    [x]     = 0;
                tetrisGrid[y + 1][x]     = 0;
                tetrisGrid[y + 1][x + 1] = 0;
                tetrisGrid[y + 1][x + 2] = 0;

                goto success;
            }
            break;
        }
        case J_180: {
            if(y < tetrisGridHeight - 3 && tetrisGrid[y + 1][x] == 0 && tetrisGrid[y + 3][x - 1] == 0) {
                int z[2] = {y + 1, y + 3};

                while(z[0] < tetrisGridHeight - 1 && tetrisGrid[z[0]][x]     == 0)z[0]++;
                while(z[1] < tetrisGridHeight - 1 && tetrisGrid[z[1]][x + 1] == 0)z[1]++;

                if(z[0] < z[1]) {
                    tetrisGrid[z[0]][x]
                }

                tetrisGrid[y]    [x]     = 0;
                tetrisGrid[y + 1][x]     = 0;
                tetrisGrid[y + 1][x + 1] = 0;
                tetrisGrid[y + 1][x + 2] = 0;
            }
        }
        default: return FAILURE;
    }
    return FAILURE;

    success:
        tile->position.x = -1;
        tile->position.y = -1;
        return SUCCESS;
} 
*/