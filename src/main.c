#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <windows.h>
#include <timeapi.h>
#include <SDL.h>
#include <SDL_image.h>

#include "date.h"
#include "logging.h"
#include "utils.h"

int main(int argc, char** argv) {

    timeBeginPeriod(1);

    const long long FPS = 120;
    
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

    if(SDL_Init(SDL_INIT_EVERYTHING) != SUCCESS) {
        status = FAILURE;
        sprintf(errormsgBuffer, "Error initializing SDL: %s", SDL_GetError());
        logToStream(errorlog, errormsgBuffer);
        goto close_errorlog;
    }

    int img_flags = IMG_INIT_PNG | IMG_INIT_JPG;
    if(!(IMG_Init(img_flags) & img_flags)) {
        status = FAILURE;
        sprintf(errormsgBuffer, "Error initializing SDL_image: %s", IMG_GetError());
        logToStream(errorlog, errormsgBuffer);
        goto sdl_quit;
    }

    logToStream(generallog, "Attempting to create a window...");
    
    window = SDL_CreateWindow("Title", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1920, 1080, SDL_WINDOW_SHOWN);
    
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

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if(renderer == NULL) {
        status = FAILURE;
        sprintf(errormsgBuffer, "Error initializing renderer: %s", SDL_GetError());
        logToStream(errorlog, errormsgBuffer);
        goto sdl_destroywindow;
    }

    texture = loadTexture("whatever.png", renderer);

    if(texture == NULL) {
        status = FAILURE;
        sprintf(errormsgBuffer, "Error initializing texture: %s", SDL_GetError());
        logToStream(errorlog, errormsgBuffer);
        goto sdl_destroyrenderer;
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);

    bool running = true;
    SDL_Event event;
    //LARGE_INTEGER timerStart, timerEnd, frequency;
    //QueryPerformanceFrequency(&frequency);

    // long long delta = 0;

    while(running) { //main game loop
        //QueryPerformanceCounter(&timerStart);
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                running = false;
                goto exit_start;
            }

        }
        
        //SDL_BlitSurface(imageSurface, NULL, windowSurface, NULL);
        // SDL_UpdateWindowSurface(window);
        // delta = (frequency.QuadPart / FPS - (timerEnd.QuadPart - timerStart.QuadPart)) * 1000 / frequency.QuadPart;
        /* while(true) {
            QueryPerformanceCounter(&timerEnd);
            if(timerEnd.QuadPart - timerStart.QuadPart > frequency.QuadPart / (FPS - 2))break;
            //there is an offset of 2 to account for system interrupts
            //looks cursed, is cursed, works though lmao
            Sleep(1);
        } */ //anyway, I'm gonna use VSync instead of this atrocity
        
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }


    exit_start: 
        timeEndPeriod(1);
        logToStream(generallog, "Exiting...");

    //sdl_freesurface_image: SDL_FreeSurface(imageSurface);

    //sdl_freesurface_main: SDL_FreeSurface(windowSurface);

    sdl_destroytexture: SDL_DestroyTexture(texture);

    sdl_destroyrenderer: SDL_DestroyRenderer(renderer);

    sdl_destroywindow: SDL_DestroyWindow(window);

    img_quit: IMG_Quit();

    sdl_quit: SDL_Quit();

    close_errorlog: fclose(errorlog);

    close_generallog: 
        logToStream(generallog, "Goodbye!");
        fclose(generallog);

    exit:
        if(status == SUCCESS) return EXIT_SUCCESS;
        else return EXIT_FAILURE;
}