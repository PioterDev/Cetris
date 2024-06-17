#include "deus.h"
#include "logic_shapes.h"
#include "utils.h"

//There is kind of no way to make these functions shorter,
//as every case is a bit different and there are a lot of them.

status_t moveLeft(int** grid, Tile* tile) {
    if(tile == NULL)return MEMORY_FAILURE;
    int x = tile->position.x;
    int y = tile->position.y;
    if(x < 0 || y < 0)return FAILURE;
    switch(tile->state) {
        case BAR_HORIZONTAL_UP:
        case BAR_HORIZONTAL_DOWN: {
            if(x > 0 && grid[y][x - 1] == 0) {
                grid[y][x - 1] = -1 * tile->color;

                grid[y][x + 3] = 0;

                tile->position.x--;
                return SUCCESS;
            }
            break;
        }
        case BAR_VERTICAL_LEFT:
        case BAR_VERTICAL_RIGHT: {
            if(x > 0 && grid[y][x - 1] == 0 && grid[y + 1][x - 1] == 0 && grid[y + 2][x - 1] == 0 && grid[y + 3][x - 1] == 0) {
                grid[y]    [x - 1] = -1 * tile->color;
                grid[y + 1][x - 1] = -1 * tile->color;
                grid[y + 2][x - 1] = -1 * tile->color;
                grid[y + 3][x - 1] = -1 * tile->color;

                grid[y]    [x] = 0;
                grid[y + 1][x] = 0;
                grid[y + 2][x] = 0;
                grid[y + 3][x] = 0;
                
                tile->position.x--;
                return SUCCESS;
            }
            break;
        }
        case J_0: {
            if(x > 0 && grid[y - 2][x] == 0 && grid[y - 1][x] == 0 && grid[y][x - 1] == 0) {
                grid[y - 2][x]     = -1 * tile->color;
                grid[y - 1][x]     = -1 * tile->color;
                grid[y]    [x - 1] = -1 * tile->color;

                grid[y - 2][x + 1] = 0;
                grid[y - 1][x + 1] = 0;
                grid[y]    [x + 1] = 0;

                tile->position.x--;
                return SUCCESS;
            }
            break;
        }
        case J_90: {
            if(x > 0 && grid[y][x - 1] == 0 && grid[y + 1][x - 1] == 0) {
                grid[y]    [x - 1] = -1 * tile->color;
                grid[y + 1][x - 1] = -1 * tile->color;
                
                grid[y]    [x]     = 0;
                grid[y + 1][x + 2] = 0;

                tile->position.x--;
                return SUCCESS;
            }
            break;
        }
        case J_180: {
            if(x > 1 && grid[y][x - 2] == 0 && grid[y + 1][x - 2] == 0 && grid[y + 2][x - 2] == 0) {
                grid[y]    [x - 2] = -1 * tile->color;
                grid[y + 1][x - 2] = -1 * tile->color;
                grid[y + 2][x - 2] = -1 * tile->color;

                grid[y]    [x]     = 0;
                grid[y + 1][x - 1] = 0;
                grid[y + 2][x - 1] = 0;
                
                tile->position.x--;
                return SUCCESS;
            }
            break;
        }
        case J_270: {
            if(x > 2 && grid[y][x - 1] == 0 && grid[y - 1][x - 3] == 0) {
                grid[y]    [x - 1] = -1 * tile->color;
                grid[y - 1][x - 3] = -1 * tile->color;

                grid[y]    [x] = 0;
                grid[y - 1][x] = 0;
                
                tile->position.x--;
                return SUCCESS;
            }
            break;
        }
        case L_0: {
            if(x > 1 && grid[y - 2][x - 2] == 0 && grid[y - 1][x - 2] == 0 && grid[y][x - 2] == 0) {
                grid[y - 2][x - 2] = -1 * tile->color;
                grid[y - 1][x - 2] = -1 * tile->color;
                grid[y]    [x - 2] = -1 * tile->color;

                grid[y - 2][x - 1] = 0;
                grid[y - 1][x - 1] = 0;
                grid[y][x] = 0;

                tile->position.x--;
                return SUCCESS;
            }
            break;
        }
        case L_90: {
            if(x > 0 && grid[y][x - 1] == 0 && grid[y - 1][x - 1] == 0) {
                grid[y]    [x - 1] = -1 * tile->color;
                grid[y - 1][x - 1] = -1 * tile->color;
                
                grid[y - 1][x + 2] = 0;
                grid[y]    [x]     = 0;
                
                tile->position.x--;
                return SUCCESS;
            }
            break;
        }
        case L_180: {
            if(x > 0 && grid[y][x - 1] == 0 && grid[y + 1][x] == 0 && grid[y + 2][x] == 0) {
                grid[y]    [x - 1] = -1 * tile->color;
                grid[y + 1][x]     = -1 * tile->color;
                grid[y + 2][x]     = -1 * tile->color;
                
                grid[y]    [x + 1] = 0;
                grid[y + 1][x + 1] = 0;
                grid[y + 2][x + 1] = 0;
            
                tile->position.x--;
                return SUCCESS;
            }
            break;
        }
        case L_270: {
            if(x > 2 && grid[y][x - 1] == 0 && grid[y + 1][x - 3] == 0) {
                grid[y]    [x - 1] = -1 * tile->color;
                grid[y + 1][x - 3] = -1 * tile->color;
                
                grid[y]    [x] = 0;
                grid[y + 1][x] = 0;

                tile->position.x--;
                return SUCCESS;
            }
            break;
        }
        case S_0:
        case S_180: {
            if(x > 0 && grid[y][x - 1] == 0 && grid[y -1][x] == 0) {
                grid[y][x - 1] = -1 * tile->color;
                grid[y - 1][x] = -1 * tile->color;

                grid[y]    [x + 1] = 0;
                grid[y - 1][x + 2] = 0;
            
                tile->position.x--;
                return SUCCESS;
            }
            break;
        }
        case S_90:
        case S_270: {
            if(x > 0 && grid[y][x - 1] == 0 && grid[y + 1][x - 1] == 0 && grid[y + 2][x] == 0) {
                grid[y]    [x - 1] = -1 * tile->color;
                grid[y + 1][x - 1] = -1 * tile->color;
                grid[y + 2][x]     = -1 * tile->color;

                grid[y][x] = 0;
                grid[y + 1][x + 1] = 0;
                grid[y + 2][x + 1] = 0;
                
                tile->position.x--;
                return SUCCESS;
            }
            break;
        }
        case SQR: {
            if(x > 0 && grid[y][x - 1] == 0 && grid[y + 1][x - 1] == 0) {
                grid[y]    [x - 1] = -1 * tile->color;
                grid[y + 1][x - 1] = -1 * tile->color;
                
                grid[y]    [x + 1] = 0;
                grid[y + 1][x + 1] = 0;

                tile->position.x--;
                return SUCCESS;
            }
            break;
        }
        case T_0: {
            if(x > 1 && grid[y - 1][x - 2] == 0 && grid[y][x - 1] == 0) {
                grid[y - 1][x - 2] = -1 * tile->color;
                grid[y]    [x - 1] = -1 * tile->color;

                grid[y]    [x]     = 0;
                grid[y - 1][x + 1] = 0;

                tile->position.x--;
                return SUCCESS;
            }
            break;
        }
        case T_90: {
            if(x > 0 && grid[y][x - 1] == 0 && grid[y - 1][x] == 0 && grid[y + 1][x] == 0) {
                grid[y][x - 1] = -1 * tile->color;
                grid[y - 1][x] = -1 * tile->color;
                grid[y + 1][x] = -1 * tile->color;

                grid[y - 1][x + 1] = 0;
                grid[y]    [x + 1] = 0;
                grid[y + 1][x + 1] = 0;
            
                tile->position.x--;
                return SUCCESS;
            }
            break;
        }
        case T_180: {
            if(x > 1 && grid[y][x - 1] == 0 && grid[y + 1][x - 2] == 0) {
                grid[y]    [x - 1] = -1 * tile->color;
                grid[y + 1][x - 2] = -1 * tile->color;

                grid[y]    [x] = 0;
                grid[y + 1][x + 1] = 0;
                
                tile->position.x--;
                return SUCCESS;           
            }
            break;
        }
        case T_270: {
            if(x > 1 && grid[y - 1][x - 2] == 0 && grid[y][x - 2] == 0 && grid[y + 1][x - 2] == 0) {
                grid[y - 1][x - 2] = -1 * tile->color;
                grid[y]    [x - 2] = -1 * tile->color;
                grid[y + 1][x - 2] = -1 * tile->color;

                grid[y - 1][x - 1] = 0;
                grid[y]    [x]     = 0;
                grid[y + 1][x - 1] = 0;

                tile->position.x--;
                return SUCCESS;
            }
            break;
        }

        case Z_0:
        case Z_180: {
            if(x > 0 && grid[y][x - 1] == 0 && grid[y + 1][x] == 0) {
                grid[y][x - 1] = -1 * tile->color;
                grid[y + 1][x] = -1 * tile->color;

                grid[y]    [x + 1] = 0;
                grid[y + 1][x + 2] = 0;

                tile->position.x--;
                return SUCCESS;
            }
            break;
        }
        case Z_90:
        case Z_270: {
            if(x > 0 && grid[y][x - 1] == 0 && grid[y + 1][x - 1] == 0 && grid[y - 1][x] == 0) {
                grid[y]    [x - 1] = -1 * tile->color;
                grid[y + 1][x - 1] = -1 * tile->color;
                grid[y - 1][x]     = -1 * tile->color;

                grid[y + 1][x] = 0;
                grid[y][x + 1] = 0;
                grid[y - 1][x + 1] = 0;
                
                tile->position.x--;
                return SUCCESS;
            }
            break;
        }
        default: return FAILURE;
    }
    return FAILURE;
}

status_t moveRight(int** grid, Tile* tile, const int gridWidth) {
    if(tile == NULL)return MEMORY_FAILURE;
    int x = tile->position.x;
    int y = tile->position.y;
    if(x < 0 || y < 0)return FAILURE;
    switch(tile->state) {
        case BAR_HORIZONTAL_UP:
        case BAR_HORIZONTAL_DOWN: {
            if(x < gridWidth - 4 && grid[y][x + 4] == 0) {
                grid[y][x + 4] = -1 * tile->color;

                grid[y][x] = 0;

                tile->position.x++;
                return SUCCESS;
            }
            break;
        }
        case BAR_VERTICAL_LEFT:
        case BAR_VERTICAL_RIGHT: {
            if(x < gridWidth - 1 && grid[y][x + 1] == 0 && grid[y + 1][x + 1] == 0 && grid[y + 2][x + 1] == 0 && grid[y + 3][x + 1] == 0) {
                grid[y]    [x + 1] = -1 * tile->color;
                grid[y + 1][x + 1] = -1 * tile->color;
                grid[y + 2][x + 1] = -1 * tile->color;
                grid[y + 3][x + 1] = -1 * tile->color;

                grid[y]    [x] = 0;
                grid[y + 1][x] = 0;
                grid[y + 2][x] = 0;
                grid[y + 3][x] = 0;
                
                tile->position.x++;
                return SUCCESS;
            }
            break;
        }
        case J_0: {
            if(x < gridWidth - 2 && grid[y - 2][x + 2] == 0 && grid[y - 1][x + 2] == 0 && grid[y][x + 2] == 0) {
                grid[y - 2][x + 2] = -1 * tile->color;
                grid[y - 1][x + 2] = -1 * tile->color;
                grid[y]    [x + 2] = -1 * tile->color;

                grid[y - 2][x + 1] = 0;
                grid[y - 1][x + 1] = 0;
                grid[y]    [x]     = 0;

                tile->position.x++;
                return SUCCESS;
            }
            break;
        }
        case J_90: {
            if(x < gridWidth - 3 && grid[y][x + 1] == 0 && grid[y + 1][x + 3] == 0) {
                grid[y]    [x + 1] = -1 * tile->color;
                grid[y + 1][x + 3] = -1 * tile->color;
                
                grid[y]    [x] = 0;
                grid[y + 1][x] = 0;

                tile->position.x++;
                return SUCCESS;
            }
            break;
        }
        case J_180: {
            if(x < gridWidth - 1 && grid[y][x + 1] == 0 && grid[y + 1][x] == 0 && grid[y + 2][x] == 0) {
                grid[y]    [x + 1] = -1 * tile->color;
                grid[y + 1][x]     = -1 * tile->color;
                grid[y + 2][x]     = -1 * tile->color;

                grid[y]    [x - 1] = 0;
                grid[y + 1][x - 1] = 0;
                grid[y + 2][x - 1] = 0;
                
                tile->position.x++;
                return SUCCESS;
            }
            break;
        }
        case J_270: {
            if(x < gridWidth - 1 && grid[y][x + 1] == 0 && grid[y - 1][x + 1] == 0) {
                grid[y]    [x + 1] = -1 * tile->color;
                grid[y - 1][x + 1] = -1 * tile->color;

                grid[y]    [x]     = 0;
                grid[y - 1][x - 2] = 0;
                
                tile->position.x++;
                return SUCCESS;
            }
            break;
        }
        case L_0: {
            if(x < gridWidth - 1 && grid[y][x + 1] == 0 && grid[y - 1][x] == 0 && grid[y - 2][x] == 0) {
                grid[y][x + 1] = -1 * tile->color;
                grid[y - 1][x] = -1 * tile->color;
                grid[y - 2][x] = -1 * tile->color;

                grid[y - 2][x - 1] = 0;
                grid[y - 1][x - 1] = 0;
                grid[y]    [x - 1] = 0;

                tile->position.x++;
                return SUCCESS;
            }
            break;
        }
        case L_90: {
            if(x < gridWidth - 3 && grid[y][x + 1] == 0 && grid[y - 1][x + 3] == 0) {
                grid[y]    [x + 1] = -1 * tile->color;
                grid[y - 1][x + 3] = -1 * tile->color;
                
                grid[y - 1][x] = 0;
                grid[y]    [x] = 0;
                
                tile->position.x++;
                return SUCCESS;
            }
            break;
        }
        case L_180: {
            if(x < gridWidth - 2 && grid[y][x + 2] == 0 && grid[y + 1][x + 2] == 0 && grid[y + 2][x + 2] == 0) {
                grid[y]    [x + 2] = -1 * tile->color;
                grid[y + 1][x + 2] = -1 * tile->color;
                grid[y + 2][x + 2] = -1 * tile->color;
                
                grid[y]    [x]     = 0;
                grid[y + 1][x + 1] = 0;
                grid[y + 2][x + 1] = 0;
            
                tile->position.x++;
                return SUCCESS;
            }
            break;
        }
        case L_270: {
            if(x < gridWidth - 1 && grid[y][x + 1] == 0 && grid[y + 1][x + 1] == 0) {
                grid[y]    [x + 1] = -1 * tile->color;
                grid[y + 1][x + 1] = -1 * tile->color;
                
                grid[y]    [x]     = 0;
                grid[y + 1][x - 2] = 0;

                tile->position.x++;
                return SUCCESS;
            }
            break;
        }
        case S_0:
        case S_180: {
            if(x < gridWidth - 3 && grid[y][x + 2] == 0 && grid[y - 1][x + 3] == 0) {
                grid[y]    [x + 2] = -1 * tile->color;
                grid[y - 1][x + 3] = -1 * tile->color;

                grid[y]    [x] = 0;
                grid[y - 1][x + 1] = 0;
            
                tile->position.x++;
                return SUCCESS;
            }
            break;
        }
        case S_90:
        case S_270: {
            if(x < gridWidth - 2 && grid[y][x + 1] == 0 && grid[y + 1][x + 2] == 0 && grid[y + 2][x + 2] == 0) {
                grid[y]    [x + 1] = -1 * tile->color;
                grid[y + 1][x + 2] = -1 * tile->color;
                grid[y + 2][x + 2]     = -1 * tile->color;

                grid[y][x] = 0;
                grid[y + 1][x] = 0;
                grid[y + 2][x + 1] = 0;
                
                tile->position.x++;
                return SUCCESS;
            }
            break;
        }
        case SQR: {
            if(x < gridWidth - 2 && grid[y][x + 2] == 0 && grid[y + 1][x + 2] == 0) {
                grid[y]    [x + 2] = -1 * tile->color;
                grid[y + 1][x + 2] = -1 * tile->color;
                
                grid[y]    [x] = 0;
                grid[y + 1][x] = 0;

                tile->position.x++;
                return SUCCESS;
            }
            break;
        }
        case T_0: {
            if(x < gridWidth - 2 && grid[y][x + 1] == 0 && grid[y - 1][x + 2] == 0) {
                grid[y][x + 1] = -1 * tile->color;
                grid[y - 1][x + 2] = -1 * tile->color;

                grid[y]    [x]     = 0;
                grid[y - 1][x - 1] = 0;

                tile->position.x++;
                return SUCCESS;
            }
            break;
        }
        case T_90: {
            if(x < gridWidth - 2 && grid[y - 1][x + 2] == 0 && grid[y][x + 2] == 0 && grid[y + 1][x + 2] == 0) {
                grid[y - 1][x + 2] = -1 * tile->color;
                grid[y]    [x + 2] = -1 * tile->color;
                grid[y + 1][x + 2] = -1 * tile->color;

                grid[y]    [x] = 0;
                grid[y - 1][x + 1] = 0;
                grid[y + 1][x + 1] = 0;
            
                tile->position.x++;
                return SUCCESS;
            }
            break;
        }
        case T_180: {
            if(x < gridWidth - 2 && grid[y][x + 1] == 0 && grid[y + 1][x + 2] == 0) {
                grid[y]    [x + 1] = -1 * tile->color;
                grid[y + 1][x + 2] = -1 * tile->color;

                grid[y]    [x] = 0;
                grid[y + 1][x - 1] = 0;
                
                tile->position.x++;
                return SUCCESS;           
            }
            break;
        }
        case T_270: {
            if(x < gridWidth - 1 && grid[y][x + 1] == 0 && grid[y - 1][x] == 0 && grid[y + 1][x] == 0) {
                grid[y]    [x + 1] = -1 * tile->color;
                grid[y - 1][x]     = -1 * tile->color;
                grid[y + 1][x]     = -1 * tile->color;

                grid[y - 1][x - 1] = 0;
                grid[y]    [x - 1] = 0;
                grid[y + 1][x - 1] = 0;

                tile->position.x++;
                return SUCCESS;
            }
            break;
        }

        case Z_0:
        case Z_180: {
            if(x < gridWidth - 3 && grid[y + 1][x + 3] == 0 && grid[y][x + 2] == 0) {
                grid[y + 1][x + 3] = -1 * tile->color;
                grid[y]    [x + 2] = -1 * tile->color;

                grid[y]    [x]     = 0;
                grid[y + 1][x + 1] = 0;

                tile->position.x++;
                return SUCCESS;
            }
            break;
        }
        case Z_90:
        case Z_270: {
            if(x < gridWidth - 2 && grid[y][x + 2] == 0 && grid[y - 1][x + 2] == 0 && grid[y + 1][x + 1] == 0) {
                grid[y]    [x + 2] = -1 * tile->color;
                grid[y - 1][x + 2] = -1 * tile->color;
                grid[y + 1][x + 1] = -1 * tile->color;

                grid[y]    [x] = 0;
                grid[y + 1][x] = 0;
                grid[y - 1][x + 1] = 0;
                
                tile->position.x++;
                return SUCCESS;
            }
            break;
        }
        default: return FAILURE;
    }
    return FAILURE;
}

status_t moveDown(int** grid, Tile* tile, const int gridHeight) {
    if(tile == NULL)return MEMORY_FAILURE;
    int x = tile->position.x;
    int y = tile->position.y;
    if(x < 0 || y < 0)return FAILURE;
    switch(tile->state) {
        case BAR_HORIZONTAL_UP:
        case BAR_HORIZONTAL_DOWN: {
            if(y < gridHeight - 1 && grid[y + 1][x] == 0 && grid[y + 1][x + 1] == 0 && grid[y + 1][x + 2] == 0 && grid[y + 1][x + 3] == 0) {
                grid[y + 1][x]     = -1 * tile->color;
                grid[y + 1][x + 1] = -1 * tile->color;
                grid[y + 1][x + 2] = -1 * tile->color;
                grid[y + 1][x + 3] = -1 * tile->color;

                grid[y][x] = 0;
                grid[y][x + 1] = 0;
                grid[y][x + 2] = 0;
                grid[y][x + 3] = 0;

                tile->position.y++;
                return SUCCESS;
            }
            break;
        }
        case BAR_VERTICAL_LEFT:
        case BAR_VERTICAL_RIGHT: {
            if(y < gridHeight - 4 && grid[y + 4][x] == 0) {
                grid[y + 4][x] = -1 * tile->color;

                grid[y][x] = 0;

                tile->position.y++;

                return SUCCESS;                
            }
            break;
        }
        case J_0: {
            if(y < gridHeight - 1 && grid[y + 1][x] == 0 && grid[y + 1][x + 1] == 0) {
                grid[y + 1][x]     = -1 * tile->color;
                grid[y + 1][x + 1] = -1 * tile->color;

                grid[y][x]         = 0;
                grid[y - 2][x + 1] = 0;

                tile->position.y++;

                return SUCCESS;
            }
            break;
        }
        case J_90: {
            if(y < gridHeight - 2 && grid[y + 2][x] == 0 && grid[y + 2][x + 1] == 0 && grid[y + 2][x + 2] == 0) {
                grid[y + 2][x]     = -1 * tile->color;
                grid[y + 2][x + 1] = -1 * tile->color;
                grid[y + 2][x + 2] = -1 * tile->color;

                grid[y][x]         = 0;
                grid[y + 1][x + 1] = 0;
                grid[y + 1][x + 2] = 0;

                tile->position.y++;

                return SUCCESS;
            }
            break;
        }
        case J_180: {
            if(y < gridHeight - 3 && grid[y + 1][x] == 0 && grid[y + 3][x - 1] == 0) {
                grid[y + 1][x]     = -1 * tile->color;
                grid[y + 3][x - 1] = -1 * tile->color;

                grid[y][x]     = 0;
                grid[y][x - 1] = 0;

                tile->position.y++;

                return SUCCESS;
            }
            break;
        }
        case J_270: {
            if(y < gridHeight - 1 && grid[y + 1][x] == 0 && grid[y][x - 1] == 0 && grid[y][x - 2] == 0) {
                grid[y + 1][x]     = -1 * tile->color;
                grid[y]    [x - 1] = -1 * tile->color;
                grid[y]    [x - 2] = -1 * tile->color;

                grid[y - 1][x - 2] = 0;
                grid[y - 1][x - 1] = 0;
                grid[y - 1][x]     = 0;

                tile->position.y++;

                return SUCCESS;
            }
            break;
        }
        case L_0: {
            if(y < gridHeight - 1 && grid[y + 1][x] == 0 && grid[y + 1][x - 1] == 0) {
                grid[y + 1][x]     = -1 * tile->color;
                grid[y + 1][x - 1] = -1 * tile->color;

                grid[y][x]         = 0;
                grid[y - 2][x - 1] = 0;

                tile->position.y++;

                return SUCCESS;
            }
            break;
        }
        case L_90: {
            if(y < gridHeight - 1 && grid[y + 1][x] == 0 && grid[y][x + 1] == 0 && grid[y][x + 2] == 0) {
                grid[y + 1][x]     = -1 * tile->color;
                grid[y]    [x + 1] = -1 * tile->color;
                grid[y]    [x + 2] = -1 * tile->color;

                grid[y - 1][x] = 0;
                grid[y - 1][x + 1] = 0;
                grid[y - 1][x + 2] = 0;

                tile->position.y++;

                return SUCCESS;
            }
            break;
        }
        case L_180: {
            if(y < gridHeight - 3 && grid[y + 1][x] == 0 && grid[y + 3][x + 1] == 0) {
                grid[y + 1][x]     = -1 * tile->color;
                grid[y + 3][x + 1] = -1 * tile->color;

                grid[y][x]     = 0;
                grid[y][x + 1] = 0;

                tile->position.y++;

                return SUCCESS;
            }
            break;
        }
        case L_270: {
            if(y < gridHeight - 2 && grid[y + 2][x] == 0 && grid[y + 2][x - 1] == 0 && grid[y + 2][x - 2] == 0) {
                grid[y + 2][x]     = -1 * tile->color;
                grid[y + 2][x - 1] = -1 * tile->color;
                grid[y + 2][x - 2] = -1 * tile->color;

                grid[y + 1][x - 2] = 0;
                grid[y + 1][x - 1] = 0;
                grid[y][x] = 0;

                tile->position.y++;

                return SUCCESS;
            }
            break;
        }
        case S_0:
        case S_180: {
            if(y < gridHeight - 1 && grid[y + 1][x] == 0 && grid[y + 1][x + 1] == 0 && grid[y][x + 2] == 0) {
                grid[y + 1][x]     = -1 * tile->color;
                grid[y + 1][x + 1] = -1 * tile->color;
                grid[y]    [x + 2] = -1 * tile->color;

                grid[y][x] = 0;
                grid[y - 1][x + 1] = 0;
                grid[y - 1][x + 2] = 0;

                tile->position.y++;

                return SUCCESS;
            }
            break;
        }
        case S_90:
        case S_270: {
            if(y < gridHeight - 3 && grid[y + 2][x] == 0 && grid[y + 3][x + 1] == 0) {
                grid[y + 2][x]     = -1 * tile->color;
                grid[y + 3][x + 1] = -1 * tile->color;

                grid[y]    [x]     = 0;
                grid[y + 1][x + 1] = 0;
            
                tile->position.y++;

                return SUCCESS;
            }
            break;
        }
        case SQR: {
            if(y < gridHeight - 2 && grid[y + 2][x] == 0 && grid[y + 2][x + 1] == 0) {
                grid[y + 2][x]     = -1 * tile->color;
                grid[y + 2][x + 1] = -1 * tile->color;

                grid[y][x]     = 0;
                grid[y][x + 1] = 0;
            
                tile->position.y++;

                return SUCCESS;
            }
            break;
        }
        case T_0: {
            if(y < gridHeight - 1 && grid[y + 1][x] == 0 && grid[y][x - 1] == 0 && grid[y][x + 1] == 0) {
                grid[y + 1][x]     = -1 * tile->color;
                grid[y]    [x - 1] = -1 * tile->color;
                grid[y]    [x + 1] = -1 * tile->color;

                grid[y - 1][x - 1] = 0;
                grid[y - 1][x]     = 0;
                grid[y - 1][x + 1] = 0;
            
                tile->position.y++;

                return SUCCESS;
            }
            break;
        }
        case T_90: {
            if(y < gridHeight - 2 && grid[y + 1][x] == 0 && grid[y + 2][x + 1] == 0) {
                grid[y + 1][x]     = -1 * tile->color;
                grid[y + 2][x + 1] = -1 * tile->color;

                grid[y]    [x]     = 0;
                grid[y - 1][x + 1] = 0;
            
                tile->position.y++;

                return SUCCESS;
            }
            break;
        }
        case T_180: {
            if(y < gridHeight - 2 && grid[y + 2][x - 1] == 0 && grid[y + 2][x] == 0 && grid[y + 2][x + 1] == 0) {
                grid[y + 2][x - 1] = -1 * tile->color;
                grid[y + 2][x]     = -1 * tile->color;
                grid[y + 2][x + 1] = -1 * tile->color;

                grid[y + 1][x - 1] = 0;
                grid[y]    [x]     = 0;
                grid[y + 1][x + 1] = 0;
            
                tile->position.y++;

                return SUCCESS;
            }
            break;
        }
        case T_270: {
            if(y < gridHeight - 2 && grid[y + 1][x] == 0 && grid[y + 2][x - 1] == 0) {
                grid[y + 1][x]     = -1 * tile->color;
                grid[y + 2][x - 1] = -1 * tile->color;

                grid[y]    [x]     = 0;
                grid[y - 1][x - 1] = 0;
            
                tile->position.y++;

                return SUCCESS;
            }
            break;
        }
        case Z_0:
        case Z_180: {
            if(y < gridHeight - 2 && grid[y + 1][x] == 0 && grid[y + 2][x + 1] == 0 && grid[y + 2][x + 2] == 0) {
                grid[y + 1][x]     = -1 * tile->color;
                grid[y + 2][x + 1] = -1 * tile->color;
                grid[y + 2][x + 2] = -1 * tile->color;

                grid[y]    [x]     = 0;
                grid[y]    [x + 1] = 0;
                grid[y + 1][x + 2] = 0;
            
                tile->position.y++;

                return SUCCESS;
            }
            break;
        }
        case Z_90:
        case Z_270: {
            if(y < gridHeight - 2 && grid[y + 2][x] == 0 && grid[y + 1][x + 1] == 0) {
                grid[y + 2][x]     = -1 * tile->color;
                grid[y + 1][x + 1] = -1 * tile->color;

                grid[y]    [x]     = 0;
                grid[y - 1][x + 1] = 0;

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
    while(moveDown(parameters->grid, parameters->currentTile, parameters->gridSize.height) == SUCCESS) distance++;

    parameters->currentTile->position.x = -1;
    parameters->currentTile->position.y = -1;

    parameters->score += POINTS_HARDDROP * distance;
}

/**
 * FIXME: rewrite this shit from scratch
 * nah, I'm too lazy
 */
/* status_t dropHardOld(int** grid, Tile* tile, const int gridHeight, FILE* debug) {
    //Convention is as follows:
    //(x, y) point to left-most square of the current tile.
    //If there are multiple, it points to the top-most one.
    //For T, J and L-shape, it points to the tile that "sticks out".
    int x = tile->position.x;
    int y = tile->position.y;
    switch(tile->shape) {
        case BAR: {
            if(grid[y][x + 1] == -1 * tile->color) {
                //[][][][]
                int z[4] = {y + 1, y + 1, y + 1, y + 1};
 
                while(z[0] < gridHeight && grid[z[0]][x]     == 0)z[0]++;
                while(z[1] < gridHeight && grid[z[1]][x + 1] == 0)z[1]++;
                while(z[2] < gridHeight && grid[z[2]][x + 2] == 0)z[2]++;
                while(z[3] < gridHeight && grid[z[3]][x + 3] == 0)z[3]++;

                int min = Min(z, 4);
                min--;

                grid[min][x]     = tile->color;
                grid[min][x + 1] = tile->color;
                grid[min][x + 2] = tile->color;
                grid[min][x + 3] = tile->color;
                
                grid[y][x]     = 0;
                grid[y][x + 1] = 0;
                grid[y][x + 2] = 0;
                grid[y][x + 3] = 0;
            }
            else if(grid[y + 1][x] == -1 * tile->color) {
                //[]
                //[]
                //[]
                //[]
                int z = y + 4;
                while(z < gridHeight && grid[z][x] == 0)z++;
                z--;

                grid[z]    [x] = tile->color;
                grid[z + 1][x] = tile->color;
                grid[z + 2][x] = tile->color;
                grid[z + 3][x] = tile->color;

                grid[y]    [x] = 0;
                grid[y + 1][x] = 0;
                grid[y + 2][x] = 0;
                grid[y + 3][x] = 0;
            }
            break;
        }
        case J: {
            if(grid[y][x + 1] == -1 * tile->color) {
                //  []
                //  []
                //[][]
                int z[2] = {y + 1, y + 1};

                while(z[0] < gridHeight && grid[z[0]][x]     == 0)z[0]++;
                while(z[1] < gridHeight && grid[z[1]][x + 1] == 0)z[1]++;

                int min = z[0] < z[1] ? z[0] : z[1];
                min--;

                grid[min]     [x]     = tile->color;
                grid[min]     [x + 1] = tile->color;
                grid[min - 1] [x + 1] = tile->color;
                grid[min - 2] [x + 1] = tile->color;

                grid[y]    [x]     = 0;
                grid[y]    [x + 1] = 0;
                grid[y - 1][x + 1] = 0;
                grid[y - 2][x + 1] = 0;
            }
            else if(grid[y][x - 1] == -1 * tile->color) {
                //[][]
                //[]
                //[]
                int z[2] = {y + 1, y + 3};
        
                while(z[0] < gridHeight && grid[z[0]][x]     == 0)z[0]++;
                while(z[1] < gridHeight && grid[z[1]][x - 1] == 0)z[1]++;

                if(z[0] < z[1]) {
                    z[0]--;

                    grid[z[0]]    [x]     = tile->color;
                    grid[z[0]]    [x - 1] = tile->color;
                    grid[z[0] + 1][x - 1] = tile->color;
                    grid[z[0] + 2][x - 1] = tile->color;
                }
                else if(z[1] < z[0]) {
                    z[1]--;

                    grid[z[1]]    [x - 1] = tile->color;
                    grid[z[1] - 1][x - 1] = tile->color;
                    grid[z[1] - 2][x - 1] = tile->color;
                    grid[z[1] - 2][x]     = tile->color;
                }

                grid[y]    [x]     = 0;
                grid[y]    [x - 1] = 0;
                grid[y - 1][x - 1] = 0;
                grid[y - 2][x - 1] = 0;
            }
            else if(grid[y + 1][x] == -1 * tile->color) {
                //[]
                //[][][]
                int z[3] = {y + 2, y + 2, y + 2};

                while(z[0] < gridHeight && grid[z[0]][x]     == 0)z[0]++;
                while(z[1] < gridHeight && grid[z[1]][x + 1] == 0)z[1]++;
                while(z[2] < gridHeight && grid[z[2]][x + 2] == 0)z[2]++;

                int min = Min(z, 3);
                min--;

                grid[min - 1] [x] = tile->color;
                grid[min]     [x] = tile->color;
                grid[min]     [x] = tile->color;
                grid[min]     [x] = tile->color;

                grid[y]    [x]     = 0;
                grid[y + 1][x]     = 0;
                grid[y + 1][x + 1] = 0;
                grid[y + 1][x + 2] = 0;
            }
            else if(grid[y - 1][x] == -1 * tile->color) {
                //[][][]
                //    []
                int z[3] = {y + 1, y + 1, y + 1};

                while(z[0] < gridHeight && grid[z[0]][x]     == 0)z[0]++;
                while(z[1] < gridHeight && grid[z[1]][x - 1] == 0)z[1]++;
                while(z[2] < gridHeight && grid[z[2]][x - 2] == 0)z[2]++;
                
                int minIndex = MinIndex(z, 3);
                if(minIndex == 0) {
                    z[0]--;

                    grid[z[0]]    [x]     = tile->color;
                    grid[z[0] - 1][x]     = tile->color;
                    grid[z[0] - 1][x - 1] = tile->color;
                    grid[z[0] - 1][x - 2] = tile->color;
                }
                else {
                    z[minIndex]--;

                    grid[z[minIndex] + 1][x]     = tile->color;
                    grid[z[minIndex]]    [x]     = tile->color;
                    grid[z[minIndex]]    [x - 1] = tile->color;
                    grid[z[minIndex]]    [x - 2] = tile->color;
                }

                grid[y]    [x]     = 0;
                grid[y - 1][x]     = 0;
                grid[y - 1][x - 1] = 0;
                grid[y - 1][x - 2] = 0;
            }
            break;
        }
        case L: {
            if(grid[y][x - 1] == -1 * tile->color) {
                //[]
                //[]
                //[][]
                int z[2] = {y + 1, y + 1};

                while(z[0] < gridHeight && grid[z[0]][x]     == 0)z[0]++;
                while(z[1] < gridHeight && grid[z[1]][x - 1] == 0)z[1]++;

                int min = z[0] < z[1] ? z[0] : z[1];
                min--;

                grid[min]     [x]     = tile->color;
                grid[min]     [x - 1] = tile->color;
                grid[min - 1] [x - 1] = tile->color;
                grid[min - 2] [x - 1] = tile->color;

                grid[y]    [x]     = 0;
                grid[y]    [x - 1] = 0;
                grid[y - 1][x - 1] = 0;
                grid[y - 2][x - 1] = 0;
            }
            else if(grid[y][x + 1] == -1 * tile->color) {
                //[][]
                //  []
                //  []
                int z[2] = {y + 1, y + 3};

                while(z[0] < gridHeight && grid[z[0]][x]     == 0)z[0]++;
                while(z[1] < gridHeight && grid[z[1]][x + 1] == 0)z[1]++;

                if(z[0] < z[1]) {
                    z[0]--;

                    grid[z[0]]    [x]     = tile->color;
                    grid[z[0]]    [x + 1] = tile->color;
                    grid[z[0] + 1][x + 1] = tile->color;
                    grid[z[0] + 2][x + 1] = tile->color;
                }
                else if(z[1] < z[0]) {
                    z[1]--;

                    grid[z[1]]    [x + 1] = tile->color;
                    grid[z[1] - 1][x + 1] = tile->color;
                    grid[z[1] - 2][x + 1] = tile->color;
                    grid[z[1] - 2][x]     = tile->color;
                }

                grid[y]    [x]     = 0;
                grid[y]    [x + 1] = 0;
                grid[y + 1][x + 1] = 0;
                grid[y + 2][x + 1] = 0;
            }
            else if(grid[y - 1][x] == -1 * tile->color) {
                //[][][]
                //[]
                int z[3] = {y + 1, y, y};

                while(z[0] < gridHeight && grid[z[0]][x]     == 0)z[0]++;
                while(z[1] < gridHeight && grid[z[1]][x + 1] == 0)z[1]++;
                while(z[2] < gridHeight && grid[z[2]][x + 2] == 0)z[2]++;

                int minIndex = MinIndex(z, 3);
                if(minIndex == 0) {
                    z[0]--;

                    grid[z[0]]    [x]     = tile->color;
                    grid[z[0] - 1][x]     = tile->color;
                    grid[z[0] - 1][x + 1] = tile->color;
                    grid[z[0] - 1][x + 2] = tile->color;
                }
                else {
                    z[minIndex]--;

                    grid[z[minIndex] + 1] [x]     = tile->color;
                    grid[z[minIndex]]     [x]     = tile->color;
                    grid[z[minIndex]]     [x + 1] = tile->color;
                    grid[z[minIndex]]     [x + 2] = tile->color;
                }

                grid[y]    [x]     = 0;
                grid[y - 1][x]     = 0;
                grid[y - 1][x + 1] = 0;
                grid[y - 1][x + 2] = 0;
            }
            else if(grid[y + 1][x] == -1 * tile->color) {
                //    []
                //[][][]
                int z[3] = {y + 2, y + 2, y + 2};

                while(z[0] < gridHeight && grid[z[0]][x]     == 0)z[0]++;
                while(z[1] < gridHeight && grid[z[1]][x - 1] == 0)z[1]++;
                while(z[2] < gridHeight && grid[z[2]][x - 2] == 0)z[2]++;

                int min = Min(z, 3);
                min--;

                grid[min - 1][x]     = tile->color;
                grid[min]    [x]     = tile->color;
                grid[min]    [x - 1] = tile->color;
                grid[min]    [x - 2] = tile->color;

                grid[y]    [x]     = 0;
                grid[y + 1][x]     = 0;
                grid[y + 1][x - 1] = 0;
                grid[y + 1][x - 2] = 0;
            }
            break;
        }
        case S: {
            if(grid[y][x + 1] == -1 * tile->color) {
                //  [][]
                //[][]
                int z[3] = {y + 1, y + 1, y};

                while(z[0] < gridHeight && grid[z[0]][x]     == 0)z[0]++;
                while(z[1] < gridHeight && grid[z[1]][x + 1] == 0)z[1]++;
                while(z[2] < gridHeight && grid[z[2]][x + 2] == 0)z[2]++;

                int min = Min(z, 3);
                if(min == z[2]) {
                    min--;

                    grid[min]    [x + 2] = tile->color; //upper right one
                    grid[min]    [x + 1] = tile->color;
                    grid[min + 1][x + 1] = tile->color;
                    grid[min + 1][x]     = tile->color;
                }
                else {
                    min--;

                    grid[min]    [x]     = tile->color;
                    grid[min]    [x + 1] = tile->color;
                    grid[min - 1][x + 1] = tile->color;
                    grid[min - 1][x + 2] = tile->color;
                }

                grid[y]    [x]     = 0;
                grid[y]    [x + 1] = 0;
                grid[y - 1][x + 1] = 0;
                grid[y - 1][x + 2] = 0;
            }
            else if(grid[y + 1][x] == -1 * tile->color) {
                //[]
                //[][]
                //  []
                int z[2] = {y + 3, y + 3};

                while(z[0] < gridHeight && grid[z[0]][x]     == 0)z[0]++;
                while(z[1] < gridHeight && grid[z[1]][x + 1] == 0)z[1]++;

                if(z[0] < z[1]) {
                    z[0]--;

                    grid[z[0] + 1][x + 1] = tile->color; //bottom one
                    grid[z[0]]    [x + 1] = tile->color;
                    grid[z[0]]    [x]     = tile->color;
                    grid[z[0] - 1][x]     = tile->color;
                }
                else if(z[1] < z[0]) {
                    z[1]--;

                    grid[z[1]]    [x + 1] = tile->color;
                    grid[z[1] - 1][x + 1] = tile->color;
                    grid[z[1] - 1][x]     = tile->color;
                    grid[z[1] - 2][x]     = tile->color;
                }

                grid[y]    [x]     = 0;
                grid[y + 1][x]     = 0;
                grid[y + 1][x + 1] = 0;
                grid[y + 2][x + 1] = 0;
            }
            break;
        }
        case SQUARE: {
            //[][]
            //[][]
            int z[2] = {y + 2, y + 2};

            while(z[0] < gridHeight && grid[z[0]][x]     == 0)z[0]++;
            while(z[1] < gridHeight && grid[z[1]][x + 1] == 0)z[1]++;

            int min = z[0] < z[1] ? z[0] : z[1];
            min--;

            grid[min]    [x]      = tile->color;
            grid[min]    [x + 1]  = tile->color;
            grid[min - 1][x]      = tile->color;
            grid[min - 1][x + 1]  = tile->color;

            grid[y]    [x]     = 0;
            grid[y]    [x + 1] = 0;
            grid[y + 1][x]     = 0;
            grid[y + 1][x + 1] = 0;
            break;
        }
        case T: {
            if(grid[y][x - 1] == -1 * tile->color) {
                //[]
                //[][]
                //[]
                int z[2] = {y + 2, y + 1};

                while(z[0] < gridHeight && grid[z[0]][x - 1] == 0)z[0]++;
                while(z[1] < gridHeight && grid[z[1]][x]     == 0)z[1]++;

                if(z[0] < z[1]) {
                    z[0]--;
                    grid[z[0]]    [x - 1] = tile->color;
                    grid[z[0] - 1][x - 1] = tile->color;
                    grid[z[0] - 1][x]     = tile->color;
                    grid[z[0] - 2][x - 1] = tile->color;
                }
                else if(z[1] < z[0]) {
                    z[1]--;

                    grid[z[1] + 1][x - 1] = tile->color;
                    grid[z[1]]    [x - 1] = tile->color;
                    grid[z[1]]    [x]     = tile->color;
                    grid[z[1] - 1][x - 1] = tile->color;
                }
                
                grid[y + 1][x - 1] = 0;
                grid[y]    [x - 1] = 0;
                grid[y]    [x]     = 0;
                grid[y - 1][x - 1] = 0;
            }
            else if(grid[y][x + 1] == -1 * tile->color) {
                //  []
                //[][]
                //  []
                int z[2] = {y + 2, y + 1};

                while(z[0] < gridHeight && grid[z[0]][x + 1] == 0)z[0]++;
                while(z[1] < gridHeight && grid[z[1]][x]     == 0)z[1]++;
                
                if(z[0] < z[1]) {
                    z[0]--;

                    grid[z[0]]    [x + 1] = tile->color;
                    grid[z[0] - 1][x + 1] = tile->color;
                    grid[z[0] - 1][x]     = tile->color;
                    grid[z[0] - 2][x + 1] = tile->color;
                }
                else if(z[1] < z[0]) {
                    z[1]--;

                    grid[z[1] + 1][x + 1] = tile->color;
                    grid[z[1]]    [x + 1] = tile->color;
                    grid[z[1]]    [x]     = tile->color;
                    grid[z[1] - 1][x + 1] = tile->color;
                }
                
                grid[y + 1][x + 1] = 0;
                grid[y]    [x + 1] = 0;
                grid[y]    [x]     = 0;
                grid[y - 1][x + 1] = 0;
            }
            else if(grid[y - 1][x] == -1 * tile->color) {
                //[][][]
                //  []
                int z[3] = {y, y + 1, y};

                while(z[0] < gridHeight && grid[z[0]][x - 1] == 0)z[0]++;
                while(z[1] < gridHeight && grid[z[1]][x]     == 0)z[1]++;
                while(z[2] < gridHeight && grid[z[2]][x + 1] == 0)z[2]++;

                int min = Min(z, 3);
                min--;

                grid[min]    [x]     = tile->color;
                grid[min - 1][x - 1] = tile->color;
                grid[min - 1][x]     = tile->color;
                grid[min - 1][x + 1] = tile->color;

                grid[y]    [x]     = 0;
                grid[y - 1][x - 1] = 0;
                grid[y - 1][x]     = 0;
                grid[y - 1][x + 1] = 0;
            }
            else if(grid[y + 1][x] == -1 * tile->color) {
                //  []
                //[][][]
                int z[3] = {y + 2, y + 2, y + 2};

                while(z[0] < gridHeight && grid[z[0]][x - 1] == 0)z[0]++;
                while(z[1] < gridHeight && grid[z[1]][x]     == 0)z[1]++;
                while(z[2] < gridHeight && grid[z[2]][x + 1] == 0)z[2]++;

                int min = Min(z, 3);
                min--;

                grid[min - 1][x]     = tile->color;
                grid[min]    [x]     = tile->color;
                grid[min]    [x - 1] = tile->color;
                grid[min]    [x + 1] = tile->color;

                grid[y]    [x]     = 0;
                grid[y + 1][x - 1] = 0;
                grid[y + 1][x]     = 0;
                grid[y + 1][x + 1] = 0;
            }
            break;
        }
        case Z: {
            if(grid[y][x + 1] == -1 * tile->color) {
                //[][]
                //  [][]
                int z[3] = {y + 1, y + 1, y + 1};
                
                while(z[0] < gridHeight && grid[z[0]][x]     == 0)z[0]++;
                while(z[1] < gridHeight && grid[z[1]][x + 1] == 0)z[1]++;
                while(z[2] < gridHeight && grid[z[2]][x + 2] == 0)z[2]++;

                int min = Min(z, 3);
                if(min == z[0]) {
                    min--;

                    grid[min]    [x]     = tile->color; //upper left one
                    grid[min]    [x + 1] = tile->color;
                    grid[min + 1][x + 1] = tile->color;
                    grid[min + 1][x + 2] = tile->color;
                }
                else {
                    min--;

                    grid[min]    [x + 2] = tile->color; //bottom right one
                    grid[min]    [x + 1] = tile->color;
                    grid[min - 1][x + 1] = tile->color;
                    grid[min - 1][x]     = tile->color;
                }

                grid[y]    [x]     = 0;
                grid[y]    [x + 1] = 0;
                grid[y + 1][x + 1] = 0;
                grid[y + 1][x + 2] = 0;
            }
            else if(grid[y + 1][x] == -1 * tile->color) {
                //  []
                //[][]
                //[]
                int z[2] = {y + 2, y + 1};

                while(z[0] < gridHeight && grid[z[0]][x]     == 0)z[0]++;
                while(z[1] < gridHeight && grid[z[1]][x + 1] == 0)z[1]++;

                if(z[0] < z[1]) {
                    z[0]--;

                    grid[z[0]]    [x]     = tile->color; //bottom one
                    grid[z[0] - 1][x]     = tile->color;
                    grid[z[0] - 1][x + 1] = tile->color;
                    grid[z[0] - 2][x + 1] = tile->color;
                }
                else if(z[1] < z[0]) {
                    z[1]--;

                    grid[z[1]]    [x]     = tile->color;
                    grid[z[1]]    [x + 1] = tile->color;
                    grid[z[1] - 1][x + 1] = tile->color;
                    grid[z[1] + 1][x]     = tile->color;
                }

                grid[y]    [x]     = 0;
                grid[y + 1][x]     = 0;
                grid[y]    [x + 1] = 0;
                grid[y - 1][x + 1] = 0;
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
status_t dropHard(char** grid, Tile* tile, const int gridHeight) {
    int x = tile->position.x;
    int y = tile->position.y;
    switch(tile->state) {
        case BAR_HORIZONTAL_UP:
        case BAR_HORIZONTAL_DOWN: {
            if(y < gridHeight - 1 && grid[y + 1][x] == 0 && grid[y + 1][x + 1] == 0 && grid[y + 1][x + 2] == 0 && grid[y + 1][x + 3] == 0) {
                int z[4] = {y + 1, y + 1, y + 1, y + 1};
            
                while(z[0] < gridHeight - 1 && grid[z[0]][x]     == 0)z[0]++;
                while(z[1] < gridHeight - 1 && grid[z[1]][x + 1] == 0)z[1]++;
                while(z[2] < gridHeight - 1 && grid[z[2]][x + 2] == 0)z[2]++;
                while(z[3] < gridHeight - 1 && grid[z[3]][x + 3] == 0)z[3]++;
            
                int min = Min(z, 4);

                grid[min][x]     = tile->color;
                grid[min][x + 1] = tile->color;
                grid[min][x + 2] = tile->color;
                grid[min][x + 3] = tile->color;
                
                grid[y][x]     = 0;
                grid[y][x + 1] = 0;
                grid[y][x + 2] = 0;
                grid[y][x + 3] = 0;

                goto success;
            }
            break;
        }
        case BAR_VERTICAL_LEFT:
        case BAR_VERTICAL_RIGHT: {
            if(y < gridHeight - 4 && grid[y + 4][x] == 0) {
                int z = y + 4;
                while(z < gridHeight - 1 && grid[z][x] == 0)z++;

                grid[z]    [x] = tile->color;
                grid[z - 1][x] = tile->color;
                grid[z - 2][x] = tile->color;
                grid[z - 3][x] = tile->color;

                grid[y]    [x] = 0;
                grid[y + 1][x] = 0;
                grid[y + 2][x] = 0;
                grid[y + 3][x] = 0;

                goto success;
            }
            break;
        }
        case J_0: {
            if(y < gridHeight - 1 && grid[y + 1][x] == 0 && grid[y + 1][x + 1] == 0) {
                int z[2] = {y + 1, y + 1};

                while(z[0] < gridHeight - 1 && grid[z[0]][x]     == 0)z[0]++;
                while(z[1] < gridHeight - 1 && grid[z[1]][x + 1] == 0)z[1]++;

                int min = z[0] < z[1] ? z[0] : z[1];

                grid[min]     [x]     = tile->color;
                grid[min]     [x + 1] = tile->color;
                grid[min - 1] [x + 1] = tile->color;
                grid[min - 2] [x + 1] = tile->color;

                grid[y]    [x]     = 0;
                grid[y]    [x + 1] = 0;
                grid[y - 1][x + 1] = 0;
                grid[y - 2][x + 1] = 0;

                goto success;
            }
            break;
        }
        case J_90: {
            if(y < gridHeight - 2 && grid[y + 2][x] == 0 && grid[y + 2][x + 1] == 0 && grid[y + 2][x + 2] == 0) {
                int z[3] = {y + 2, y + 2, y + 2};

                while(z[0] < gridHeight - 1 && grid[z[0]][x]     == 0)z[0]++;
                while(z[1] < gridHeight - 1 && grid[z[1]][x + 1] == 0)z[1]++;
                while(z[2] < gridHeight - 1 && grid[z[2]][x + 2] == 0)z[1]++;

                int min = Min(z, 3);

                grid[min]    [x]     = tile->color;
                grid[min]    [x + 1] = tile->color;
                grid[min]    [x + 2] = tile->color;
                grid[min - 1][x]     = tile->color;

                grid[y]    [x]     = 0;
                grid[y + 1][x]     = 0;
                grid[y + 1][x + 1] = 0;
                grid[y + 1][x + 2] = 0;

                goto success;
            }
            break;
        }
        case J_180: {
            if(y < gridHeight - 3 && grid[y + 1][x] == 0 && grid[y + 3][x - 1] == 0) {
                int z[2] = {y + 1, y + 3};

                while(z[0] < gridHeight - 1 && grid[z[0]][x]     == 0)z[0]++;
                while(z[1] < gridHeight - 1 && grid[z[1]][x + 1] == 0)z[1]++;

                if(z[0] < z[1]) {
                    grid[z[0]][x]
                }

                grid[y]    [x]     = 0;
                grid[y + 1][x]     = 0;
                grid[y + 1][x + 1] = 0;
                grid[y + 1][x + 2] = 0;
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