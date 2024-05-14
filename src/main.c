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

#define DEBUG
#define TEST

int main(int argc, char** argv) {

    srand(time(NULL));
    timeBeginPeriod(1);

    LARGE_INTEGER timer, frequency;
    long long tickTimerStart = 0, tickTimerEnd = 0;
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
    programParameters->baseTileSize = 0;
    programParameters->scalingFactor = 0;
    
    programParameters->clockFrequency = &frequency;
    programParameters->timer = &timer;
    
    programParameters->generallog = generallog;
    programParameters->errorlog = errorlog;
    programParameters->debugLog = debugLog;

    programParameters->tetrisGridSize.height = GridHeight;
    programParameters->tetrisGridSize.width = GridWidth;
    
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
    window = SDL_CreateWindow("Tetris", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, programParameters->screenSize.width, programParameters->screenSize.height, SDL_WINDOW_SHOWN);
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
    char** tetrisGrid = zeroMatrix(programParameters->tetrisGridSize);
    if(tetrisGrid == NULL) {
        status = MEMORY_FAILURE;
        sprintf(errormsgBuffer, "Error allocating memory for the game matrix.");
        logToStream(errorlog, errormsgBuffer);
        goto sdl_destroyrenderer;
    }
    programParameters->tetrisGrid = tetrisGrid;
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
    //dynamic calculation of by how much should everything be scaled
    if(backgroundTile->rect.h > programParameters->screenSize.height || backgroundTile->rect.w > programParameters->screenSize.width) {
        programParameters->scalingFactor = -1;
        while(
            backgroundTile->rect.h / abs(programParameters->scalingFactor) > programParameters->screenSize.height ||
            backgroundTile->rect.w / abs(programParameters->scalingFactor) > programParameters->screenSize.width
        ) {programParameters->scalingFactor--;}
    }
    else if(backgroundTile->rect.h * 2 < programParameters->screenSize.height || backgroundTile->rect.w * 2 < programParameters->screenSize.width) {
        programParameters->scalingFactor = 1;
        while(
            backgroundTile->rect.h * programParameters->scalingFactor < programParameters->screenSize.height ||
            backgroundTile->rect.w * programParameters->scalingFactor < programParameters->screenSize.width
        ) {programParameters->scalingFactor++;}
    }
    logToStream(generallog, "Background tile successfully loaded!");

    Tile* currentTile = tiles[1];
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


    printConfig(programParameters, debugLog);

    bool running = true;
    SDL_Event event;

    ReleaseMutex(tilesMutex);
    ReleaseMutex(renderMutex); //start render thread

    tickTimerStart = tickTimerEnd = timer.QuadPart;
    MovementSpeed speed = NORMAL;

    while(running) { //main game loop
        WaitForSingleObject(tilesMutex, INFINITE);
        while(SDL_PollEvent(&event)) {
            switch(event.type) {
                case SDL_QUIT: {
                    running = false;
                    renderStatus = STOP;

                    ReleaseMutex(tilesMutex);
                    ReleaseMutex(renderMutex);
                    
                    WaitForSingleObject(renderThread, INFINITE);
                    
                    goto exit_start;
                }
                case SDL_KEYDOWN: {
                    SDL_Keycode key = event.key.keysym.sym;
                
                    if(debugLog != NULL)fprintf(debugLog, "%d\n", key);

                    if(key == programParameters->keymap.dropHard) {
                        dropHard(tetrisGrid, currentTile, programParameters->tetrisGridSize);
                        onPlacement(tetrisGrid, programParameters->tetrisGridSize, 0); //TODO: score
                        freeTile(currentTile);
                        currentTile = loadTileRandom(renderer, NULL, debugLog);
                        if(currentTile == NULL) {
                            logToStream(errorlog, "Error loading tile");
                        }
                        else loadTileIntoGrid(tetrisGrid, currentTile);
                        speed = NORMAL;
                    }
                    else if(key == programParameters->keymap.movePieceLeft) {
                        moveLeft(tetrisGrid, currentTile);
                    }
                    else if(key == programParameters->keymap.movePieceRight) {
                        moveRight(tetrisGrid, currentTile, programParameters->tetrisGridSize.width);
                    }
                    else if(key == programParameters->keymap.dropSoft) {
                        speed = DROPSOFT;
                    }
                    else if(key == programParameters->keymap.rotateClockwise) {
                        rotateClockwise(tetrisGrid, currentTile);
                    }
                    else if(key == programParameters->keymap.rotateCounterClockwise) {
                        rotateCounterClockwise(tetrisGrid, currentTile);
                    }
                    else if(key == programParameters->keymap.hold) {
                        speed = HOLD;
                    }
                    #ifdef TEST
                    else if(key == programParameters->keymap.test) {
                        freeTile(currentTile);
                        currentTile = loadTileRandom(renderer, NULL, debugLog);
                        if(currentTile == NULL) {
                            logToStream(errorlog, "Error loading tile");
                        }
                        else loadTileIntoGrid(tetrisGrid, currentTile);
                    }
                    else if(key == SDLK_1) {
                        freeTile(currentTile);
                        currentTile = loadTile(renderer, AQUA, BAR, NULL, debugLog);
                        if(currentTile == NULL) {
                            logToStream(errorlog, "Error loading tile");
                            continue;
                        }
                        else loadTileIntoGrid(tetrisGrid, currentTile);
                    }
                    else if(key == SDLK_2) {
                        freeTile(currentTile);
                        currentTile = loadTile(renderer, BLUE, J, NULL, debugLog);
                        if(currentTile == NULL) {
                            logToStream(errorlog, "Error loading tile");
                            continue;
                        }
                        else loadTileIntoGrid(tetrisGrid, currentTile);
                    }
                    else if(key == SDLK_3) {
                        freeTile(currentTile);
                        currentTile = loadTile(renderer, GREEN, L, NULL, debugLog);
                        if(currentTile == NULL) {
                            logToStream(errorlog, "Error loading tile");
                            continue;
                        }
                        else loadTileIntoGrid(tetrisGrid, currentTile);
                    }
                    else if(key == SDLK_4) {
                        freeTile(currentTile);
                        currentTile = loadTile(renderer, MAGENTA, S, NULL, debugLog);
                        if(currentTile == NULL) {
                            logToStream(errorlog, "Error loading tile");
                            continue;
                        }
                        else loadTileIntoGrid(tetrisGrid, currentTile);
                    }
                    else if(key == SDLK_5) {
                        freeTile(currentTile);
                        currentTile = loadTile(renderer, ORANGE, SQUARE, NULL, debugLog);
                        if(currentTile == NULL) {
                            logToStream(errorlog, "Error loading tile");
                            continue;
                        }
                        else loadTileIntoGrid(tetrisGrid, currentTile);
                    }
                    else if(key == SDLK_6) {
                        freeTile(currentTile);
                        currentTile = loadTile(renderer, RED, T, NULL, debugLog);
                        if(currentTile == NULL) {
                            logToStream(errorlog, "Error loading tile");
                            continue;
                        }
                        else loadTileIntoGrid(tetrisGrid, currentTile);
                    }
                    else if(key == SDLK_7) {
                        freeTile(currentTile);
                        currentTile = loadTile(renderer, YELLOW, Z, NULL, debugLog);
                        if(currentTile == NULL) {
                            logToStream(errorlog, "Error loading tile");
                            continue;
                        }
                        else loadTileIntoGrid(tetrisGrid, currentTile);
                    }
                    else if(key == SDLK_ESCAPE) {
                        setMatrix(tetrisGrid, programParameters->tetrisGridSize, 0);
                    }
                    #endif
                    break;
                }
                case SDL_KEYUP: {
                    if(event.key.keysym.sym == programParameters->keymap.dropSoft) {
                        speed = NORMAL;
                    }
                    if(event.key.keysym.sym == programParameters->keymap.hold) {
                        speed = NORMAL;
                    }
                    break;
                }
            }

        }
        //long long baseFallSpeed = (long long)programParameters->baseFallSpeed * (frequency.QuadPart / 1000);
        long long baseFallSpeed = programParameters->baseFallSpeed * (frequency.QuadPart / 1000);
        if(speed == DROPSOFT)baseFallSpeed /= 5;
        else if(speed == HOLD)baseFallSpeed *= 5;
        tickTimerEnd = timer.QuadPart;

        if(tickTimerEnd - tickTimerStart > baseFallSpeed) {
            if(currentTile != NULL) {
                status_t moveStatus = moveDown(tetrisGrid, currentTile, programParameters->tetrisGridSize.height);
                if(moveStatus == FAILURE) {
                    currentTile->position.x = -1;
                    currentTile->position.y = -1;
                    absMatrix(tetrisGrid, programParameters->tetrisGridSize);
                    freeTile(currentTile);
                    currentTile = loadTileRandom(renderer, NULL, debugLog);
                    if(currentTile == NULL) {
                        logToStream(errorlog, "Error loading tile");
                    }
                    else {
                        if(loadTileIntoGrid(tetrisGrid, currentTile) == FAILURE) { //TODO: expand functionality on end of the game
                            freeTile(currentTile);
                            currentTile = NULL;
                            setMatrix(tetrisGrid, programParameters->tetrisGridSize, 0);
                            printMatrix(tetrisGrid, programParameters->tetrisGridSize, debugLog);
                            printTile(currentTile, debugLog);
                        }
                    }
                    speed = NORMAL;
                }
            }
            tickTimerStart += baseFallSpeed;
        }

        ReleaseMutex(tilesMutex);
        Sleep(1);
    }



    exit_start: 
        timeEndPeriod(1);
        logToStream(generallog, "Exiting...");

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

    freeConfig:
        freeProgramConfig(programParameters);

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