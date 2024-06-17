#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <windows.h>
#include <timeapi.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
// #include <SDL_ttf.h>

#include "clock_thread.h"
#include "config.h"
#include "date.h"
#include "logging.h"
#include "logic.h"
#include "render.h"
#include "deus.h"
#include "tiles.h"
#include "tile_queue.h"
#include "utils.h"

#define TEST

int main(int argc, char** argv) {
    srand(time(NULL));
    timeBeginPeriod(1);

    LARGE_INTEGER timer, frequency;
    size_t tickTimerStart = 0, tickTimerEnd = 0;
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



    logToStream(generallog, LOGLEVEL_INFO, "Successfully started!");



    // start of timer thread //
    logToStream(generallog, LOGLEVEL_INFO, "Attempting to start a high performance clock...");
    loopStatus_t clockStatus = CONTINUE;
    clockThreadParameters clockParameters = {
        &clockStatus,
        &timer
    };



    HANDLE clockThread = CreateThread(NULL, 0, highPerformanceClockThread, &clockParameters, 0, NULL);;
    if(clockThread == NULL) {
        status = THREAD_START_FAILURE;
        sprintf(errormsgBuffer, "Error starting clock: %ld", GetLastError());
        logToStream(errorlog, LOGLEVEL_ERROR, errormsgBuffer);
        goto close_errorlog;
    }
    logToStream(generallog, LOGLEVEL_INFO, "High performance clock started!");
    // end of timer thread //



    FILE* configFile = fopen("./config/config.cfg", "r");
    if(configFile == NULL) {
        status = FILEOPEN_FAILURE;
        sprintf(errormsgBuffer, "Error opening config file");
        logToStream(errorlog, LOGLEVEL_ERROR, errormsgBuffer);
        goto closeClockThread;
    }


    ProgramParameters programParameters = {};
    if(loadConfig(configFile, debugLog, &programParameters) != SUCCESS) {
        status = LOADCONFIG_FAILURE;
        sprintf(errormsgBuffer, "Error loading game config.");
        logToStream(errorlog, LOGLEVEL_ERROR, errormsgBuffer);
        goto close_configfile;
    }
    programParameters.clockFrequency = &frequency;
    programParameters.timer = &timer;
    
    programParameters.generallog = generallog;
    programParameters.errorlog = errorlog;
    programParameters.debugLog = debugLog;

    if(programParameters.gridSize.height == 0)  programParameters.gridSize.height = GridHeight;
    if(programParameters.gridSize.width == 0)   programParameters.gridSize.width = GridWidth;
    if(programParameters.baseFallSpeed == 0)    programParameters.baseFallSpeed = defaultFallSpeed;
    if(programParameters.fps == 0)              programParameters.fps = 60;
    if(programParameters.screenSize.height == 0)programParameters.screenSize.height = 720;
    if(programParameters.screenSize.width == 0) programParameters.screenSize.width = 1280;
    
    #ifdef TEST
    programParameters.keymap.test = SDLK_w;
    #endif
    logToStream(generallog, LOGLEVEL_INFO, "Loaded configuration file.");



    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_AUDIO) != SUCCESS) {
        status = SDL_INIT_FAILURE;
        sprintf(errormsgBuffer, "Error initializing SDL: %s", SDL_GetError());
        logToStream(errorlog, LOGLEVEL_ERROR, errormsgBuffer);
        goto freeConfig;
    }
    logToStream(generallog, LOGLEVEL_INFO, "Initialized SDL.");



    int img_flags = IMG_INIT_PNG | IMG_INIT_JPG;
    if(!(IMG_Init(img_flags) & img_flags)) {
        status = IMG_INIT_FAILURE;
        sprintf(errormsgBuffer, "Error initializing SDL_image: %s", IMG_GetError());
        logToStream(errorlog, LOGLEVEL_ERROR, errormsgBuffer);
        goto sdl_quit;
    }



    if(Mix_OpenAudio(48000, MIX_DEFAULT_FORMAT, 4, 2048) < 0) {
        status = FAILURE; //TODO: add enum for SDL_mixer failure
        sprintf(errormsgBuffer, "Error initializing SDL_mixer: %s", Mix_GetError());
        logToStream(errorlog, LOGLEVEL_ERROR, errormsgBuffer);
        goto img_quit;
    }


    /* if(TTF_Init() != SUCCESS) {
        status = FAILURE;
        sprintf("Error initializing SDL_ttf: %s", TTF_GetError());
        logToStream(errorlog, LOGLEVEL_ERROR, errormsgBuffer);
        goto mix_quit;
    } */

    logToStream(generallog, LOGLEVEL_INFO, "Attempting to create a window...");
    window = SDL_CreateWindow("Cetris", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, programParameters.screenSize.width, programParameters.screenSize.height, SDL_WINDOW_SHOWN);
    if(window == NULL) {
        status = SDL_WINDOW_FAILURE;
        sprintf(errormsgBuffer, "Error creating window: %s", SDL_GetError());
        logToStream(errorlog, LOGLEVEL_ERROR, errormsgBuffer);
        goto mix_quit;
    }
    logToStream(generallog, LOGLEVEL_INFO, "Window successfully created!");



    logToStream(generallog, LOGLEVEL_INFO, "Attempting to create a renderer...");
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(renderer == NULL) {
        status = SDL_RENDERER_FAILURE;
        sprintf(errormsgBuffer, "Error initializing renderer: %s", SDL_GetError());
        logToStream(errorlog, LOGLEVEL_ERROR, errormsgBuffer);
        goto sdl_destroywindow;
    }
    logToStream(generallog, LOGLEVEL_INFO, "Renderer successfully created!");



    logToStream(generallog, LOGLEVEL_INFO, "Attempting to load base tile textures...");
    if(loadBaseTextures(&programParameters, renderer) != SUCCESS) {
        status = FAILURE;
        sprintf(errormsgBuffer, "Error loading base tile textures.");
        logToStream(errorlog, LOGLEVEL_ERROR, errormsgBuffer);
        goto sdl_destroyrenderer;
    }
    logToStream(generallog, LOGLEVEL_INFO, "Base tile textures successfully loaded!");


    logToStream(generallog, LOGLEVEL_INFO, "Attempting to load digits...");
    if(loadDigits(&programParameters, renderer) != SUCCESS) {
        status = FAILURE;
        sprintf(errormsgBuffer, "Error loading digit textures.");
        logToStream(errorlog, LOGLEVEL_ERROR, errormsgBuffer);
        goto sdl_destroyrenderer;
    }


    logToStream(generallog, LOGLEVEL_INFO, "Attempting to load a soundtrack...");
    if(loadSoundtracks(&programParameters) != SUCCESS) {
        status = FAILURE;
        sprintf(errormsgBuffer, "Error loading soundtrack.");
        logToStream(errorlog, LOGLEVEL_ERROR, errormsgBuffer);
        goto sdl_destroyrenderer;
    }
    logToStream(generallog, LOGLEVEL_INFO, "Soundtrack successfully loaded!");


    logToStream(generallog, LOGLEVEL_INFO, "Attempting to load sound effects...");
    if(loadSoundEffects(&programParameters) != SUCCESS) {
        status = FAILURE;
        sprintf(errormsgBuffer, "Error loading sound effects.");
        logToStream(errorlog, LOGLEVEL_ERROR, errormsgBuffer);
        goto sdl_destroyrenderer;
    }
    logToStream(generallog, LOGLEVEL_INFO, "Sound effects successfully loaded!");



    //dynamic calculation of by how much should everything be scaled
    if( programParameters.gridSize.height * programParameters.baseTileSize > programParameters.screenSize.height || 
        programParameters.gridSize.width * programParameters.baseTileSize > programParameters.screenSize.width) { //scale down
        programParameters.scalingFactor = -1;
        while(
            programParameters.gridSize.height * programParameters.baseTileSize / abs(programParameters.scalingFactor) > programParameters.screenSize.height ||
            programParameters.gridSize.width * programParameters.baseTileSize / abs(programParameters.scalingFactor) > programParameters.screenSize.width
        ) {programParameters.scalingFactor--;}
    }
    else if(programParameters.gridSize.height * programParameters.baseTileSize * 2 < programParameters.screenSize.height || 
            programParameters.gridSize.width  * programParameters.baseTileSize * 2 < programParameters.screenSize.width) { //scale up
        programParameters.scalingFactor = 1;
        while(
            programParameters.gridSize.height * programParameters.baseTileSize * 2 *(unsigned short)programParameters.scalingFactor < programParameters.screenSize.height &&
            programParameters.gridSize.width  * programParameters.baseTileSize * 2 *(unsigned short)programParameters.scalingFactor < programParameters.screenSize.width
        ) {programParameters.scalingFactor++;}
    }


    logToStream(generallog, LOGLEVEL_INFO, "Attempting to create a tiles mutex...");
    HANDLE tilesMutex = CreateMutex(NULL, TRUE, NULL);
    if(tilesMutex == NULL) {
        status = MUTEX_FAILURE;
        sprintf(errormsgBuffer, "Error initializing tiles mutex: %ld", GetLastError());
        logToStream(errorlog, LOGLEVEL_ERROR, errormsgBuffer);
        goto sdl_destroyrenderer;
    }
    logToStream(generallog, LOGLEVEL_INFO, "Tiles mutex successfully created!");



    //start of render thread //
    logToStream(generallog, LOGLEVEL_INFO, "Attempting to create a render thread mutex...");
    HANDLE renderMutex = CreateMutex(NULL, TRUE, NULL);
    if(renderMutex == NULL) {
        status = MUTEX_FAILURE;
        sprintf(errormsgBuffer, "Error initializing render thread mutex: %ld", GetLastError());
        logToStream(errorlog, LOGLEVEL_ERROR, errormsgBuffer);
        goto closeTilesMutex;
    }
    logToStream(generallog, LOGLEVEL_INFO, "Render thread mutex successfully created!");

    loopStatus_t renderStatus = CONTINUE;
    
    Color backgroundColor;
    backgroundColor.red = 32;
    backgroundColor.green = 32;
    backgroundColor.blue = 32;
    backgroundColor.alpha = 0xFF;
    
    renderThreadParameters renderParameters = {
        &renderStatus, 
        renderMutex, 
        tilesMutex,
        &programParameters,
        renderer,
        &backgroundColor
    };

    logToStream(generallog, LOGLEVEL_INFO, "Attempting to start render thread...");
    HANDLE renderThread = CreateThread(NULL, 0, renderScreen, &renderParameters, 0, NULL);
    if(renderThread == NULL) {
        status = THREAD_START_FAILURE;
        sprintf(errormsgBuffer, "Error starting render thread: %ld", GetLastError());
        logToStream(errorlog, LOGLEVEL_ERROR, errormsgBuffer);
        goto closeRenderMutex;
    }
    logToStream(generallog, LOGLEVEL_INFO, "Render thread is now operational.");
    //end of render thread //

    printConfig(&programParameters, debugLog);

    programParameters.flags.running = true;
    SDL_Event event;

    ReleaseMutex(tilesMutex);
    ReleaseMutex(renderMutex); //start render thread

    tickTimerStart = tickTimerEnd = timer.QuadPart;

    while(programParameters.flags.running) { //main game loop
        WaitForSingleObject(tilesMutex, INFINITE);
        while(SDL_PollEvent(&event)) {
            switch(event.type) { //this section *can* be optimized and moved into its own function, 
                case SDL_QUIT: { //but for now, input processing will be here
                    if(programParameters.flags.playing) onGameEnd(&programParameters);
                    programParameters.flags.running = false;
                    renderStatus = STOP;

                    ReleaseMutex(tilesMutex);
                    ReleaseMutex(renderMutex);
                    
                    WaitForSingleObject(renderThread, INFINITE);
                    
                    goto exit_start;
                }
                case SDL_KEYDOWN: {
                    SDL_Keycode key = event.key.keysym.sym;

                    if(!programParameters.flags.playing) {
                        if(isFunctionalKey(key)) break;
                        if(onGameStart(&programParameters, renderer) != SUCCESS) {
                            programParameters.flags.playing = false;
                            status = MEMORY_FAILURE;
                            renderStatus = STOP;

                            ReleaseMutex(tilesMutex);
                            ReleaseMutex(renderMutex);
                    
                            WaitForSingleObject(renderThread, INFINITE);
                    
                            goto exit_start;
                        }                  
                        break;
                    }
                
                    if(debugLog != NULL) {
                        snprintf(loggingBuffer, loggingBufferSize, "[Key press] %d", key);
                        logToStream(debugLog, LOGLEVEL_DEBUG, NULL);
                    }

                    if(key == programParameters.keymap.dropHard) {
                        dropHard(&programParameters);
                        onPlacement(&programParameters);
                        
                        freeTile(programParameters.currentTile);
                        dequeueTile(&programParameters.tileQueue, &programParameters.currentTile);

                        Tile* tmp = loadTileRandom(renderer, NULL, programParameters.gridSize.width, TILELOAD_NOTEXTURE, debugLog);
                        if(tmp != NULL) enqueueTile(&programParameters.tileQueue, tmp);
                        
                        if(programParameters.currentTile == NULL) {
                            logToStream(errorlog, LOGLEVEL_ERROR, "Error loading tile");
                        }
                        else if(loadTileIntoGrid(programParameters.grid, programParameters.currentTile) == FAILURE) {
                            onGameEnd(&programParameters);
                            //FIXME: after game end, a tile STILL somehow falls
                        }
                        programParameters.flags.speed = SPEED_NORMAL;
                    }

                    else if(key == programParameters.keymap.movePieceLeft)  moveLeft(programParameters.grid, programParameters.currentTile);

                    else if(key == programParameters.keymap.movePieceRight) moveRight(programParameters.grid, programParameters.currentTile, programParameters.gridSize.width);
                    
                    else if(key == programParameters.keymap.dropSoft) programParameters.flags.speed = SPEED_DROPSOFT;

                    else if(key == programParameters.keymap.rotateClockwise) {
                        rotateClockwise(programParameters.grid, programParameters.currentTile);
                        playSound(&programParameters.soundEffects[ACTION_ROTATECLOCKWISE], programParameters.soundEffectsVolume);
                    }
                    
                    else if(key == programParameters.keymap.rotateCounterClockwise) {
                        rotateCounterClockwise(programParameters.grid, programParameters.currentTile);
                        playSound(&programParameters.soundEffects[ACTION_ROTATECOUNTERCLOCKWISE], programParameters.soundEffectsVolume);
                    }
                        
                    else if(key == programParameters.keymap.hold) programParameters.flags.speed = SPEED_HOLD;

                    break;
                }
                case SDL_KEYUP: {
                    if( event.key.keysym.sym == programParameters.keymap.dropSoft ||
                        event.key.keysym.sym == programParameters.keymap.hold) {
                        programParameters.flags.speed = SPEED_NORMAL;
                    }
                    break;
                }
            }
        }
        if(programParameters.flags.playing) {
            if(!Mix_PlayingMusic()) playMusic(&programParameters);
            size_t baseFallSpeed = programParameters.baseFallSpeed * (frequency.QuadPart / 1000); //relies upon the fact that frequency is 10^7 (almost always will be, almost, always...)
            if(programParameters.flags.speed == SPEED_DROPSOFT)  baseFallSpeed /= 5;
            else if(programParameters.flags.speed == SPEED_HOLD) baseFallSpeed *= 5;
            tickTimerEnd = timer.QuadPart;
            // fprintf(debugLog, "%lld %lld\n", tickTimerEnd, tickTimerStart);
            if(tickTimerEnd - tickTimerStart > baseFallSpeed) {
                if(programParameters.currentTile != NULL) {
                    status_t moveStatus = moveDown(programParameters.grid, programParameters.currentTile, programParameters.gridSize.height);
                    #ifdef DEBUG
                    logToStream(debugLog, LOGLEVEL_DEBUG, "[moveDown] Moving tile down");
                    #endif
                    if(moveStatus == FAILURE) {
                        onPlacement(&programParameters);
                        freeTile(programParameters.currentTile);

                        dequeueTile(&programParameters.tileQueue, &programParameters.currentTile);
                        enqueueTile(&programParameters.tileQueue, loadTileRandom(renderer, NULL, programParameters.gridSize.width, TILELOAD_NOTEXTURE, debugLog));

                        if(programParameters.currentTile == NULL) {
                            logToStream(errorlog, LOGLEVEL_ERROR, "Error loading tile");
                        }
                        else if(loadTileIntoGrid(programParameters.grid, programParameters.currentTile) == FAILURE) { 
                            onGameEnd(&programParameters);
                            //TODO: expand functionality on end of the game
                        }
                        programParameters.flags.speed = SPEED_NORMAL;
                    }
                    else {
                        if     (programParameters.flags.speed == SPEED_DROPSOFT) programParameters.score += POINTS_SOFTDROP;
                        else if(programParameters.flags.speed == SPEED_HOLD)     programParameters.score += POINTS_HOLD;
                    }
                }
                tickTimerStart = timer.QuadPart;
            }
        }
        
        ReleaseMutex(tilesMutex);
        Sleep(1);
    }



    exit_start: 
        timeEndPeriod(1);
        logToStream(generallog, LOGLEVEL_INFO, "Exiting...");

    closeRenderThread:
        if(renderStatus != STOP)renderStatus = STOP;
        WaitForSingleObject(renderThread, INFINITE);
        CloseHandle(renderThread);

    closeRenderMutex: CloseHandle(renderMutex);

    closeTilesMutex: CloseHandle(tilesMutex);

    sdl_destroyrenderer: SDL_DestroyRenderer(renderer);

    sdl_destroywindow: SDL_DestroyWindow(window);

    // ttf_quit: TTF_Quit();

    mix_quit: Mix_Quit();

    img_quit: IMG_Quit();

    sdl_quit: SDL_Quit();

    freeConfig:
        freeProgramConfig(&programParameters);

    close_configfile: fclose(configFile);

    closeClockThread:
        clockStatus = STOP;
        WaitForSingleObject(clockThread, INFINITE);
        CloseHandle(clockThread);

    close_errorlog: fclose(errorlog);

    close_generallog: 
        if(status == SUCCESS) logToStream(generallog, LOGLEVEL_INFO, "Goodbye!");
        fclose(generallog);

    exit:
        #ifdef DEBUG
        fclose(debugLog);
        #endif

        return status;
}