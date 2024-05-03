#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <windows.h>
#include <SDL.h>

#include "queue.h"
#include "structs_unions_defines.h"
#include "utils.h"

/* DWORD WINAPI logicLoop(void* params) {
    logicLoopParameters* parameters = params;
    ProgramParameters* programParameters = parameters->programParameters;
    Queue* q = parameters->eventQueue;

    while(true) {
        if(*(parameters->logicStatus) == STOP)break;

        WaitForSingleObject(parameters->logicMutex, INFINITE);
        WaitForSingleObject(parameters->tilesMutex, INFINITE);
        
        while(q->size == 0)Sleep(1);

        SDL_Event event; //change later accordingly
        while(dequeue(q, &event)) {
            if(programParameters->debugLog != NULL)fprintf(programParameters->debugLog, "Dequeued event\n");
            if(event.type == SDL_QUIT) {
                *(parameters->logicStatus) = STOP;
                break;
            }
            else if(event.type == SDL_KEYDOWN) {
                SDL_Keycode key = event.key.keysym.sym;
                Keymap keymap = parameters->programParameters->keymap;
                SDL_Rect* rect = &parameters->tiles[1]->rect;

                if(key == keymap.movePieceLeft) {
                    if(rect->x >= 10)rect->x -= 32;
                }
                else if(key == keymap.movePieceRight) {
                    if(rect->x <= programParameters->screen_width)rect->x += 32;
                }
                else if(key == keymap.dropSoft) {
                    if(rect->y <= programParameters->screen_height)rect->y += 32;
                }
                else if(key == keymap.rotateClockwise) {
                    if(rect->y >= 32)rect->y -= 32;
                }
            }
        }

        ReleaseMutex(parameters->tilesMutex);
        ReleaseMutex(parameters->logicMutex);
    }

    return SUCCESS;
} */

status_t dropHard(char** tetrisGrid, int x, int y, TileShape shape, TileColor color, int tetrisGridWidth, int tetrisGridHeight) {
    /**
     * Convention is as follows:
     * (x, y) point to left-most square of the current tile.
     * If there are multiple, it points to the top-most one.
     * For T, J and L-shape, it points to the tile that "sticks out".
     */
    switch(shape) {
        case BAR: {
            if(tetrisGrid[y][x + 1] == -1 * BAR) {
                //[][][][]
                int z[4] = {y + 1, y + 1, y + 1, y + 1};
                while(
                    (tetrisGrid[z[0]][x]    == 0 &&
                    tetrisGrid[z[1]][x + 1] == 0 &&
                    tetrisGrid[z[2]][x + 2] == 0 &&
                    tetrisGrid[z[3]][x + 3] == 0) 
                    &&
                    (
                        z[0] < tetrisGridHeight &&
                        z[1] < tetrisGridHeight &&
                        z[2] < tetrisGridHeight &&
                        z[3] < tetrisGridHeight
                    )
                ) {
                    z[0]++;
                    z[1]++;
                    z[2]++;
                    z[3]++;
                }
                int min = z[MinIndex(z, 4)];
                min--;

                tetrisGrid[min][x]     = color;
                tetrisGrid[min][x + 1] = color;
                tetrisGrid[min][x + 2] = color;
                tetrisGrid[min][x + 3] = color;
                
                tetrisGrid[y][x]     = 0;
                tetrisGrid[y][x + 1] = 0;
                tetrisGrid[y][x + 2] = 0;
                tetrisGrid[y][x + 3] = 0;
            }
            else if(tetrisGrid[y + 1][x] == -1 * BAR) {
                //[]
                //[]
                //[]
                //[]
                int z = y + 1;
                while(tetrisGrid[z][x] == 0 && z < tetrisGridHeight)z++;
                z--;

                tetrisGrid[z]    [x] = color;
                tetrisGrid[z + 1][x] = color;
                tetrisGrid[z + 2][x] = color;
                tetrisGrid[z + 3][x] = color;

                tetrisGrid[y]    [x] = 0;
                tetrisGrid[y + 1][x] = 0;
                tetrisGrid[y + 2][x] = 0;
                tetrisGrid[y + 3][x] = 0;
            }
            break;
        }
        case J: {
            if(tetrisGrid[y][x + 1] == -1 * J) {
                //  []
                //  []
                //[][]
                int z[2] = {y + 1, y + 1};
                while(
                    (tetrisGrid[z[0]][x] == 0 && tetrisGrid[z[1]][x + 1] == 0)
                    &&
                    (z[0] < tetrisGridHeight && z[1] < tetrisGridHeight)
                ) {
                    z[0]++;
                    z[1]++;
                }
                int min = z[0] < z[1] ? z[0] : z[1];
                min--;

                tetrisGrid[min]     [x]     = color;
                tetrisGrid[min]     [x + 1] = color;
                tetrisGrid[min - 1] [x + 1] = color;
                tetrisGrid[min - 2] [x + 1] = color;

                tetrisGrid[y][x] = 0;
                tetrisGrid[y][x + 1] = 0;
                tetrisGrid[y - 1][x + 1] = 0;
                tetrisGrid[y - 2][x + 1] = 0;
            }
            else if(tetrisGrid[y][x - 1] == -1 * J) {
                //[][]
                //[]
                //[]
                int z[2] = {y + 3, y + 3};
                while(
                    (tetrisGrid[z[0]][x] == 0 && tetrisGrid[z[1]][x - 1] == 0)
                    &&
                    (z[0] < tetrisGridHeight && z[1] < tetrisGridHeight)
                ) {
                    z[0]++;
                    z[1]++;
                }
                if(z[0] < z[1]) {
                    z[0]--;

                    tetrisGrid[z[0]]    [x]     = color;
                    tetrisGrid[z[0]]    [x - 1] = color;
                    tetrisGrid[z[0] + 1][x - 1] = color;
                    tetrisGrid[z[0] + 2][x - 1] = color;
                }
                else if(z[1] < z[0]) {
                    z[1]--;

                    tetrisGrid[z[1]]    [x - 1] = color;
                    tetrisGrid[z[1] - 1][x - 1] = color;
                    tetrisGrid[z[1] - 2][x - 1] = color;
                    tetrisGrid[z[1] - 2][x]     = color;
                }

                tetrisGrid[y]    [x]     = 0;
                tetrisGrid[y]    [x - 1] = 0;
                tetrisGrid[y - 1][x - 1] = 0;
                tetrisGrid[y - 2][x - 1] = 0;
            }
            else if(tetrisGrid[y + 1][x] == -1 * J) {
                //[]
                //[][][]
                int z[3] = {y + 2, y + 2, y + 2};

                while(
                    (tetrisGrid[z[0]][x] == 0 && tetrisGrid[z[1]][x + 1] == 0 && tetrisGrid[z[2]][x + 2] == 0)
                    &&
                    (z[0] < tetrisGridHeight && z[1] < tetrisGridHeight && z[2] < tetrisGridHeight)
                ) {
                    z[0]++;
                    z[1]++;
                    z[2]++;
                }

                int min = Min(z, 3);
                min--;

                tetrisGrid[min - 1] [x] = color;
                tetrisGrid[min]     [x] = color;
                tetrisGrid[min]     [x] = color;
                tetrisGrid[min]     [x] = color;

                tetrisGrid[y]    [x]     = 0;
                tetrisGrid[y + 1][x]     = 0;
                tetrisGrid[y + 1][x + 1] = 0;
                tetrisGrid[y + 1][x + 2] = 0;
            }
            
            else if(tetrisGrid[y - 1][x] == -1 * J) {
                //[][][]
                //    []
                int z[3] = {y + 1, y + 1, y + 1};
                while(
                    (tetrisGrid[z[0]][x] == 0 && tetrisGrid[z[1]][x - 1] == 0 && tetrisGrid[z[2]][x - 2] == 0)
                    &&
                    (z[0] < tetrisGridHeight && z[1] < tetrisGridHeight && z[2] < tetrisGridHeight)
                ) {
                    z[0]++;
                    z[1]++;
                    z[2]++;
                }
                
                int minIndex = MinIndex(z, 3);
                if(minIndex == 0) {
                    z[0]--;

                    tetrisGrid[z[0]]    [x]     = color;
                    tetrisGrid[z[0] - 1][x]     = color;
                    tetrisGrid[z[0] - 1][x - 1] = color;
                    tetrisGrid[z[0] - 1][x - 2] = color;
                }
                else {
                    z[minIndex]--;

                    tetrisGrid[z[minIndex] + 1][x]     = color;
                    tetrisGrid[z[minIndex]]    [x]     = color;
                    tetrisGrid[z[minIndex]]    [x - 1] = color;
                    tetrisGrid[z[minIndex]]    [x - 2] = color;
                }

                tetrisGrid[y]    [x]     = 0;
                tetrisGrid[y - 1][x]     = 0;
                tetrisGrid[y - 1][x - 1] = 0;
                tetrisGrid[y - 1][x - 2] = 0;
            }
            break;
        }
        case L: {
            if(tetrisGrid[y][x - 1] == -1 * L) {
                //[]
                //[]
                //[][]
                int z[2] = {y + 1, y + 1};
                while(
                    (tetrisGrid[z[0]][x] == 0 && tetrisGrid[z[1]][x - 1] == 0)
                    &&
                    (z[0] < tetrisGridHeight && z[1] < tetrisGridHeight)
                ) {
                    z[0]++;
                    z[1]++;
                }
                int min = z[0] < z[1] ? z[0] : z[1];
                min--;

                tetrisGrid[min]     [x]     = color;
                tetrisGrid[min]     [x - 1] = color;
                tetrisGrid[min - 1] [x - 1] = color;
                tetrisGrid[min - 2] [x - 1] = color;

                tetrisGrid[y]    [x]     = 0;
                tetrisGrid[y]    [x - 1] = 0;
                tetrisGrid[y - 1][x - 1] = 0;
                tetrisGrid[y - 2][x - 1] = 0;
            }
            else if(tetrisGrid[y][x + 1] == -1 * L) {
                //[][]
                //  []
                //  []
                int z[2] = {y + 3, y + 3};
                while(
                    (tetrisGrid[z[0]][x] == 0 && tetrisGrid[z[1]][x + 1] == 0)
                    &&
                    (z[0] < tetrisGridHeight && z[1] < tetrisGridHeight)
                ) {
                    z[0]++;
                    z[1]++;
                }

                if(z[0] < z[1]) {
                    z[0]--;

                    tetrisGrid[z[0]]    [x]     = color;
                    tetrisGrid[z[0]]    [x + 1] = color;
                    tetrisGrid[z[0] + 1][x + 1] = color;
                    tetrisGrid[z[0] + 2][x + 1] = color;
                }
                else if(z[1] < z[0]) {
                    z[1]--;

                    tetrisGrid[z[1]]    [x + 1] = color;
                    tetrisGrid[z[1] - 1][x + 1] = color;
                    tetrisGrid[z[1] - 2][x + 1] = color;
                    tetrisGrid[z[1] - 2][x]     = color;
                }

                tetrisGrid[y]    [x]     = 0;
                tetrisGrid[y]    [x + 1] = 0;
                tetrisGrid[y + 1][x + 1] = 0;
                tetrisGrid[y + 2][x + 1] = 0;
            }
            else if(tetrisGrid[y - 1][x] == -1 * L) {
                //[][][]
                //[]
                int z[3] = {y + 1, y + 1, y + 1};
                while(
                    (tetrisGrid[z[0]][x] == 0 && tetrisGrid[z[1]][x + 1] == 0 && tetrisGrid[z[2]][x + 2] == 0)
                    &&
                    (z[0] < tetrisGridHeight && z[0] < tetrisGridHeight && z[0] < tetrisGridHeight)
                ) {
                    z[0]++;
                    z[1]++;
                    z[2]++;
                }

                int minIndex = MinIndex(z, 3);
                if(minIndex == 0) {
                    z[0]--;

                    tetrisGrid[z[0]]    [x]     = color;
                    tetrisGrid[z[0] - 1][x]     = color;
                    tetrisGrid[z[0] - 1][x + 1] = color;
                    tetrisGrid[z[0] - 1][x + 2] = color;
                }
                else {
                    z[minIndex]--;

                    tetrisGrid[z[minIndex] + 1] [x]     = color;
                    tetrisGrid[z[minIndex]]     [x]     = color;
                    tetrisGrid[z[minIndex]]     [x + 1] = color;
                    tetrisGrid[z[minIndex]]     [x + 2] = color;
                }

                tetrisGrid[y]    [x]     = 0;
                tetrisGrid[y - 1][x]     = 0;
                tetrisGrid[y - 1][x + 1] = 0;
                tetrisGrid[y - 1][x + 2] = 0;
            }
            else if(tetrisGrid[y + 1][x] == -1 * L) {
                //    []
                //[][][]
                int z[3] = {y + 2, y + 2, y + 2};
                while(
                    (tetrisGrid[z[0]][x] == 0 && tetrisGrid[z[1]][x - 1] == 0 && tetrisGrid[z[2]][x - 2] == 0)
                    &&
                    (z[0] < tetrisGridHeight && z[0] < tetrisGridHeight && z[0] < tetrisGridHeight)
                ) {
                    z[0]++;
                    z[1]++;
                    z[2]++;
                }

                int min = Min(z, 3);
                min--;

                tetrisGrid[min - 1][x]     = color;
                tetrisGrid[min]    [x]     = color;
                tetrisGrid[min]    [x - 1] = color;
                tetrisGrid[min]    [x - 2] = color;

                tetrisGrid[y]    [x]     = 0;
                tetrisGrid[y + 1][x]     = 0;
                tetrisGrid[y + 1][x - 1] = 0;
                tetrisGrid[y + 1][x - 2] = 0;
            }
            break;
        }
        case S: {

            break;
        }
        case SQUARE: {
            int z[2] = {y + 1, y + 1};
            while( 
                (tetrisGrid[z[0]][x] == 0 && tetrisGrid[z[1]][x + 1] == 0)
                &&
                (z[0] < tetrisGridHeight && z[1] < tetrisGridHeight)
            ) {
                z[0]++;
                z[1]++;
            }
            int min = z[0] < z[1] ? z[0] : z[1];
            min--;

            tetrisGrid[min][x]      = color;
            tetrisGrid[min][x + 1]  = color;
            tetrisGrid[min][x]      = color;
            tetrisGrid[min][x + 1]  = color;

            tetrisGrid[y]    [x]     = 0;
            tetrisGrid[y]    [x + 1] = 0;
            tetrisGrid[y + 1][x]     = 0;
            tetrisGrid[y + 1][x + 1] = 0;

            break;
        }
        case T: {
            if(tetrisGrid[y][x - 1] == -1 * T) {
                //[]
                //[][]
                //[]
                int z[2] = {y + 2, y + 2};
                while(
                    (tetrisGrid[z[0]][x - 1] == 0 && tetrisGrid[z[1]][x] == 0)
                    &&
                    (z[0] < tetrisGridHeight && z[1] < tetrisGridHeight)
                ) {
                    z[0]++;
                    z[1]++;
                }
                if(z[0] < z[1]) {
                    z[0]--;
                    tetrisGrid[z[0]]    [x - 1] = color;
                    tetrisGrid[z[0] - 1][x - 1] = color;
                    tetrisGrid[z[0] - 1][x] = color;
                    tetrisGrid[z[0] - 2][x - 1] = color;
                }
                else if(z[1] < z[0]) {
                    z[1]--;

                    tetrisGrid[z[1] + 1][x - 1] = color;
                    tetrisGrid[z[1]]    [x - 1] = color;
                    tetrisGrid[z[1]]    [x]     = color;
                    tetrisGrid[z[1] - 1][x - 1] = color;
                }
                
                tetrisGrid[y + 1][x - 1] = 0;
                tetrisGrid[y]    [x - 1] = 0;
                tetrisGrid[y]    [x]     = 0;
                tetrisGrid[y - 1][x - 1] = 0;

            }
            else if(tetrisGrid[y][x + 1] == -1 * T) {
                //  []
                //[][]
                //  []
                int z[2] = {y + 2, y + 2};
                while(
                    (tetrisGrid[z[0]][x + 1] == 0 && tetrisGrid[z[1]][x] == 0)
                    &&
                    (z[0] < tetrisGridHeight && z[1] < tetrisGridHeight)
                ) {
                    z[0]++;
                    z[1]++;
                }
                if(z[0] < z[1]) {
                    z[0]--;

                    tetrisGrid[z[0]]    [x + 1] = color;
                    tetrisGrid[z[0] - 1][x + 1] = color;
                    tetrisGrid[z[0] - 1][x]     = color;
                    tetrisGrid[z[0] - 2][x + 1] = color;
                }
                else if(z[1] < z[0]) {
                    z[1]--;

                    tetrisGrid[z[1] + 1][x + 1] = color;
                    tetrisGrid[z[1]]    [x + 1] = color;
                    tetrisGrid[z[1]]    [x]     = color;
                    tetrisGrid[z[1] - 1][x + 1] = color;
                }
                
                tetrisGrid[y + 1][x + 1] = 0;
                tetrisGrid[y]    [x + 1] = 0;
                tetrisGrid[y]    [x]     = 0;
                tetrisGrid[y - 1][x + 1] = 0;
            }
            else if(tetrisGrid[y - 1][x] == -1 * T) {
                //[][][]
                //  []
                int z[3] = {y + 1, y + 1, y + 1};
                while(
                    (tetrisGrid[z[0]][x - 1] == 0 && tetrisGrid[z[1]][x] == 0 && tetrisGrid[z[2]][x + 1] == 0)
                    &&
                    (z[0] < tetrisGridHeight && z[1] < tetrisGridHeight && z[2] < tetrisGridHeight)
                ) {
                    z[0]++;
                    z[1]++;
                    z[2]++;
                }
                int min = Min(z, 3);
                min--;

                tetrisGrid[min]    [x]    = color;
                tetrisGrid[min - 1][x - 1] = color;
                tetrisGrid[min - 1][x]     = color;
                tetrisGrid[min - 1][x + 1] = color;

                tetrisGrid[y]    [x] = 0;
                tetrisGrid[y - 1][x - 1] = 0;
                tetrisGrid[y - 1][x] = 0;
                tetrisGrid[y - 1][x + 1] = 0;
            }
            else if(tetrisGrid[y + 1][x] == -1 * T) {
                //  []
                //[][][]
                int z[3] = {y + 2, y + 2, y + 2};
                while(
                    (tetrisGrid[z[0]][x - 1] == 0 && tetrisGrid[z[1]][x] == 0 && tetrisGrid[z[2]][x + 1] == 0)
                    &&
                    (z[0] < tetrisGridHeight && z[1] < tetrisGridHeight && z[2] < tetrisGridHeight)
                ) {
                    z[0]++;
                    z[1]++;
                    z[2]++;
                }
                int min = Min(z, 3);
                min--;

                tetrisGrid[min - 1][x]     = color;
                tetrisGrid[min]    [x]    = color;
                tetrisGrid[min]    [x - 1] = color;
                tetrisGrid[min]    [x + 1] = color;

                tetrisGrid[y]    [x] = 0;
                tetrisGrid[y + 1][x - 1] = 0;
                tetrisGrid[y + 1][x] = 0;
                tetrisGrid[y + 1][x + 1] = 0;
            }
            break;
        }
        case Z: {

            break;
        }
        default: return FAILURE;
    }
    return SUCCESS;
}