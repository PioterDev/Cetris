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
#include "events.h"
#include "logging.h"
#include "logic.h"
#include "render.h"
#include "deus.h"
#include "tiles.h"
#include "tile_queue.h"
#include "utils.h"

int main(__attribute__((unused)) int argc, __attribute__((unused)) char** argv) {
    srand(time(NULL));
    timeBeginPeriod(1);
    //Disables Windows' scale and layout for the process
    SDL_SetHint(SDL_HINT_WINDOWS_DPI_AWARENESS, "permonitorv2");

    LARGE_INTEGER frequency, timerStart, timerEnd;
    QueryPerformanceFrequency(&frequency);

    status_t status = SUCCESS; //Exit code

    SDL_Window* window = NULL;
    
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
        logToStream(log, LOGLEVEL_ERROR, "Error opening config file");
        goto close_log;
    }


    ProgramParameters programParameters = {0};
    if(loadConfig(configFile, log, &programParameters) != SUCCESS) {
        status = LOADCONFIG_FAILURE;
        logToStream(log, LOGLEVEL_ERROR, "Error loading game config.");
        goto close_configfile;
    }
    programParameters.clockFrequency = &frequency;
    
    programParameters.log = log;
    //Default parameters
    if(programParameters.screenSize.width == 0)  programParameters.screenSize.width = 640;
    if(programParameters.screenSize.height == 0) programParameters.screenSize.height = 360;
    if(programParameters.fps == 0)               programParameters.fps = 60;
    if(programParameters.baseFallSpeed == 0)     programParameters.baseFallSpeed = defaultFallSpeed;
    if(programParameters.speedMultiplier == 0)   programParameters.speedMultiplier = defaultSpeedMultiplier;
    if(programParameters.gridSize.height == 0)   programParameters.gridSize.height = defaultGridHeight;
    if(programParameters.gridSize.width == 0)    programParameters.gridSize.width = defaultGridWidth;
    
    logToStream(log, LOGLEVEL_INFO, "Loaded configuration file.");



    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_AUDIO) != SUCCESS) {
        status = SDL_INIT_FAILURE;
        logToStream(
            log, LOGLEVEL_ERROR,
            "Error initializing SDL: %s", SDL_GetError()
        );
        goto freeConfig;
    }
    logToStream(log, LOGLEVEL_INFO, "Initialized SDL.");



    int img_flags = IMG_INIT_PNG | IMG_INIT_JPG;
    if(!(IMG_Init(img_flags) & img_flags)) {
        status = IMG_INIT_FAILURE;
        logToStream(
            log, LOGLEVEL_ERROR,
            "Error initializing SDL_image: %s", IMG_GetError()
        );
        goto sdl_quit;
    }



    if(Mix_OpenAudio(48000, MIX_DEFAULT_FORMAT, 4, 2048) < 0) {
        status = MIX_OPEN_FAILURE;
        logToStream(
            log, LOGLEVEL_ERROR,
            "Error initializing SDL_mixer: %s", Mix_GetError()
        );
        goto img_quit;
    }


    /* if(TTF_Init() != SUCCESS) {
        status = FAILURE;
        snprintf("Error initializsizeof(errormsgBuffer), ing SDL_ttf: %s", TTF_GetError());
        logToStream(log, LOGLEVEL_ERROR, errormsgBuffer);
        goto mix_quit;
    } */

    logToStream(log, LOGLEVEL_INFO, "Attempting to create a window...");
    window = SDL_CreateWindow(
        "Cetris",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        programParameters.screenSize.width,
        programParameters.screenSize.height,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
    );
    if(window == NULL) {
        status = SDL_WINDOW_FAILURE;
        logToStream(
            log, LOGLEVEL_ERROR,
            "Error creating window: %s", SDL_GetError()
        );
        goto mix_quit;
    }
    logToStream(log, LOGLEVEL_INFO, "Window successfully created!");



    logToStream(log, LOGLEVEL_INFO, "Attempting to create a renderer...");
    programParameters.renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(programParameters.renderer == NULL) {
        status = SDL_RENDERER_FAILURE;
        logToStream(
            log, LOGLEVEL_ERROR,
            "Error initializing renderer: %s", SDL_GetError()
        );
        goto sdl_destroywindow;
    }
    logToStream(log, LOGLEVEL_INFO, "Renderer successfully created!");



    logToStream(log, LOGLEVEL_INFO, "Attempting to load base tile textures...");
    if(loadBaseTextures(&programParameters) != SUCCESS) {
        status = FAILURE;
        logToStream(log, LOGLEVEL_ERROR,  "Error loading base tile textures.");
        goto sdl_destroyrenderer;
    }
    logToStream(log, LOGLEVEL_INFO, "Base tile textures successfully loaded!");


    logToStream(log, LOGLEVEL_INFO, "Attempting to load digits...");
    if(loadDigits(&programParameters) != SUCCESS) {
        status = FAILURE;
        logToStream(log, LOGLEVEL_ERROR, "Error loading digit textures.");
        goto sdl_destroyrenderer;
    }


    logToStream(log, LOGLEVEL_INFO, "Attempting to load a soundtrack...");
    if(loadSoundtracks(&programParameters) != SUCCESS) {
        status = FAILURE;
        logToStream(log, LOGLEVEL_ERROR, "Error loading soundtrack.");
        goto sdl_destroyrenderer;
    }
    logToStream(log, LOGLEVEL_INFO, "Soundtrack successfully loaded!");


    logToStream(log, LOGLEVEL_INFO, "Attempting to load sound effects...");
    if(loadSoundEffects(&programParameters) != SUCCESS) {
        status = FAILURE;
        logToStream(log, LOGLEVEL_ERROR, "Error loading sound effects.");
        goto sdl_destroyrenderer;
    }
    logToStream(log, LOGLEVEL_INFO, "Sound effects successfully loaded!");

    calculateScalingFactor(&programParameters);

    logToStream(log, LOGLEVEL_INFO, "Attempting to create a tiles mutex...");
    HANDLE tilesMutex = CreateMutex(NULL, TRUE, NULL);
    if(tilesMutex == NULL) {
        status = MUTEX_FAILURE;
        logToStream(
            log, LOGLEVEL_ERROR,
            "Error initializing tiles mutex: %ld", GetLastError()
        );
        goto sdl_destroyrenderer;
    }
    logToStream(log, LOGLEVEL_INFO, "Tiles mutex successfully created!");



    //start of render thread //
    logToStream(log, LOGLEVEL_INFO, "Attempting to create a render thread mutex...");
    HANDLE renderMutex = CreateMutex(NULL, TRUE, NULL);
    if(renderMutex == NULL) {
        status = MUTEX_FAILURE;
        logToStream(
            log, LOGLEVEL_ERROR,
            "Error initializing render thread mutex: %ld", GetLastError()
        );
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
        &backgroundColor
    };

    logToStream(log, LOGLEVEL_INFO, "Attempting to start render thread...");
    HANDLE renderThread = CreateThread(NULL, 0, renderScreen, &renderParameters, 0, NULL);
    if(renderThread == NULL) {
        status = THREAD_START_FAILURE;
        logToStream(
            log, LOGLEVEL_ERROR,
            "Error starting render thread: %ld", GetLastError()
        );
        goto closeRenderMutex;
    }
    logToStream(log, LOGLEVEL_INFO, "Render thread is now operational.");
    //end of render thread //

#ifdef DEBUG
    printConfig(&programParameters, log);
#endif
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
                    goto gameloop_end;
                }
                case SDL_WINDOWEVENT: {
                    if(event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                        programParameters.screenSize.width = event.window.data1;
                        programParameters.screenSize.height = event.window.data2;
                        onWindowResize(&programParameters, window);
                    }
                    break;
                }
                case SDL_KEYDOWN: {
                    SDL_Keycode key = event.key.keysym.sym;

                    if(!programParameters.flags.playing) {
                        if(isFunctionalKey(key)) break; //this allows for Alt-Tab without starting the game
                        switch(onGameStart(&programParameters)) {
                            case FAILURE:
                                logToStream(log, LOGLEVEL_FATAL, "Failed to start the game - unable to load a tile into the grid.");
                                goto error;
                            case MEMORY_FAILURE:
                                logToStream(log, LOGLEVEL_FATAL, "Failed to start the game - no memory.");
                                goto error;
                            default: break;
                        }
                        break;
                        error:
                            programParameters.flags.running = false;
                            renderStatus = STOP;
                            goto gameloop_end;
                    }

#ifdef DEBUG
                    int keyIndex = getKeystringIndex(key);
                    if(keyIndex == -1) {
                        logToStream(log, LOGLEVEL_DEBUG, "[Key press] %c", key);
                    }
                    else {
                        logToStream(log, LOGLEVEL_DEBUG, "[Key press] %s", keynames[keyIndex]);
                    }
#endif

                    if(key == programParameters.keymap.pause) togglePause(&programParameters);

                    else if(programParameters.flags.paused) break;
                
                    else if(key == programParameters.keymap.dropHard) {
                        dropHard(&programParameters);
                        onPlacement(&programParameters);

                        freeTile(programParameters.currentTile);
                        dequeueTile(&programParameters.tileQueue, &programParameters.currentTile);
                        enqueueTile(&programParameters.tileQueue, loadTileRandom(NULL, programParameters.gridSize.width));
                        
                        if(programParameters.currentTile == NULL) {
                            logToStream(log, LOGLEVEL_FATAL, "Error loading tile");
                            onGameEnd(&programParameters, GAME_END_REASON_NOMEM);
                            programParameters.flags.running = false;
                            goto gameloop_end;
                        }
                        else {
                            switch(loadTileIntoGrid(&programParameters)) {
                                case FAILURE:
                                case INDEXOUTOFRANGE:
                                    onGameEnd(&programParameters, GAME_END_REASON_LOADFAIL);
                                    break;
                                case BASEOUTOFRANGE:
                                    logToStream(
                                        log, LOGLEVEL_WARNING, 
                                        "Tell that dumbass dev to add state values that he clearly forgot lmao"
                                    );
                                    onGameEnd(&programParameters, GAME_END_REASON_YOUIDIOT);
                                    programParameters.flags.running = false;
                                    goto gameloop_end;
                                default: 
                                    break;
                            }
                        }
                        
                        programParameters.flags.speed = SPEED_NORMAL;
                    }

                    else if(key == programParameters.keymap.movePieceLeft)  {
                        moveLeft(programParameters.grid, programParameters.currentTile);
                    }
                    else if(key == programParameters.keymap.movePieceRight) {
                        moveRight(programParameters.grid, programParameters.currentTile, programParameters.gridSize.width);
                    }
                    else if(key == programParameters.keymap.dropSoft) {
                        programParameters.flags.speed = SPEED_DROPSOFT;
                    }
                    else if(key == programParameters.keymap.rotateClockwise) {
                        rotateClockwise(programParameters.grid, programParameters.currentTile, programParameters.gridSize);
                        playSound(&programParameters.soundEffects[ACTION_ROTATECLOCKWISE], programParameters.soundEffectsVolume);
                    }
                    else if(key == programParameters.keymap.rotateCounterClockwise) {
                        rotateCounterClockwise(programParameters.grid, programParameters.currentTile, programParameters.gridSize);
                        playSound(&programParameters.soundEffects[ACTION_ROTATECOUNTERCLOCKWISE], programParameters.soundEffectsVolume);
                    }   
                    else if(key == programParameters.keymap.hold) {
                        onHold(&programParameters);
                        // programParameters.flags.speed = SPEED_HOLD;
                    }
                    break;
                }
                case SDL_KEYUP: {
                    if( event.key.keysym.sym == programParameters.keymap.dropSoft ||
                        event.key.keysym.sym == programParameters.keymap.hold) {
                        programParameters.flags.speed = SPEED_NORMAL;
                    }
                    break;
                }
                /* case SDL_CONTROLLERBUTTONDOWN: {
                    SDL_GameControllerButton button = event.cbutton.button;
                    logToStream(log, LOGLEVEL_DEBUG, "[Controller] %d", button);
                    if(button == SDL_CONTROLLER_BUTTON_LEFTSHOULDER) {
                        moveLeft(programParameters.grid, programParameters.currentTile);
                    }
                    break;
                }
                case SDL_CONTROLLERAXISMOTION: {
                    logToStream(log, LOGLEVEL_DEBUG, "%d", event.caxis.value);
                    break;
                }
                case SDL_JOYAXISMOTION: {
                    fprintf(log, "%d %d\n", event.jaxis.axis, event.jaxis.value);
                    if(event.jaxis.which == 0) {
                        if(event.jaxis.axis == 1) {
                            if(event.jaxis.value > 0) {
                                moveLeft(programParameters.grid, programParameters.currentTile);
                            }
                        }
                        else if(event.jaxis.axis == 2) {
                            if(event.jaxis.value > 0) {
                                moveRight(programParameters.grid, programParameters.currentTile, programParameters.gridSize.width);
                            }
                        }
                    }
                } */
            }
        }
        if(programParameters.flags.playing) {
            if(programParameters.flags.paused) goto gameloop_end;
            if(!Mix_PlayingMusic()) playMusic(&programParameters);
            
            //relies upon the fact that frequency is 10^7 (almost always will be, almost, always...)
            long long baseFallSpeed = programParameters.baseFallSpeed * (frequency.QuadPart / 1000); 
            
            if(programParameters.flags.speed == SPEED_DROPSOFT)  baseFallSpeed /= programParameters.speedMultiplier;
            else if(programParameters.flags.speed == SPEED_HOLD) baseFallSpeed *= programParameters.speedMultiplier;

            if(programParameters.flags.tileRecentlyLoaded)  {
                QueryPerformanceCounter(&timerStart);
                programParameters.flags.tileRecentlyLoaded = false;
            }
            QueryPerformanceCounter(&timerEnd);
            if(timerEnd.QuadPart - timerStart.QuadPart > baseFallSpeed) {
                if(programParameters.currentTile != NULL) {
                    status_t moveStatus = moveDown(
                        programParameters.grid,
                        programParameters.currentTile,
                        programParameters.gridSize.height
                    );
#ifdef DEBUG
                    logToStream(log, LOGLEVEL_DEBUG, "[moveDown] Moving tile down...");
#endif
                    if(moveStatus != SUCCESS) {
#ifdef DEBUG
                        logToStream(log, LOGLEVEL_DEBUG, "[moveDown] Failed to move tile down.");
#endif
                        onPlacement(&programParameters);
                        freeTile(programParameters.currentTile);

                        dequeueTile(&programParameters.tileQueue, &programParameters.currentTile);
                        enqueueTile(&programParameters.tileQueue, loadTileRandom(NULL, programParameters.gridSize.width));

                        if(programParameters.currentTile == NULL) {
                            logToStream(log, LOGLEVEL_ERROR, "Error loading tile");
                            onGameEnd(&programParameters, GAME_END_REASON_NOMEM);
                            programParameters.flags.running = false;
                            goto gameloop_end;
                        }
                        else if(loadTileIntoGrid(&programParameters) == FAILURE) { 
                            onGameEnd(&programParameters, GAME_END_REASON_LOADFAIL);
                            //TODO: expand functionality on end of the game
                        }
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



    timeEndPeriod(1);
    logToStream(log, LOGLEVEL_INFO, "Started the exit sequence...");

    closeRenderThread:
        if(renderStatus != STOP) renderStatus = STOP;
        logToStream(log, LOGLEVEL_INFO, "Closing the render thread...");
        WaitForSingleObject(renderThread, INFINITE);
        CloseHandle(renderThread);

    closeRenderMutex:
        WaitForSingleObject(renderMutex, 1000);
        CloseHandle(renderMutex);

    closeTilesMutex:
        WaitForSingleObject(tilesMutex, 1000);
        CloseHandle(tilesMutex);

    sdl_destroyrenderer:
        SDL_DestroyRenderer(programParameters.renderer);

    sdl_destroywindow:
        SDL_DestroyWindow(window);

    // ttf_quit: TTF_Quit();

    mix_quit: Mix_Quit();

    img_quit: IMG_Quit();

    sdl_quit: SDL_Quit();

    freeConfig:
        freeProgramConfig(&programParameters);

    close_configfile:
        fclose(configFile);

    close_log:
        logToStream(log, LOGLEVEL_INFO, "Exiting with code %d...", status);
        fclose(log);
    exit:
        return status;
}