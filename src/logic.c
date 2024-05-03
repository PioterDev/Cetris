#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <windows.h>
#include <SDL.h>

#include "queue.h"
#include "structs_unions.h"
#include "utils.h"

DWORD WINAPI logicLoop(void* params) {
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
}