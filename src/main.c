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
#include "render.h"
#include "structs_unions.h"
#include "utils.h"

int main(int argc, char** argv) {

    timeBeginPeriod(1);

    LARGE_INTEGER timerStart, timerEnd, frequency;
    QueryPerformanceFrequency(&frequency);
    
    status_t status = SUCCESS; //for controlling exits
    char errormsgBuffer[128] = {0};

    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Texture* texture = NULL;
    
    FILE* generallog = fopen("./log/general.log", "a");
    if(generallog == NULL) {
        status = FAILURE;
        goto exit;
    }
    
    FILE* errorlog = fopen("./log/error.log", "a");
    if(errorlog == NULL) {
        status = FAILURE;
        goto close_generallog;
    }

    fprintf(generallog, "\n\nWelcome to <the name has not been yet initialized, but we'll get there soon enough...>!\n\n\n");
    logToStream(generallog, "Successfully started!");

    FILE* configFile = fopen("./config/config.cfg", "r");
    if(configFile == NULL) {
        status = FAILURE;
        sprintf(errormsgBuffer, "Error opening config file");
        logToStream(errorlog, errormsgBuffer);
        goto close_errorlog;
    }

    ProgramParameters* programParameters = loadConfig(configFile, NULL);
    if(programParameters == NULL) {
        status = FAILURE;
        sprintf(errormsgBuffer, "Error loading game config");
        logToStream(errorlog, errormsgBuffer);
        goto close_configfile;
    }

    logToStream(generallog, "Loaded configuration file.");

    if(SDL_Init(SDL_INIT_EVERYTHING) != SUCCESS) {
        status = FAILURE;
        sprintf(errormsgBuffer, "Error initializing SDL: %s", SDL_GetError());
        logToStream(errorlog, errormsgBuffer);
        goto freeConfig;
    }

    logToStream(generallog, "Initialized SDL.");

    int img_flags = IMG_INIT_PNG | IMG_INIT_JPG;
    if(!(IMG_Init(img_flags) & img_flags)) {
        status = FAILURE;
        sprintf(errormsgBuffer, "Error initializing SDL_image: %s", IMG_GetError());
        logToStream(errorlog, errormsgBuffer);
        goto sdl_quit;
    }

    logToStream(generallog, "Attempting to create a window...");
    
    window = SDL_CreateWindow("Title", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, programParameters->screen_width, programParameters->screen_height, SDL_WINDOW_SHOWN);
    
    if(window == NULL) {
        status = FAILURE;
        sprintf(errormsgBuffer, "Error creating window: %s", SDL_GetError());
        logToStream(errorlog, errormsgBuffer);
        goto img_quit;
    }
    logToStream(generallog, "Window successfully created!");

    /* logToStream(generallog, "Attempting to create a window surface...");

    SDL_Surface* windowSurface = SDL_GetWindowSurface(window);

    if(windowSurface == NULL) {
        status = FAILURE;
        sprintf(errormsgBuffer, "Error initializing surface: %s", SDL_GetError());
        logToStream(errorlog, errormsgBuffer);
        goto sdl_destroywindow;
    }

    logToStream(generallog, "Surface successfully created!");

    SDL_Surface* imageSurface = SDL_LoadBMP("test.bmp");

    logToStream(generallog, "Attempting to create an image surface...");

    if(imageSurface == NULL) {
        status = FAILURE;
        sprintf(errormsgBuffer, "Error initializing surface: %s", SDL_GetError());
        logToStream(errorlog, errormsgBuffer);
        goto sdl_freesurface_main;
    }

    logToStream(generallog, "Surface successfully created!"); */

    logToStream(generallog, "Attempting to create a renderer...");

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if(renderer == NULL) {
        status = FAILURE;
        sprintf(errormsgBuffer, "Error initializing renderer: %s", SDL_GetError());
        logToStream(errorlog, errormsgBuffer);
        goto sdl_destroywindow;
    }

    logToStream(generallog, "Renderer successfully created!");

    SDL_Rect rect;
    SDL_Rect** rects = malloc(sizeof(SDL_Rect*));
    if(rects == NULL) {
        status = FAILURE;
        goto sdl_destroywindow;
    }
    rect.x = 0;
    rect.y = 0;
    rects[0] = &rect;

    texture = loadTextureRect("whatever.png", renderer, &rect);

    if(texture == NULL) {
        status = FAILURE;
        sprintf(errormsgBuffer, "Error initializing texture: %s", SDL_GetError());
        logToStream(errorlog, errormsgBuffer);
        goto sdl_destroyrenderer;
    }

    HANDLE rectMutex = CreateMutex(NULL, FALSE, NULL);
    if(rectMutex == NULL) {
        status = FAILURE;
        sprintf(errormsgBuffer, "Error initializing rect mutex: %ld", GetLastError());
        logToStream(errorlog, errormsgBuffer);
        goto sdl_destroytexture;
    }



    //start of render thread //
    logToStream(generallog, "Attempting to create a render thread mutex...");
    HANDLE renderMutex = CreateMutex(NULL, TRUE, NULL);
    if(renderMutex == NULL) {
        status = FAILURE;
        sprintf(errormsgBuffer, "Error initializing render thread mutex: %ld", GetLastError());
        logToStream(errorlog, errormsgBuffer);
        goto closeRectMutex;
    }

    logToStream(generallog, "Render thread mutex successfully created!");

    loopStatus_t renderStatus = CONTINUE;
    renderThreadParameters renderParameters = {
        &renderStatus, 
        renderMutex, 
        rectMutex, 
        renderer, 
        texture, 
        rects
    };

    HANDLE renderThread = CreateThread(NULL, 0, renderScreen, &renderParameters, 0, NULL);
    if(renderThread == NULL) {
        status = FAILURE;
        sprintf(errormsgBuffer, "Error initializing render thread: %ld", GetLastError());
        logToStream(errorlog, errormsgBuffer);
        goto closeRenderMutex;
    }
    logToStream(generallog, "Render thread is now operational.");
    //end of render thread //



    /* //start of input thread //
    logToStream(generallog, "Attempting to create an input thread mutex...");
    HANDLE inputMutex = CreateMutex(NULL, TRUE, NULL);
    if(inputMutex == NULL) {
        status = FAILURE;
        sprintf(errormsgBuffer, "Error initializing input thread mutex: %ld", GetLastError());
        logToStream(errorlog, errormsgBuffer);
        goto closeRenderThread;
    }

    logToStream(generallog, "Input thread mutex successfully created!");

    loopStatus_t inputStatus = CONTINUE;
    inputThreadParameters inputParameters = {
        &inputStatus, 
        inputMutex, 
        rectMutex, 
        programParameters, 
        rects
    };

    logToStream(generallog, "Attempting to spawn an input thread...");

    HANDLE inputThread = CreateThread(NULL, 0, inputLoop, &inputParameters, 0, NULL);
    if(inputThread == NULL) {
        status = FAILURE;
        sprintf(errormsgBuffer, "Error initializing input thread: %ld", GetLastError());
        logToStream(errorlog, errormsgBuffer);
        goto closeInputMutex;
    }
    logToStream(generallog, "Input thread is now operational.");
    //end of input thread //
 */
    

    long long delta = 0; //time needed to process logic

    bool running = true;
    SDL_Event event;
    ReleaseMutex(renderMutex); //start render thread
    //ReleaseMutex(inputMutex);  //start input thread
    while(running) { //main game loop
        QueryPerformanceCounter(&timerStart);
        WaitForSingleObject(renderMutex, INFINITE);
        /* WaitForSingleObject(inputMutex, INFINITE);
        if(inputStatus == STOP) {
            running = false;
            renderStatus = STOP;
            WaitForSingleObject(renderThread, INFINITE);
            goto exit_start;
        } */
        //ReleaseMutex(inputMutex);

        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                running = false;
                renderStatus = STOP;
                ReleaseMutex(renderMutex);
                WaitForSingleObject(renderThread, INFINITE);
                goto exit_start;
            }
            else if(event.type == SDL_KEYDOWN) { 
                switch(event.key.keysym.sym) {
                    case SDLK_UP:
                        if(rect.y > 10)rect.y -= 10;
                        break;
                    case SDLK_DOWN:
                        if(rect.y < programParameters->screen_height)rect.y += 10;
                        break;
                    case SDLK_LEFT:
                        if(rect.x > 10)rect.x -= 10;
                        break;
                    case SDLK_RIGHT:
                        if(rect.x < programParameters->screen_width)rect.x += 10;
                        break;
                }
            }
        }
        
        //SDL_BlitSurface(imageSurface, NULL, windowSurface, NULL);
        // SDL_UpdateWindowSurface(window);
        
        //formula: [ticks per second (probably 10^7) / FPS - time elapsed for input processing] 
        delta = frequency.QuadPart / programParameters->fps - (timerEnd.QuadPart - timerStart.QuadPart);

        delta = delta * 1000 / frequency.QuadPart; //* 1000 / ticks per second (to get value in miliseconds)
        while(true) {
            QueryPerformanceCounter(&timerEnd);
            if(timerEnd.QuadPart - timerStart.QuadPart > frequency.QuadPart / (programParameters->fps + 2))break;
            //there is an offset of 2 to account for system interrupts
            //looks cursed, is cursed, works though lmao
            Sleep(1);
        } //anyway, I'm gonna use VSync instead of this atrocity
        //UPDATE: no, I'm not gonna use VSync, screw it

        ReleaseMutex(renderMutex); //allow for rendering
    }


    exit_start: 
        timeEndPeriod(1);
        logToStream(generallog, "Exiting...");

    //sdl_freesurface_image: SDL_FreeSurface(imageSurface);

    //sdl_freesurface_main: SDL_FreeSurface(windowSurface);
    //closeInputThread: CloseHandle(inputThread);

    //closeInputMutex: CloseHandle(inputMutex);

    closeRenderThread: CloseHandle(renderThread);

    closeRenderMutex: CloseHandle(renderMutex);

    closeRectMutex: CloseHandle(rectMutex);

    sdl_destroytexture: SDL_DestroyTexture(texture);

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
        if(status == SUCCESS) return EXIT_SUCCESS;
        else return EXIT_FAILURE;
}