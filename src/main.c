#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <windows.h>
#include <timeapi.h>
#include <SDL.h>
#include <SDL_image.h>

#include "clock_thread.h"
#include "config.h"
#include "date.h"
#include "logging.h"
#include "logic.h"
#include "queue.h"
#include "render.h"
#include "structs_unions_defines.h"
#include "tiles.h"
#include "utils.h"

// #define DEBUG
#define TEST

int main(int argc, char** argv) {

    srand(time(NULL));
    timeBeginPeriod(1);

    LARGE_INTEGER timer, frequency;
    QueryPerformanceFrequency(&frequency);

    const int tetrisGridHeight = 20;
    const int tetrisGridWidth = 10;

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



    // start of timer thread //
    logToStream(generallog, "Attempting to start a high performance clock...");
    loopStatus_t clockStatus = CONTINUE;
    clockThreadParameters clockParameters = {
        &clockStatus,
        &timer
    };



    HANDLE clockThread = CreateThread(NULL, 0, highPerformanceClockThread, &clockParameters, 0, NULL);;
    if(clockThread == NULL) {
        status = THREAD_START_FAILURE;
        sprintf(errormsgBuffer, "Error starting clock: %ld", GetLastError());
        logToStream(errorlog, errormsgBuffer);
        goto close_errorlog;
    }
    logToStream(generallog, "High performance clock started!");
    // end of timer thread //



    FILE* configFile = fopen("./config/config.cfg", "r");
    if(configFile == NULL) {
        status = FILEOPEN_FAILURE;
        sprintf(errormsgBuffer, "Error opening config file");
        logToStream(errorlog, errormsgBuffer);
        goto closeClockThread;
    }



    ProgramParameters* programParameters = loadConfig(configFile, debugLog);
    if(programParameters == NULL) {
        status = LOADCONFIG_FAILURE;
        sprintf(errormsgBuffer, "Error loading game config.");
        logToStream(errorlog, errormsgBuffer);
        goto close_configfile;
    }
    printConfig(programParameters, debugLog);
    programParameters->baseTileSize = 0;
    programParameters->scalingFactor = 0;
    programParameters->clockFrequency = &frequency;
    programParameters->timer = &timer;
    programParameters->generallog = generallog;
    programParameters->errorlog = errorlog;
    programParameters->debugLog = debugLog;
    #ifdef TEST
    programParameters->keymap.test = SDLK_w;
    #endif
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



    logToStream(generallog, "Attempting to load base tile textures...");
    if(loadBaseTextures(programParameters, renderer) != SUCCESS) {
        status = FAILURE;
        sprintf(errormsgBuffer, "Error loading base tile textures.");
        logToStream(errorlog, errormsgBuffer);
        goto sdl_destroyrenderer;
    }
    logToStream(generallog, "Base tile textures successfully loaded!");



    logToStream(generallog, "Attempting to create a game matrix...");
    char** tetrisGrid = zeroMatrix(tetrisGridHeight, tetrisGridWidth);
    if(tetrisGrid == NULL) {
        status = MEMORY_FAILURE;
        sprintf(errormsgBuffer, "Error allocating memory for the game matrix.");
        logToStream(errorlog, errormsgBuffer);
        goto sdl_destroyrenderer;
    }
    programParameters->tetrisGrid = tetrisGrid;
    programParameters->tetrisGridHeight = tetrisGridHeight;
    programParameters->tetrisGridWidth = tetrisGridWidth;
    logToStream(generallog, "Game matrix successfully created!");



    Tile** tiles = calloc(3, sizeof(Tile*));
    size_t tilesAmount = 0;
    if(tiles == NULL) {
        status = MEMORY_FAILURE;
        sprintf(errormsgBuffer, "Error allocating memory for tiles.");
        logToStream(errorlog, errormsgBuffer);
        goto sdl_destroyrenderer;
    }



    logToStream(generallog, "Attempting to load a background tile...");
    Tile* backgroundTile = loadTile(renderer, COLOR_UNKNOWN, BACKGROUND, NULL, debugLog);
    if(backgroundTile == NULL) {
        status = FAILURE;
        sprintf(errormsgBuffer, "Error loading background tile.");
        logToStream(errorlog, errormsgBuffer);
        goto freeTiles;
    }
    centerTileHorizontally(backgroundTile, programParameters);
    tiles[0] = backgroundTile;
    tilesAmount++;
    if(backgroundTile->rect.h > programParameters->screen_height || backgroundTile->rect.w > programParameters->screen_width) {
        programParameters->scalingFactor = -1;
        while(
            backgroundTile->rect.h / abs(programParameters->scalingFactor) > programParameters->screen_height ||
            backgroundTile->rect.w / abs(programParameters->scalingFactor) > programParameters->screen_width
        ) {programParameters->scalingFactor--;}
    }
    else if(backgroundTile->rect.h * 2 < programParameters->screen_height || backgroundTile->rect.w * 2 < programParameters->screen_width) {
        programParameters->scalingFactor = 1;
        while(
            backgroundTile->rect.h * programParameters->scalingFactor < programParameters->screen_height ||
            backgroundTile->rect.w * programParameters->scalingFactor < programParameters->screen_width
        ) {programParameters->scalingFactor++;}
    }
    logToStream(generallog, "Background tile successfully loaded!");



    tiles[1] = loadTile(renderer, RED, L, NULL, debugLog);
    if(tiles[1] == NULL) {
        status = FAILURE;
        sprintf(errormsgBuffer, "Error loading current tile.");
        logToStream(errorlog, errormsgBuffer);
        goto freeTiles;
    }
    tilesAmount++;



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
    Color backgroundColor; backgroundColor.color = 0xFFFFFFFF;
    renderThreadParameters renderParameters = {
        &renderStatus, 
        renderMutex, 
        tilesMutex,
        programParameters,
        renderer,
        &backgroundColor,
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



    /* // start of logic thread //
    logToStream(generallog, "Attempting to create an action queue...");
    Queue* eventQueue = initQueue();
    if(eventQueue == NULL) {
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
        goto freeEventQueue;
    }
    logToStream(generallog, "Logic thread mutex successfully created!");

    loopStatus_t logicStatus = CONTINUE;
    logicLoopParameters logicParameters = {
        &logicStatus, 
        logicMutex, 
        tilesMutex,
        programParameters,
        eventQueue,
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
    //end of logic thread // */


    //long long delta = 0; //time needed to process logic

    bool running = true;
    SDL_Event event;

    ReleaseMutex(tilesMutex);
    ReleaseMutex(renderMutex); //start render thread

    //ReleaseMutex(logicMutex); //start logic thread
    while(running) { //main game loop
        WaitForSingleObject(tilesMutex, INFINITE);
        
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                running = false;
                renderStatus = STOP;

                ReleaseMutex(tilesMutex);
                ReleaseMutex(renderMutex);
                
                WaitForSingleObject(renderThread, INFINITE);
                
                goto exit_start;
            }
            else if(event.type == SDL_KEYDOWN) {
                SDL_Keycode key = event.key.keysym.sym;
                Keymap keymap = programParameters->keymap;
                if(tiles[1] == NULL)continue;
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
                else if(key == keymap.test) {
                    freeTile(tiles[1]);
                    tiles[1] = loadTileRandom(renderer, NULL, debugLog);
                    if(tiles[1] == NULL) {
                        logToStream(errorlog, "Error loading random tile");
                    }
                }
            }
        }

        ReleaseMutex(tilesMutex);
        //ReleaseMutex(logicMutex);
    }



    exit_start: 
        timeEndPeriod(1);
        logToStream(generallog, "Exiting...");

    //sdl_freesurface_image: SDL_FreeSurface(imageSurface);

    //sdl_freesurface_main: SDL_FreeSurface(windowSurface);
    //closeInputThread: CloseHandle(inputThread);

    //closeInputMutex: CloseHandle(inputMutex);
    /* closeLogicThread:
        if(logicStatus != STOP)logicStatus = STOP;
        WaitForSingleObject(logicThread, INFINITE);
        CloseHandle(logicThread);

    closeLogicMutex: CloseHandle(logicMutex);

    freeEventQueue: freeQueue(eventQueue); */

    closeRenderThread:
        if(renderStatus != STOP)renderStatus = STOP;
        WaitForSingleObject(renderThread, INFINITE);
        CloseHandle(renderThread);

    closeRenderMutex: CloseHandle(renderMutex);

    closeTilesMutex: CloseHandle(tilesMutex);
    
    freeTiles: 
        for(size_t i = 0; i < tilesAmount; i++) {
            if(tiles[i] != NULL) {
                freeTile(tiles[i]);
            }
        }
        free(tiles);

    sdl_destroyrenderer: SDL_DestroyRenderer(renderer);

    sdl_destroywindow: SDL_DestroyWindow(window);

    img_quit: IMG_Quit();

    sdl_quit: SDL_Quit();

    freeConfig: freeProgramConfig(programParameters);

    close_configfile: fclose(configFile);

    closeClockThread:
        clockStatus = STOP;
        WaitForSingleObject(clockThread, INFINITE);
        CloseHandle(clockThread);

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