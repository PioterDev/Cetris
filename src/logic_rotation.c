#include "structs_unions_defines.h"
#include "utils.h"

status_t rotateClockwise(int** tetrisGrid, Tile* tile) {
    if(tile == NULL)return MEMORY_FAILURE;
    int x = tile->position.x;
    int y = tile->position.y;
    switch(tile->state) {
        case BAR_HORIZONTAL_UP: {
            if(y > 0 && tetrisGrid[y - 1][x + 2] == 0 && tetrisGrid[y + 1][x + 2] == 0 && tetrisGrid[y + 2][x + 2] == 0) {
                tetrisGrid[y - 1][x + 2] = -1 * tile->color;
                tetrisGrid[y + 1][x + 2] = -1 * tile->color;
                tetrisGrid[y + 2][x + 2] = -1 * tile->color;

                tetrisGrid[y][x]     = 0;
                tetrisGrid[y][x + 1] = 0;
                tetrisGrid[y][x + 3] = 0;

                tile->state = BAR_VERTICAL_RIGHT;
                
                tile->position.x += 2;
                tile->position.y -= 1;

                return SUCCESS;
            }
            break;
        }
        case BAR_HORIZONTAL_DOWN: {
            if(y > 1 && tetrisGrid[y - 2][x + 1] == 0 && tetrisGrid[y - 1][x + 1] == 0 && tetrisGrid[y + 1][x + 1] == 0) {
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
            if(x > 0 && tetrisGrid[y + 1][x - 1] == 0 && tetrisGrid[y + 1][x + 1] == 0 && tetrisGrid[y + 1][x + 2] == 0) {
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
            if(x > 1 && tetrisGrid[y + 2][x - 2] == 0 && tetrisGrid[y + 2][x - 1] == 0 && tetrisGrid[y + 2][x + 1] == 0) {
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
            if(x < GridWidth - 2 && tetrisGrid[y - 2][x] == 0 && tetrisGrid[y - 1][x] == 0 && tetrisGrid[y - 1][x + 2] == 0) {
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
            if(y < GridHeight - 2 && tetrisGrid[y][x + 1] == 0 && tetrisGrid[y][x + 2] == 0 && tetrisGrid[y + 2][x + 1] == 0) {
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
            if(y > 1 && tetrisGrid[y - 1][x] == 0 && tetrisGrid[y - 2][x] == 0) {
                tetrisGrid[y - 1][x] = -1 * tile->color;
                tetrisGrid[y - 2][x] = -1 * tile->color;

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
                tile->position.y += 1;

                return SUCCESS;
            }
            break;
        }
        case Z_90: {
            if(x > 0 && tetrisGrid[y][x - 1] == 0 && tetrisGrid[y + 1][x + 1] == 0) {
                tetrisGrid[y]    [x - 1] = -1 * tile->color;
                tetrisGrid[y + 1][x + 1] = -1 * tile->color;

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

status_t rotateCounterClockwise(int** tetrisGrid, Tile* tile) {
    if(tile == NULL)return MEMORY_FAILURE;
    int x = tile->position.x;
    int y = tile->position.y;
    switch(tile->state) {
        case BAR_HORIZONTAL_UP: {
            if(y > 0 && tetrisGrid[y - 1][x + 1] == 0 && tetrisGrid[y + 1][x + 1] == 0 && tetrisGrid[y + 2][x + 1] == 0) {
                tetrisGrid[y - 1][x + 1] = -1 * tile->color;
                tetrisGrid[y + 1][x + 1] = -1 * tile->color;
                tetrisGrid[y + 2][x + 1] = -1 * tile->color;

                tetrisGrid[y][x]     = 0;
                tetrisGrid[y][x + 2] = 0;
                tetrisGrid[y][x + 3] = 0;

                tile->state = BAR_VERTICAL_LEFT;
                
                tile->position.x += 1;
                tile->position.y -= 1;

                return SUCCESS;
            }
            break;
        }
        case BAR_HORIZONTAL_DOWN: {
            if(y > 1 && tetrisGrid[y - 2][x + 2] == 0 && tetrisGrid[y - 1][x + 2] == 0 && tetrisGrid[y + 1][x + 2] == 0) {
                tetrisGrid[y - 2][x + 2] = -1 * tile->color;
                tetrisGrid[y - 1][x + 2] = -1 * tile->color;
                tetrisGrid[y + 1][x + 2] = -1 * tile->color;

                tetrisGrid[y][x]     = 0;
                tetrisGrid[y][x + 1] = 0;
                tetrisGrid[y][x + 3] = 0;

                tile->state = BAR_VERTICAL_RIGHT;
                
                tile->position.x += 2;
                tile->position.y -= 2;

                return SUCCESS;
            }
            break;
        }
        case BAR_VERTICAL_LEFT: {
            if(x > 0 && tetrisGrid[y + 2][x - 1] == 0 && tetrisGrid[y + 2][x + 1] == 0 && tetrisGrid[y + 2][x + 2] == 0) {
                tetrisGrid[y + 2][x - 1] = -1 * tile->color;
                tetrisGrid[y + 2][x + 1] = -1 * tile->color;
                tetrisGrid[y + 2][x + 2] = -1 * tile->color;

                tetrisGrid[y]    [x] = 0;
                tetrisGrid[y + 1][x] = 0;
                tetrisGrid[y + 3][x] = 0;

                tile->state = BAR_HORIZONTAL_DOWN;

                tile->position.x -= 1;
                tile->position.y += 2;

                return SUCCESS;
            }
            break;
        }
        case BAR_VERTICAL_RIGHT: {
            if(x > 1 && tetrisGrid[y + 1][x - 2] == 0 && tetrisGrid[y + 1][x + 1] == 0 && tetrisGrid[y + 1][x - 1] == 0) {
                tetrisGrid[y + 1][x - 2] = -1 * tile->color;
                tetrisGrid[y + 1][x - 1] = -1 * tile->color;
                tetrisGrid[y + 1][x + 1] = -1 * tile->color;

                tetrisGrid[y]    [x] = 0;
                tetrisGrid[y + 2][x] = 0;
                tetrisGrid[y + 3][x] = 0;

                tile->state = BAR_HORIZONTAL_UP;

                tile->position.x -= 2;
                tile->position.y += 1;

                return SUCCESS;
            }
            break;
        }
        case J_0: {
            if(x < GridWidth - 2 && tetrisGrid[y][x + 2] == 0 && tetrisGrid[y - 1][x] == 0 && tetrisGrid[y - 1][x + 2] == 0) {
                tetrisGrid[y]    [x + 2] = -1 * tile->color;
                tetrisGrid[y - 1][x]     = -1 * tile->color;
                tetrisGrid[y - 1][x + 2] = -1 * tile->color;

                tetrisGrid[y]    [x]     = 0;
                tetrisGrid[y]    [x + 1] = 0;
                tetrisGrid[y - 2][x + 1] = 0;

                tile->state = J_270;

                tile->position.x += 2;

                return SUCCESS;
            }
            break;
        }
        case J_90: {
            if(y < GridHeight - 2 && tetrisGrid[y + 2][x] == 0 && tetrisGrid[y + 2][x + 1] == 0 && tetrisGrid[y][x + 1] == 0) {
                tetrisGrid[y + 2][x]     = -1 * tile->color;
                tetrisGrid[y + 2][x + 1] = -1 * tile->color;
                tetrisGrid[y]    [x + 1] = -1 * tile->color;

                tetrisGrid[y]    [x]     = 0;
                tetrisGrid[y + 1][x]     = 0;
                tetrisGrid[y + 1][x + 2] = 0;

                tile->state = J_0;

                tile->position.y += 2;

                return SUCCESS;
            }
            break;
        }
        case J_180: {
            if(x > 1 && tetrisGrid[y][x - 2] == 0 && tetrisGrid[y + 1][x - 2] == 0 && tetrisGrid[y + 1][x] == 0) {
                tetrisGrid[y]    [x - 2] = -1 * tile->color;
                tetrisGrid[y + 1][x - 2] = -1 * tile->color;
                tetrisGrid[y + 1][x]     = -1 * tile->color;

                tetrisGrid[y]    [x]     = 0;
                tetrisGrid[y]    [x - 1] = 0;
                tetrisGrid[y + 2][x - 1] = 0;

                tile->state = J_90;

                tile->position.x -= 2;

                return SUCCESS;
            }
            break;
        }
        case J_270: {
            if(y > 1 && tetrisGrid[y - 2][x] == 0 && tetrisGrid[y - 2][x - 1] == 0 && tetrisGrid[y][x - 1] == 0) {
                tetrisGrid[y - 2][x]     = -1 * tile->color;
                tetrisGrid[y - 2][x - 1] = -1 * tile->color;
                tetrisGrid[y]    [x - 1] = -1 * tile->color;

                tetrisGrid[y]    [x]     = 0;
                tetrisGrid[y - 1][x]     = 0;
                tetrisGrid[y - 1][x - 2] = 0;

                tile->state = J_180;

                tile->position.y -= 2;

                return SUCCESS;
            }
            break;
        }
        case L_0: {
            if(x > 1 && tetrisGrid[y - 2][x] == 0 && tetrisGrid[y - 1][x] == 0 && tetrisGrid[y - 1][x - 2] == 0) {
                tetrisGrid[y - 2][x]     = -1 * tile->color;
                tetrisGrid[y - 1][x]     = -1 * tile->color;
                tetrisGrid[y - 1][x - 2] = -1 * tile->color;

                tetrisGrid[y]    [x]     = 0;
                tetrisGrid[y]    [x - 1] = 0;
                tetrisGrid[y - 2][x - 1] = 0;

                tile->state = L_270;

                tile->position.y -= 2;

                return SUCCESS;
            }
            break;
        }
        case L_90: {
            if(y > 1 && tetrisGrid[y][x + 1] == 0 && tetrisGrid[y][x + 2] == 0 && tetrisGrid[y - 2][x + 1] == 0) {
                tetrisGrid[y]    [x + 1] = -1 * tile->color;
                tetrisGrid[y]    [x + 2] = -1 * tile->color;
                tetrisGrid[y - 2][x + 1] = -1 * tile->color;

                tetrisGrid[y]    [x]     = 0;
                tetrisGrid[y - 1][x]     = 0;
                tetrisGrid[y - 1][x + 2] = 0;

                tile->state = L_0;

                tile->position.x += 2;

                return SUCCESS;
            }
            break;
        }
        case L_180: {
            if(x < GridWidth - 2 && tetrisGrid[y + 1][x] == 0 && tetrisGrid[y + 2][x] == 0 && tetrisGrid[y + 1][x + 2] == 0) {
                tetrisGrid[y + 1][x]     = -1 * tile->color;
                tetrisGrid[y + 2][x]     = -1 * tile->color;
                tetrisGrid[y + 1][x + 2] = -1 * tile->color;

                tetrisGrid[y]    [x]     = 0;
                tetrisGrid[y]    [x + 1] = 0;
                tetrisGrid[y + 2][x + 1] = 0;

                tile->state = L_90;

                tile->position.y += 2;

                return SUCCESS;
            }
            break;
        }
        case L_270: {
            if(y < GridHeight - 2 && tetrisGrid[y][x - 1] == 0 && tetrisGrid[y][x - 2] == 0 && tetrisGrid[y + 2][x - 1] == 0) {
                tetrisGrid[y]    [x - 1] = -1 * tile->color;
                tetrisGrid[y]    [x - 2] = -1 * tile->color;
                tetrisGrid[y + 2][x - 1] = -1 * tile->color;

                tetrisGrid[y]    [x]     = 0;
                tetrisGrid[y + 1][x]     = 0;
                tetrisGrid[y + 1][x - 2] = 0;

                tile->state = L_180;

                tile->position.x -= 2;

                return SUCCESS;
            }
            break;
        }
        case S_0: {
            if(y < GridHeight - 2 && tetrisGrid[y - 1][x] == 0 && tetrisGrid[y + 1][x + 1] == 0) {
                tetrisGrid[y - 1][x]     = -1 * tile->color;
                tetrisGrid[y + 1][x + 1] = -1 * tile->color;

                tetrisGrid[y - 1][x + 1]     = 0;
                tetrisGrid[y - 1][x + 2] = 0;

                tile->state = S_270;

                tile->position.y -= 1;

                return SUCCESS;
            }
            break;
        }
        case S_90: {
            if(x > 0 && tetrisGrid[y + 1][x - 1] == 0 && tetrisGrid[y][x - 1] == 0) {
                tetrisGrid[y + 1][x - 1] = -1 * tile->color;
                tetrisGrid[y]    [x + 1] = -1 * tile->color;

                tetrisGrid[y + 1][x + 1] = 0;
                tetrisGrid[y + 2][x + 1] = 0;

                tile->state = S_0;

                tile->position.x -= 1;
                tile->position.y += 1;

                return SUCCESS;
            }
            break;
        }
        case S_180: {
            if(y > 1 && tetrisGrid[y - 2][x + 1] == 0 && tetrisGrid[y][x + 2] == 0) {
                tetrisGrid[y - 2][x + 1] = -1 * tile->color;
                tetrisGrid[y]    [x + 2] = -1 * tile->color;

                tetrisGrid[y][x]     = 0;
                tetrisGrid[y][x + 1] = 0;

                tile->state = S_90;

                tile->position.x += 1;
                tile->position.y -= 2;

                return SUCCESS;
            }
            break;
        }
        case S_270: {
            if(x < GridWidth - 2 && tetrisGrid[y + 2][x] == 0 && tetrisGrid[y + 1][x + 2] == 0) {
                tetrisGrid[y + 2][x]     = -1 * tile->color;
                tetrisGrid[y + 1][x + 2] = -1 * tile->color;

                tetrisGrid[y]    [x]     = 0;
                tetrisGrid[y + 1][x]     = 0;

                tile->state = S_180;

                tile->position.y += 2;

                return SUCCESS;
            }
            break;
        }
        case SQR: return SUCCESS;
        case T_0: {
            if(y > 1 && tetrisGrid[y - 2][x] == 0) {
                tetrisGrid[y - 2][x] = -1 * tile->color;

                tetrisGrid[y - 1][x - 1] = 0;

                tile->state = T_270;

                tile->position.x += 1;
                tile->position.y -= 1;

                return SUCCESS;
            }
            break;
        }
        case T_90: {
            if(x < GridWidth - 2 && tetrisGrid[y][x + 2] == 0) {
                tetrisGrid[y][x + 2] = -1 * tile->color;

                tetrisGrid[y - 1][x + 1] = 0;

                tile->state = T_0;

                tile->position.x += 1;
                tile->position.y += 1;

                return SUCCESS;
            }
            break;
        }
        case T_180: {
            if(y < GridHeight - 2 && tetrisGrid[y + 2][x] == 0) {
                tetrisGrid[y + 2][x] = -1 * tile->color;

                tetrisGrid[y + 1][x + 1] = 0;

                tile->state = T_90;

                tile->position.x -= 1;
                tile->position.y += 1;

                return SUCCESS;
            }
            break;
        }
        case T_270: {
            if(x > 1 && tetrisGrid[y][x - 2] == 0) {
                tetrisGrid[y][x - 2] = -1 * tile->color;

                tetrisGrid[y + 1][x - 1] = 0;

                tile->state = T_180;

                tile->position.x -= 1;
                tile->position.y -= 1;

                return SUCCESS;
            }
            break;
        }
        case Z_0: {
            if(y < GridHeight - 2 && tetrisGrid[y + 1][x] == 0 && tetrisGrid[y + 2][x] == 0) {
                tetrisGrid[y + 1][x] = -1 * tile->color;
                tetrisGrid[y + 2][x] = -1 * tile->color;

                tetrisGrid[y]    [x]     = 0;
                tetrisGrid[y + 1][x + 2] = 0;

                tile->state = Z_270;

                tile->position.y += 1;

                return SUCCESS;
            }
            break;
        }
        case Z_90: {
            if(x > 0 && tetrisGrid[y - 1][x] == 0 && tetrisGrid[y - 1][x - 1] == 0) {
                tetrisGrid[y - 1][x]     = -1 * tile->color;
                tetrisGrid[y - 1][x - 1] = -1 * tile->color;

                tetrisGrid[y - 1][x + 1] = 0;
                tetrisGrid[y + 1][x]     = 0;

                tile->state = Z_0;

                tile->position.x -= 1;
                tile->position.y -= 1;

                return SUCCESS;
            }
            break;
        }
        case Z_180: {
            if(y > 0 && tetrisGrid[y][x + 2] == 0 && tetrisGrid[y - 1][x + 2] == 0) {
                tetrisGrid[y]    [x + 2] = -1 * tile->color;
                tetrisGrid[y - 1][x + 2] = -1 * tile->color;

                tetrisGrid[y]    [x]     = 0;
                tetrisGrid[y + 1][x + 2] = 0;

                tile->state = Z_90;

                tile->position.x += 1;

                return SUCCESS;
            }
            break;
        }
        case Z_270: {
            if(x < GridWidth - 2 && tetrisGrid[y + 1][x + 1] == 0 && tetrisGrid[y + 1][x + 2] == 0) {
                tetrisGrid[y + 1][x + 1] = -1 * tile->color;
                tetrisGrid[y + 1][x + 2] = -1 * tile->color;

                tetrisGrid[y - 1][x + 1] = 0;
                tetrisGrid[y + 1][x]     = 0;

                tile->state = Z_180;

                //position stays the same

                return SUCCESS;
            }
            break;
        }
        default: return FAILURE;
    }
    return FAILURE;
}