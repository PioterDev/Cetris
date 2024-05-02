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

    while(true) {
        if(*(parameters->logicStatus) == STOP)break;

        WaitForSingleObject(parameters->logicMutex, INFINITE);
        WaitForSingleObject(parameters->tilesMutex, INFINITE);
        
        void* action; //change later accordingly
        while(dequeueAlt(parameters->actionQueue, action)) {

        }

        ReleaseMutex(parameters->tilesMutex);
        ReleaseMutex(parameters->logicMutex);
    }

    return SUCCESS;
}