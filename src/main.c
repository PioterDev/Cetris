#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <windows.h>
#include <timeapi.h>
#include <SDL.h>
#include <SDL_image.h>

#include "config.h"
#include "date.h"
#include "logging.h"
#include "logic.h"
#include "queue.h"
#include "render.h"
#include "structs_unions.h"
#include "tiles.h"
#include "utils.h"

// #define DEBUG

int main(int argc, char** argv) {

    timeBeginPeriod(1);

    LARGE_INTEGER timerStart, timerEnd, frequency;
    QueryPerformanceFrequency(&frequency);
    
    status_t status = SUCCESS; //Exit code
    char errormsgBuffer[128] = {0};

    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    
    #ifdef DEBUG
    FILE* debugLog = fopen("./log/debug.log", "a");
    if(debugLog == NULL) {
        status = FILEOPEN_FAILURE;
        goto exit;
    }
    #else
    FILE* debugLog = NULL;
    #endif

    FILE* generallog = fopen("./log/general.log", "a");
    if(generallog == NULL) {
        status = FILEOPEN_FAILURE;
        goto exit;
    }
    
    FILE* errorlog = fopen("./log/error.log", "a");
    if(errorlog == NULL) {
        status = FILEOPEN_FAILURE;
        goto close_generallog;
    }

    fprintf(generallog, "\n\nWelcome to <the name has not been yet initialized, but we'll get there soon enough...>!\n\n\n");
    logToStream(generallog, "Successfully started!");

    FILE* configFile = fopen("./config/config.cfg", "r");
    if(configFile == NULL) {
        status = FILEOPEN_FAILURE;
        sprintf(errormsgBuffer, "Error opening config file");
        logToStream(errorlog, errormsgBuffer);
        goto close_errorlog;
    }

    ProgramParameters* programParameters = loadConfig(configFile, debugLog);
    if(programParameters == NULL) {
        status = LOADCONFIG_FAILURE;
        sprintf(errormsgBuffer, "Error loading game config");
        logToStream(errorlog, errormsgBuffer);
        goto close_configfile;
    }
    
    logToStream(generallog, "Loaded configuration file.");

    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_AUDIO) != SUCCESS) {
        status = SDL_INIT_FAILURE;
        sprintf(errormsgBuffer, "Error initializing SDL: %s", SDL_GetError());
        logToStream(errorlog, errormsgBuffer);
        goto freeConfig;
    }

    logToStream(generallog, "Initialized SDL.");

    int img_flags = IMG_INIT_PNG | IMG_INIT_JPG;
    if(!(IMG_Init(img_flags) & img_flags)) {
        status = IMG_INIT_FAILURE;
        sprintf(errormsgBuffer, "Error initializing SDL_image: %s", IMG_GetError());
        logToStream(errorlog, errormsgBuffer);
        goto sdl_quit;
    }

    logToStream(generallog, "Attempting to create a window...");
    
    window = SDL_CreateWindow("Title", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, programParameters->screen_width, programParameters->screen_height, SDL_WINDOW_SHOWN);
    
    if(window == NULL) {
        status = SDL_WINDOW_FAILURE;
        sprintf(errormsgBuffer, "Error creating window: %s", SDL_GetError());
        logToStream(errorlog, errormsgBuffer);
        goto img_quit;
    }
    logToStream(generallog, "Window successfully created!");

    logToStream(generallog, "Attempting to create a renderer...");
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(renderer == NULL) {
        status = SDL_RENDERER_FAILURE;
        sprintf(errormsgBuffer, "Error initializing renderer: %s", SDL_GetError());
        logToStream(errorlog, errormsgBuffer);
        goto sdl_destroywindow;
    }
    logToStream(generallog, "Renderer successfully created!");

    Tile** tiles = calloc(2, sizeof(Tile*));
    size_t tilesAmount = 0;
    if(tiles == NULL) {
        status = MEMORY_FAILURE;
        sprintf(errormsgBuffer, "Error allocating memory for tiles");
        logToStream(errorlog, errormsgBuffer);
        goto sdl_destroyrenderer;
    }

    logToStream(generallog, "Attempting to load a background tile...");
    Tile* backgroundTile = loadTile(renderer, COLOR_UNKNOWN, BACKGROUND, NULL, debugLog);
    if(backgroundTile == NULL) {
        status = FAILURE;
        sprintf(errormsgBuffer, "Error loading background tile");
        logToStream(errorlog, errormsgBuffer);
        goto freeTiles;
    }
    tilesAmount++;
    tiles[0] = backgroundTile;
    logToStream(generallog, "Background tile successfully loaded!");

    tiles[1] = loadTile(renderer, RED, SQUARE, NULL, debugLog);
    if(tiles[1] == NULL) {
        status = FAILURE;
        sprintf(errormsgBuffer, "Error loading background tile");
        logToStream(errorlog, errormsgBuffer);
        goto freeTiles;
    }
    tilesAmount++;

    for(size_t i = 0; i < tilesAmount; i++) {
        tiles[i]->rect.w /= 2;
        tiles[i]->rect.h /= 2;
    }

    logToStream(generallog, "Attempting to create a tiles mutex...");
    HANDLE tilesMutex = CreateMutex(NULL, TRUE, NULL);
    if(tilesMutex == NULL) {
        status = MUTEX_FAILURE;
        sprintf(errormsgBuffer, "Error initializing tiles mutex: %ld", GetLastError());
        logToStream(errorlog, errormsgBuffer);
        goto freeTiles;
    }
    logToStream(generallog, "Tiles mutex successfully created!");



    //start of render thread //
    logToStream(generallog, "Attempting to create a render thread mutex...");
    HANDLE renderMutex = CreateMutex(NULL, TRUE, NULL);
    if(renderMutex == NULL) {
        status = MUTEX_FAILURE;
        sprintf(errormsgBuffer, "Error initializing render thread mutex: %ld", GetLastError());
        logToStream(errorlog, errormsgBuffer);
        goto closeTilesMutex;
    }
    logToStream(generallog, "Render thread mutex successfully created!");

    loopStatus_t renderStatus = CONTINUE;
    renderThreadParameters renderParameters = {
        &renderStatus, 
        renderMutex, 
        tilesMutex, 
        renderer, 
        tiles,
        tilesAmount
    };

    logToStream(generallog, "Attempting to start render thread...");
    HANDLE renderThread = CreateThread(NULL, 0, renderScreen, &renderParameters, 0, NULL);
    if(renderThread == NULL) {
        status = THREAD_START_FAILURE;
        sprintf(errormsgBuffer, "Error starting render thread: %ld", GetLastError());
        logToStream(errorlog, errormsgBuffer);
        goto closeRenderMutex;
    }
    logToStream(generallog, "Render thread is now operational.");
    //end of render thread //



    // start of logic thread //
    logToStream(generallog, "Attempting to create an action queue...");
    Queue* actionQueue = initQueue();
    if(actionQueue == NULL) {
        status = MEMORY_FAILURE;
        sprintf(errormsgBuffer, "Error allocating memory for action queue");
        logToStream(errorlog, errormsgBuffer);
        goto closeRenderThread;
    }
    logToStream(generallog, "Successfully created an action queue!");

    logToStream(generallog, "Attempting to create a logic thread mutex...");
    HANDLE logicMutex = CreateMutex(NULL, TRUE, NULL);
    if(logicMutex == NULL) {
        status = MUTEX_FAILURE;
        sprintf(errormsgBuffer, "Error initializing logic thread mutex: %ld", GetLastError());
        logToStream(errorlog, errormsgBuffer);
        goto freeActionQueue;
    }
    logToStream(generallog, "Logic thread mutex successfully created!");

    loopStatus_t logicStatus = CONTINUE;
    logicLoopParameters logicParameters = {
        &logicStatus, 
        logicMutex, 
        tilesMutex,
        actionQueue,
        tiles,
        tilesAmount
    };

    HANDLE logicThread = CreateThread(NULL, 0, logicLoop, &logicParameters, 0, NULL);
    if(logicThread == NULL) {
        status = THREAD_START_FAILURE;
        sprintf(errormsgBuffer, "Error starting logic thread: %ld", GetLastError());
        logToStream(errorlog, errormsgBuffer);
        goto closeLogicMutex;
    }
    logToStream(generallog, "Logic thread is now operational.");
    //end of logic thread //



    long long delta = 0; //time needed to process logic

    bool running = true;
    SDL_Event event;

    ReleaseMutex(tilesMutex);
    ReleaseMutex(renderMutex); //start render thread
    ReleaseMutex(logicMutex); //start logic thread
    while(running) { //main game loop
        QueryPerformanceCounter(&timerStart);
        WaitForSingleObject(renderMutex, INFINITE);
        WaitForSingleObject(tilesMutex, INFINITE);
        WaitForSingleObject(logicMutex, INFINITE);
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                running = false;
                renderStatus = STOP;
                logicStatus = STOP;

                ReleaseMutex(tilesMutex);
                ReleaseMutex(logicMutex);
                ReleaseMutex(renderMutex);
                
                WaitForSingleObject(renderThread, INFINITE);
                WaitForSingleObject(logicThread, INFINITE);
                
                goto exit_start;
            }
            else if(event.type == SDL_KEYDOWN) {
                SDL_Keycode key = event.key.keysym.sym;
                Keymap keymap = programParameters->keymap;
                SDL_Rect* rect = &tiles[1]->rect;
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
                    if(rect->y >= 10)rect->y -= 32;
                }
            }
        }
        
        //formula: [ticks per second (probably 10^7) / FPS - time elapsed for input processing] 
        delta = frequency.QuadPart / programParameters->fps - (timerEnd.QuadPart - timerStart.QuadPart);

        delta = delta * 1000 / frequency.QuadPart; //* 1000 / ticks per second (to get value in miliseconds)
        while(true) {
            QueryPerformanceCounter(&timerEnd);
            if(timerEnd.QuadPart - timerStart.QuadPart > frequency.QuadPart / (programParameters->fps))break;
            //there is an offset of 2 to account for system interrupts
            //looks cursed, is cursed, works though lmao
            Sleep(1);
        } //anyway, I'm gonna use VSync instead of this atrocity
        //UPDATE: no, I'm not gonna use VSync, screw it
        ReleaseMutex(tilesMutex);
        ReleaseMutex(renderMutex); //allow for rendering
    }



    exit_start: 
        timeEndPeriod(1);
        logToStream(generallog, "Exiting...");

    //sdl_freesurface_image: SDL_FreeSurface(imageSurface);

    //sdl_freesurface_main: SDL_FreeSurface(windowSurface);
    //closeInputThread: CloseHandle(inputThread);

    //closeInputMutex: CloseHandle(inputMutex);
    closeLogicThread:
        if(logicStatus != STOP)logicStatus = STOP;
        WaitForSingleObject(logicThread, INFINITE);
        CloseHandle(logicThread);

    closeLogicMutex: CloseHandle(logicMutex);

    freeActionQueue: freeQueue(actionQueue);

    closeRenderThread:
        if(renderStatus != STOP)renderStatus = STOP;
        WaitForSingleObject(renderThread, INFINITE);
        CloseHandle(renderThread);

    closeRenderMutex: CloseHandle(renderMutex);

    closeTilesMutex: CloseHandle(tilesMutex);
    
    freeTiles: 
        for(size_t i = 0; i < tilesAmount; i++) {
            freeTile(tiles[i]);
        }
        free(tiles);

    sdl_destroyrenderer: SDL_DestroyRenderer(renderer);

    sdl_destroywindow: SDL_DestroyWindow(window);

    img_quit: IMG_Quit();

    sdl_quit: SDL_Quit();

    // freeConfig: freeProgramConfig(programParameters);
    freeConfig: free(programParameters);

    close_configfile: fclose(configFile);

    close_errorlog: fclose(errorlog);

    close_generallog: 
        if(status == SUCCESS)logToStream(generallog, "Goodbye!");
        fclose(generallog);

    exit:
        #ifdef DEBUG
        fclose(debugLog);
        #endif

        return status;
}