#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <SDL.h>

#include "config.h"
#include "structs_unions_defines.h"
#include "utils.h"

Tile* loadTile(SDL_Renderer* renderer, TileColor color, TileShape shape, Point* coordinates, FILE* debug) {
    Tile* tile = malloc(sizeof(Tile));
    if(tile == NULL)return NULL;

    char path[64] = {0};
    strcat(path, tilesPath);
    if(debug != NULL)fprintf(debug, "%s\n", path);
    SDL_Texture* texture = NULL;
    switch(color) {
        case AQUA: {
            switch(shape) {
                case BAR: {
                    strcat(path, "tile_bar_aqua.png");
                    break;
                }
                case J: {
                    strcat(path, "tile_j_aqua.png");
                    break;
                }
                case L: {
                    strcat(path, "tile_l_aqua.png");
                    break;
                }
                case S: {
                    strcat(path, "tile_s_aqua.png");
                    break;
                }
                case SQUARE: {
                    strcat(path, "tile_square_aqua.png");
                    break;
                }
                case T: {
                    strcat(path, "tile_t_aqua.png");
                    break;
                }
                case Z: {
                    strcat(path, "tile_z_aqua.png");
                    break;
                }
                default: goto failure;
            }
            break;
        }
        case BLUE: {
            switch(shape) {
                case BAR: {
                    strcat(path, "tile_bar_blue.png");
                    break;
                }
                case J: {
                    strcat(path, "tile_j_blue.png");
                    break;
                }
                case L: {
                    strcat(path, "tile_l_blue.png");
                    break;
                }
                case S: {
                    strcat(path, "tile_s_blue.png");
                    break;
                }
                case SQUARE: {
                    strcat(path, "tile_square_blue.png");
                    break;
                }
                case T: {
                    strcat(path, "tile_t_blue.png");
                    break;
                }
                case Z: {
                    strcat(path, "tile_z_blue.png");
                    break;
                }
                default: goto failure;
            }
            break;
        }
        case GREEN: {
            switch(shape) {
                case BAR: {
                    strcat(path, "tile_bar_green.png");
                    break;
                }
                case J: {
                    strcat(path, "tile_j_green.png");
                    break;
                }
                case L: {
                    strcat(path, "tile_l_green.png");
                    break;
                }
                case S: {
                    strcat(path, "tile_s_green.png");
                    break;
                }
                case SQUARE: {
                    strcat(path, "tile_square_green.png");
                    break;
                }
                case T: {
                    strcat(path, "tile_t_green.png");
                    break;
                }
                case Z: {
                    strcat(path, "tile_z_green.png");
                    break;
                }
                default: goto failure;
            }
            break;
        }
        case MAGENTA: {
            switch(shape) {
                case BAR: {
                    strcat(path, "tile_bar_magenta.png");
                    break;
                }
                case J: {
                    strcat(path, "tile_j_magenta.png");
                    break;
                }
                case L: {
                    strcat(path, "tile_l_magenta.png");
                    break;
                }
                case S: {
                    strcat(path, "tile_s_magenta.png");
                    break;
                }
                case SQUARE: {
                    strcat(path, "tile_square_magenta.png");
                    break;
                }
                case T: {
                    strcat(path, "tile_t_magenta.png");
                    break;
                }
                case Z: {
                    strcat(path, "tile_z_magenta.png");
                    break;
                }
                default: goto failure;
            }
            break;
        }
        case ORANGE: {
            switch(shape) {
                case BAR: {
                    strcat(path, "tile_bar_orange.png");
                    break;
                }
                case J: {
                    strcat(path, "tile_j_orange.png");
                    break;
                }
                case L: {
                    strcat(path, "tile_l_orange.png");
                    break;
                }
                case S: {
                    strcat(path, "tile_s_orange.png");
                    break;
                }
                case SQUARE: {
                    strcat(path, "tile_square_orange.png");
                    break;
                }
                case T: {
                    strcat(path, "tile_t_orange.png");
                    break;
                }
                case Z: {
                    strcat(path, "tile_z_orange.png");
                    break;
                }
                default: goto failure;
            }
            break;
        }
        case RED: {
            switch(shape) {
                case BAR: {
                    strcat(path, "tile_bar_red.png");
                    break;
                }
                case J: {
                    strcat(path, "tile_j_red.png");
                    break;
                }
                case L: {
                    strcat(path, "tile_l_red.png");
                    break;
                }
                case S: {
                    strcat(path, "tile_s_red.png");
                    break;
                }
                case SQUARE: {
                    strcat(path, "tile_square_red.png");
                    break;
                }
                case T: {
                    strcat(path, "tile_t_red.png");
                    break;
                }
                case Z: {
                    strcat(path, "tile_z_red.png");
                    break;
                }
                default: goto failure;
            }
            break;
        }
        case YELLOW: {
            switch(shape) {
                case BAR: {
                    strcat(path, "tile_bar_yellow.png");
                    break;
                }
                case J: {
                    strcat(path, "tile_j_yellow.png");
                    break;
                }
                case L: {
                    strcat(path, "tile_l_yellow.png");
                    break;
                }
                case S: {
                    strcat(path, "tile_s_yellow.png");
                    break;
                }
                case SQUARE: {
                    strcat(path, "tile_square_yellow.png");
                    break;
                }
                case T: {
                    strcat(path, "tile_t_yellow.png");
                    break;
                }
                case Z: {
                    strcat(path, "tile_z_yellow.png");
                    break;
                }
                default: goto failure;
            }
            break;
        }
        case COLOR_UNKNOWN: {
            switch(shape) {
                case BACKGROUND: {
                    strcat(path, "tile_background.png");
                    break;
                }
                default: goto failure;
            }
            break;
        }
        default: goto failure;
    }
    if(debug != NULL)fprintf(debug, "%s\n", path);
    texture = loadTextureRect(path, renderer, &tile->rect);
    if(texture == NULL) goto failure;

    switch(shape) {
        case BAR:
            tile->center.x = tile->rect.w / 2;
            tile->center.y = tile->rect.h;
            tile->position.y = 0;
            tile->position.x = GridWidth / 2 - 2;
            tile->state = BAR_HORIZONTAL_UP;
            break;
        case J:
            tile->center.x = tile->rect.w / 4 * 3;
            tile->center.y = tile->rect.h / 2;
            tile->position.y = 2;
            tile->position.x = GridWidth / 2 - 1;
            tile->state = J_0;
            break;
        case L:
            tile->center.x = tile->rect.w / 4;
            tile->center.y = tile->rect.h / 2;
            tile->position.y = 2;
            tile->position.x = GridWidth / 2 - 1;
            tile->state = L_0;
            break;
        case S:
            tile->center.x = tile->rect.w / 2;
            tile->center.y = tile->rect.h / 4 * 3;
            tile->position.y = 1;
            tile->position.x = GridWidth / 2 - 1;
            tile->state = S_0;
            break;
        case SQUARE:
            tile->center.x = tile->rect.w / 2;
            tile->center.y = tile->rect.h / 2;
            tile->position.y = 0;
            tile->position.x = GridWidth / 2 - 1;
            tile->state = SQR;
            break;
        case T:
            tile->center.x = tile->rect.w / 2;
            tile->center.y = tile->rect.h / 4;
            tile->position.y = 1;
            tile->position.x = GridWidth / 2 - 1;
            tile->state = T_0;
            break;
        case Z:
            tile->center.x = tile->rect.w / 2;
            tile->center.y = tile->rect.h / 4 * 3;
            tile->position.y = 1;
            tile->position.x = GridWidth / 2 - 1;
            tile->state = Z_0;
            break;
        case BACKGROUND:
            tile->center.x = tile->rect.w / 2;
            tile->center.y = tile->rect.h / 2;
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
    tile->texture = texture;
    tile->angle = 0;
    tile->shape = shape;
    tile->color = color;

    return tile;

    failure:
        free(tile);
        return NULL;
}

void freeTile(Tile* tile) {
    if(tile == NULL)return;
    SDL_DestroyTexture(tile->texture);
    free(tile);
}

Tile* loadTileRandom(SDL_Renderer* renderer, Point* coordinates, FILE* debug) {
    TileColor color = rand() % 7 + 1;
    TileShape shape = rand() % 7 + 2;
    fprintf(debug, "%d %d\n", color, shape);

    return loadTile(renderer, color, shape, coordinates, debug);
}

void centerTileHorizontally(Tile* tile, ProgramParameters* programParameters) {
    tile->rect.x = programParameters->screen_width / 2 - tile->rect.w / 4;
}

void centerTileVertically(Tile* tile, ProgramParameters* programParameters) {
    tile->rect.y = programParameters->screen_height / 2 - tile->rect.h / 4;
}

void centerTile(Tile* tile, ProgramParameters* programParameters) {
    centerTileVertically(tile, programParameters);
    centerTileHorizontally(tile, programParameters);
}

void printTile(Tile* tile, FILE* stream) {
    if(tile == NULL) {
        fprintf(stream, "Nothing to print\n");
        return;
    }
    fprintf(stream, "Tile color: %d\nTile shape: %d\n", tile->color, tile->shape);
    fprintf(stream, "Position: (%d, %d)\n", tile->position.x, tile->position.y);
}