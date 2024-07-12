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

#include "config.h"
#include "date.h"
#include "logging.h"
#include "logic.h"
#include "render.h"
#include "deus.h"
#include "tiles.h"
#include "tile_queue.h"
#include "utils.h"

int main(int argc, char** argv) {
    srand(time(NULL));
    timeBeginPeriod(1);

    LARGE_INTEGER frequency, timerStart, timerEnd;
    QueryPerformanceFrequency(&frequency);

    status_t status = SUCCESS; //Exit code
    char errormsgBuffer[128] = {0};

    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    
    FILE* log = fopen("./log/latest.log", "w");
    if(log == NULL) {
        status = FILEOPEN_FAILURE;
        goto exit;
    }
    defaultStream = log;

    logToStream(log, LOGLEVEL_INFO, "Successfully started!");



    FILE* configFile = fopen("./config/config.cfg", "r");
    if(configFile == NULL) {
        status = FILEOPEN_FAILURE;
        snprintf(errormsgBuffer, sizeof(errormsgBuffer), "Error opening config file");
        logToStream(log, LOGLEVEL_ERROR, errormsgBuffer);
        goto close_log;
    }


    ProgramParameters programParameters = {};
    if(loadConfig(configFile, log, &programParameters) != SUCCESS) {
        status = LOADCONFIG_FAILURE;
        snprintf(errormsgBuffer, sizeof(errormsgBuffer), "Error loading game config.");
        logToStream(log, LOGLEVEL_ERROR, errormsgBuffer);
        goto close_configfile;
    }
    programParameters.clockFrequency = &frequency;
    
    programParameters.log = log;
    //Default parameters
    if(programParameters.screenSize.width == 0)  programParameters.screenSize.width = 1280;
    if(programParameters.screenSize.height == 0) programParameters.screenSize.height = 720;
    if(programParameters.fps == 0)               programParameters.fps = 60;
    if(programParameters.baseFallSpeed == 0)     programParameters.baseFallSpeed = defaultFallSpeed;
    if(programParameters.speedMultiplier == 0)   programParameters.speedMultiplier = defaultSpeedMultiplier;
    if(programParameters.gridSize.height == 0)   programParameters.gridSize.height = defaultGridHeight;
    if(programParameters.gridSize.width == 0)    programParameters.gridSize.width = defaultGridWidth;
    
    logToStream(log, LOGLEVEL_INFO, "Loaded configuration file.");



    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_AUDIO) != SUCCESS) {
        status = SDL_INIT_FAILURE;
        snprintf(errormsgBuffer, sizeof(errormsgBuffer), "Error initializing SDL: %s", SDL_GetError());
        logToStream(log, LOGLEVEL_ERROR, errormsgBuffer);
        goto freeConfig;
    }
    logToStream(log, LOGLEVEL_INFO, "Initialized SDL.");



    int img_flags = IMG_INIT_PNG | IMG_INIT_JPG;
    if(!(IMG_Init(img_flags) & img_flags)) {
        status = IMG_INIT_FAILURE;
        snprintf(errormsgBuffer, sizeof(errormsgBuffer), "Error initializing SDL_image: %s", IMG_GetError());
        logToStream(log, LOGLEVEL_ERROR, errormsgBuffer);
        goto sdl_quit;
    }



    if(Mix_OpenAudio(48000, MIX_DEFAULT_FORMAT, 4, 2048) < 0) {
        status = MIX_OPEN_FAILURE;
        snprintf(errormsgBuffer, sizeof(errormsgBuffer), "Error initializing SDL_mixer: %s", Mix_GetError());
        logToStream(log, LOGLEVEL_ERROR, errormsgBuffer);
        goto img_quit;
    }


    /* if(TTF_Init() != SUCCESS) {
        status = FAILURE;
        snprintf("Error initializsizeof(errormsgBuffer), ing SDL_ttf: %s", TTF_GetError());
        logToStream(log, LOGLEVEL_ERROR, errormsgBuffer);
        goto mix_quit;
    } */

    logToStream(log, LOGLEVEL_INFO, "Attempting to create a window...");
    window = SDL_CreateWindow("Cetris", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, programParameters.screenSize.width, programParameters.screenSize.height, SDL_WINDOW_SHOWN);
    if(window == NULL) {
        status = SDL_WINDOW_FAILURE;
        snprintf(errormsgBuffer, sizeof(errormsgBuffer), "Error creating window: %s", SDL_GetError());
        logToStream(log, LOGLEVEL_ERROR, errormsgBuffer);
        goto mix_quit;
    }
    logToStream(log, LOGLEVEL_INFO, "Window successfully created!");



    logToStream(log, LOGLEVEL_INFO, "Attempting to create a renderer...");
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(renderer == NULL) {
        status = SDL_RENDERER_FAILURE;
        snprintf(errormsgBuffer, sizeof(errormsgBuffer), "Error initializing renderer: %s", SDL_GetError());
        logToStream(log, LOGLEVEL_ERROR, errormsgBuffer);
        goto sdl_destroywindow;
    }
    logToStream(log, LOGLEVEL_INFO, "Renderer successfully created!");



    logToStream(log, LOGLEVEL_INFO, "Attempting to load base tile textures...");
    if(loadBaseTextures(&programParameters, renderer) != SUCCESS) {
        status = FAILURE;
        snprintf(errormsgBuffer, sizeof(errormsgBuffer), "Error loading base tile textures.");
        logToStream(log, LOGLEVEL_ERROR, errormsgBuffer);
        goto sdl_destroyrenderer;
    }
    logToStream(log, LOGLEVEL_INFO, "Base tile textures successfully loaded!");


    logToStream(log, LOGLEVEL_INFO, "Attempting to load digits...");
    if(loadDigits(&programParameters, renderer) != SUCCESS) {
        status = FAILURE;
        snprintf(errormsgBuffer, sizeof(errormsgBuffer), "Error loading digit textures.");
        logToStream(log, LOGLEVEL_ERROR, errormsgBuffer);
        goto sdl_destroyrenderer;
    }


    logToStream(log, LOGLEVEL_INFO, "Attempting to load a soundtrack...");
    if(loadSoundtracks(&programParameters) != SUCCESS) {
        status = FAILURE;
        snprintf(errormsgBuffer, sizeof(errormsgBuffer), "Error loading soundtrack.");
        logToStream(log, LOGLEVEL_ERROR, errormsgBuffer);
        goto sdl_destroyrenderer;
    }
    logToStream(log, LOGLEVEL_INFO, "Soundtrack successfully loaded!");


    logToStream(log, LOGLEVEL_INFO, "Attempting to load sound effects...");
    if(loadSoundEffects(&programParameters) != SUCCESS) {
        status = FAILURE;
        snprintf(errormsgBuffer, sizeof(errormsgBuffer), "Error loading sound effects.");
        logToStream(log, LOGLEVEL_ERROR, errormsgBuffer);
        goto sdl_destroyrenderer;
    }
    logToStream(log, LOGLEVEL_INFO, "Sound effects successfully loaded!");



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
            programParameters.gridSize.height * programParameters.baseTileSize * 2 * (unsigned short)programParameters.scalingFactor < programParameters.screenSize.height &&
            programParameters.gridSize.width  * programParameters.baseTileSize * 2 * (unsigned short)programParameters.scalingFactor < programParameters.screenSize.width
        ) {programParameters.scalingFactor++;}
    }


    logToStream(log, LOGLEVEL_INFO, "Attempting to create a tiles mutex...");
    HANDLE tilesMutex = CreateMutex(NULL, TRUE, NULL);
    if(tilesMutex == NULL) {
        status = MUTEX_FAILURE;
        snprintf(errormsgBuffer, sizeof(errormsgBuffer), "Error initializing tiles mutex: %ld", GetLastError());
        logToStream(log, LOGLEVEL_ERROR, errormsgBuffer);
        goto sdl_destroyrenderer;
    }
    logToStream(log, LOGLEVEL_INFO, "Tiles mutex successfully created!");



    //start of render thread //
    logToStream(log, LOGLEVEL_INFO, "Attempting to create a render thread mutex...");
    HANDLE renderMutex = CreateMutex(NULL, TRUE, NULL);
    if(renderMutex == NULL) {
        status = MUTEX_FAILURE;
        snprintf(errormsgBuffer, sizeof(errormsgBuffer), "Error initializing render thread mutex: %ld", GetLastError());
        logToStream(log, LOGLEVEL_ERROR, errormsgBuffer);
        goto closeTilesMutex;
    }
    logToStream(log, LOGLEVEL_INFO, "Render thread mutex successfully created!");

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

    logToStream(log, LOGLEVEL_INFO, "Attempting to start render thread...");
    HANDLE renderThread = CreateThread(NULL, 0, renderScreen, &renderParameters, 0, NULL);
    if(renderThread == NULL) {
        status = THREAD_START_FAILURE;
        snprintf(errormsgBuffer, sizeof(errormsgBuffer), "Error starting render thread: %ld", GetLastError());
        logToStream(log, LOGLEVEL_ERROR, errormsgBuffer);
        goto closeRenderMutex;
    }
    logToStream(log, LOGLEVEL_INFO, "Render thread is now operational.");
    //end of render thread //

    printConfig(&programParameters, log);

    programParameters.flags.running = true;
    SDL_Event event;

    ReleaseMutex(tilesMutex);
    ReleaseMutex(renderMutex); //start render thread

    QueryPerformanceCounter(&timerStart);

    while(programParameters.flags.running) { //main game loop
        WaitForSingleObject(tilesMutex, INFINITE);
        while(SDL_PollEvent(&event)) {
            switch(event.type) { //this section *can* be optimized and moved into its own function, 
                case SDL_QUIT: { //but for now, input processing will be here
                    if(programParameters.flags.playing) onGameEnd(&programParameters, GAME_END_REASON_EXIT);
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

                    if(log != NULL) {
                        snprintf(loggingBuffer, loggingBufferSize, "[Key press] %d", key);
                        logToStream(log, LOGLEVEL_DEBUG, NULL);
                    }

                    if(key == programParameters.keymap.pause) togglePause(&programParameters);

                    else if(programParameters.flags.paused) break;
                
                    else if(key == programParameters.keymap.dropHard) {
                        dropHard(&programParameters);
                        onPlacement(&programParameters);

                        freeTile(programParameters.currentTile);
                        dequeueTile(&programParameters.tileQueue, &programParameters.currentTile);

                        Tile* tmp = loadTileRandom(renderer, NULL, programParameters.gridSize.width, TILELOAD_NOTEXTURE, log);
                        if(tmp != NULL) enqueueTile(&programParameters.tileQueue, tmp);
                        
                        if(programParameters.currentTile == NULL) {
                            logToStream(log, LOGLEVEL_ERROR, "Error loading tile");
                        }
                        else if(loadTileIntoGrid(programParameters.grid, programParameters.currentTile, programParameters.gridSize) == FAILURE) {
                            onGameEnd(&programParameters, GAME_END_REASON_LOADFAIL);
                        }
                        programParameters.flags.tileRecentlyLoaded = true;
                        programParameters.flags.speed = SPEED_NORMAL;
                    }

                    else if(key == programParameters.keymap.movePieceLeft)  {
                        moveLeft(programParameters.grid, programParameters.currentTile);
                    }
                    else if(key == programParameters.keymap.movePieceRight) {
                        moveRight(programParameters.grid, programParameters.currentTile, programParameters.gridSize.width);
                    }
                    else if(key == programParameters.keymap.dropSoft) programParameters.flags.speed = SPEED_DROPSOFT;

                    else if(key == programParameters.keymap.rotateClockwise) {
                        rotateClockwise(programParameters.grid, programParameters.currentTile, programParameters.gridSize);
                        playSound(&programParameters.soundEffects[ACTION_ROTATECLOCKWISE], programParameters.soundEffectsVolume);
                    }
                    
                    else if(key == programParameters.keymap.rotateCounterClockwise) {
                        rotateCounterClockwise(programParameters.grid, programParameters.currentTile, programParameters.gridSize);
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
            if(programParameters.flags.paused) goto gameloop_end;
            if(!Mix_PlayingMusic()) playMusic(&programParameters);
            long long baseFallSpeed = programParameters.baseFallSpeed * (frequency.QuadPart / 1000); //relies upon the fact that frequency is 10^7 (almost always will be, almost, always...)
            if(programParameters.flags.speed == SPEED_DROPSOFT)  baseFallSpeed /= programParameters.speedMultiplier;
            else if(programParameters.flags.speed == SPEED_HOLD) baseFallSpeed *= programParameters.speedMultiplier;

            if(programParameters.flags.tileRecentlyLoaded)  {
                QueryPerformanceCounter(&timerStart);
                programParameters.flags.tileRecentlyLoaded = false;
            }
            QueryPerformanceCounter(&timerEnd);
            if(timerEnd.QuadPart - timerStart.QuadPart > baseFallSpeed) {
                if(programParameters.currentTile != NULL) {
                    status_t moveStatus = moveDown(programParameters.grid, programParameters.currentTile, programParameters.gridSize.height);
#ifdef DEBUG
                    logToStream(log, LOGLEVEL_DEBUG, "[moveDown] Moving tile down...");
#endif
                    if(moveStatus == FAILURE) {
#ifdef DEBUG
                        logToStream(log, LOGLEVEL_DEBUG, "[moveDown] Failed to move tile down.");
#endif
                        onPlacement(&programParameters);
                        freeTile(programParameters.currentTile);

                        dequeueTile(&programParameters.tileQueue, &programParameters.currentTile);
                        enqueueTile(&programParameters.tileQueue, loadTileRandom(renderer, NULL, programParameters.gridSize.width, TILELOAD_NOTEXTURE, log));

                        if(programParameters.currentTile == NULL) {
                            logToStream(log, LOGLEVEL_ERROR, "Error loading tile");
                        }
                        else if(loadTileIntoGrid(programParameters.grid, programParameters.currentTile, programParameters.gridSize) == FAILURE) { 
                            onGameEnd(&programParameters, GAME_END_REASON_LOADFAIL);
                            //TODO: expand functionality on end of the game
                        }
                        programParameters.flags.tileRecentlyLoaded = true;
                        programParameters.flags.speed = SPEED_NORMAL;
                    }
                    else {
                        if     (programParameters.flags.speed == SPEED_DROPSOFT) programParameters.score += POINTS_SOFTDROP;
                        else if(programParameters.flags.speed == SPEED_HOLD)     programParameters.score += POINTS_HOLD;
                    }
                }
                QueryPerformanceCounter(&timerStart);
            }
        }
        gameloop_end:
            ReleaseMutex(tilesMutex);
            if(programParameters.flags.paused) Sleep(10);
            else Sleep(1);
    }



    exit_start: 
        timeEndPeriod(1);
        logToStream(log, LOGLEVEL_INFO, "Exiting...");

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

    close_log:
        if(status == SUCCESS) logToStream(log, LOGLEVEL_INFO, "Goodbye!");
        fclose(log);
    exit:
        return status;
}