#include "deus.h"

status_t getPositions(TileState state, const Point position, Point* positions) {
    if(position.x < 0 || position.y < 0) return FAILURE; //to prevent going off bounds
    switch(state) {
        case BAR_HORIZONTAL_UP:
        case BAR_HORIZONTAL_DOWN: {
            for(int i = 0; i < 4; i++) {
                positions[i].x = position.x + i;
                positions[i].y = position.y;
            }
            return SUCCESS;
        }
        case BAR_VERTICAL_LEFT:
        case BAR_VERTICAL_RIGHT: {
            for(int i = 0; i < 4; i++) {
                positions[i].x = position.x;
                positions[i].y = position.y + i;
            }
            return SUCCESS;
        }
        case J_0: {
            positions[0] = position;
            for(int i = 1; i < 4; i++) {
                positions[i].x = position.x + 1;
                positions[i].y = position.y + 1 - i;
            }
            return SUCCESS;
        }
        case J_90: {
            positions[0] = position;
            for(int i = 1; i < 4; i++) {
                positions[i].x = position.x + i - 1;
                positions[i].y = position.y + 1;
            }
            return SUCCESS;
        }
        case J_180: {
            positions[0] = position;
            for(int i = 1; i < 4; i++) {
                positions[i].x = position.x - 1;
                positions[i].y = position.y - 1 + i;
            }
            return SUCCESS;    
        }
        case J_270: {
            positions[0] = position;
            for(int i = 1; i < 4; i++) {
                positions[i].x = position.x + 1 - i;
                positions[i].y = position.y - 1;
            }
            return SUCCESS;
        }
        case L_0: {
            positions[0] = position;
            for(int i = 1; i < 4; i++) {
                positions[i].x = position.x - 1;
                positions[i].y = position.y + 1 - i;
            }
            return SUCCESS;
        }
        case L_90: {
            positions[0] = position;
            for(int i = 1; i < 4; i++) {
                positions[i].x = position.x + 1 - i;
                positions[i].y = position.y - 1;
            }
            return SUCCESS;
        }
        case L_180: {
            positions[0] = position;
            for(int i = 1; i < 4; i++) {
                positions[i].x = position.x + 1;
                positions[i].y = position.y -1 + i;
            }
            return SUCCESS;
        }
        case L_270: {
            positions[0] = position;
            for(int i = 1; i < 4; i++) {
                positions[i].x = position.x + 1 - i;
                positions[i].y = position.y + 1;
            }
            return SUCCESS;
        }
        case SQR: {
            for(int i = 0; i < 2; i++) {
                positions[i].x = position.x;
                positions[i].y = position.y + i;
                positions[i + 2].x = position.x + 1;
                positions[i + 2].y = position.y + i;
            }
            return SUCCESS;
        }
        case S_0:
        case S_180: {
            for(int i = 0; i < 2; i++) {
                positions[i].x = position.x + i;
                positions[i].y = position.y;
                positions[i + 2].x = position.x + i + 1;
                positions[i + 2].y = position.y - 1;
            }
            return SUCCESS;
        }
        case S_90:
        case S_270: {
            for(int i = 0; i < 2; i++) {
                positions[i].x = position.x;
                positions[i].y = position.y + i;
                positions[i + 2].x = position.x + 1;
                positions[i + 2].y = position.y + 1 + i;
            }
            return SUCCESS;
        }
        case T_0: {
            positions[0] = position;
            for(int i = 1; i < 4; i++) {
                positions[i].x = position.x - 2 + i;
                positions[i].y = position.y - 1;
            }
            return SUCCESS;
        }
        case T_90: {
            positions[0] = position;
            for(int i = 1; i < 4; i++) {
                positions[i].x = position.x + 1;
                positions[i].y = position.y - 2 + i;
            }
            return SUCCESS;
        }
        case T_180: {
            positions[0] = position;
            for(int i = 1; i < 4; i++) {
                positions[i].x = position.x - 2 + i;
                positions[i].y = position.y + 1;
            }
            return SUCCESS;
        }
        case T_270: {
            positions[0] = position;
            for(int i = 1; i < 4; i++) {
                positions[i].x = position.x - 1;
                positions[i].y = position.y - 2 + i;
            }
            return SUCCESS;
        }
        case Z_0:
        case Z_180: {
            for(int i = 0; i < 2; i++) {
                positions[i].x = position.x + i;
                positions[i].y = position.y;

                positions[i + 2].x = position.x + i + 1;
                positions[i + 2].y = position.y + 1;
            }
            return SUCCESS;
        }
        case Z_90:
        case Z_270: {
            for(int i = 0; i < 2; i++) {
                positions[i].x = position.x;
                positions[i].y = position.y + i;

                positions[i + 2].x = position.x + 1;
                positions[i + 2].y = position.y - 1 + i;
            }
            return SUCCESS;
        }
        default: return FAILURE;
    }
    return FAILURE;
}

status_t checkPositions(int** tetrisGrid, const Point* positions, const int n) {
    for(int i = 0; i < n; i++) {
        int cell = tetrisGrid[positions[i].y][positions[i].x];
        if(cell != 0 && cell != GHOST) return FAILURE;
    }
    return SUCCESS;
}

void setPositions(int** tetrisGrid, const Point* positions, const int n, const int value) {
    for(int i = 0; i < n; i++) {
        tetrisGrid[positions[i].y][positions[i].x] = value;
    }
}

int getOccupiedAmount(TileState state) {
    switch(state) {
        case BAR_HORIZONTAL_DOWN:
        case BAR_HORIZONTAL_UP:
        case BAR_VERTICAL_LEFT:
        case BAR_VERTICAL_RIGHT:
        case J_0:
        case J_90:
        case J_180:
        case J_270:
        case L_0:
        case L_90:
        case L_180:
        case L_270:
        case S_0:
        case S_90:
        case S_180:
        case S_270:
        case SQR:
        case T_0:
        case T_90:
        case T_180:
        case T_270:
        case Z_0:
        case Z_90:
        case Z_180:
        case Z_270:
            return 4;
        default: return -1;
    }
}