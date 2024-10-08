#include <stdlib.h>
#include <string.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include "deus.h"

static const char digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

status_t itos(int in, const unsigned int base, char* buf, const size_t bufsize) {
    if(base > sizeof(digits) - 1) return BASEOUTOFRANGE;

    char tmp[64] = {0};
    size_t i = 0, sign = 0;
    if(in == 0) {
        tmp[i] = '0';
        i++;
        strncpy(buf, tmp, i);
        return SUCCESS;
    }
    if(in < 0) {
        tmp[0] = '-';
        i++; sign++; 
    }
    int len = -1, incpy = in;
    while(incpy != 0) {
        len++;
        incpy /= base;
    }
    in = abs(in);
    while(in != 0) {
        if(i == bufsize - 1) return FAILURE;
        char digit = in % base;
        tmp[sign + len] = digits[(int)digit];
        i++; len--;
        in /= base;
    }
    strncpy(buf, tmp, i);
    return SUCCESS;
}

SDL_Texture* loadTexture(const char* path, SDL_Renderer* renderer) {
    SDL_Texture* tex;
    SDL_Surface* surface = IMG_Load(path);
    if(surface == NULL) return NULL;
    
    tex = SDL_CreateTextureFromSurface(renderer, surface);
    if(tex == NULL) return NULL;

    SDL_FreeSurface(surface);

    return tex;
}

SDL_Texture* loadTextureRect(const char* path, SDL_Renderer* renderer, SDL_Rect* rect) {
    SDL_Texture* tex;
    SDL_Surface* surface = IMG_Load(path);

    if(surface == NULL) return NULL;
    
    tex = SDL_CreateTextureFromSurface(renderer, surface);
    if(tex == NULL) return NULL;

    rect->w = surface->w;
    rect->h = surface->h;
    
    SDL_FreeSurface(surface);

    return tex;
}



Mix_Music* loadMusic(const char* path) {
    return Mix_LoadMUS(path);
}

void freeMusic(Mix_Music* music) {
    Mix_FreeMusic(music);
}

void toggleMusic(int playing) {
    if(playing) Mix_PauseMusic();
    else        Mix_ResumeMusic();
}

void stopMusic() {
    Mix_HaltMusic();
}

void playMusic(ProgramParameters* parameters) {
    if(parameters->flags.soundtrack == 3) {
        if     (parameters->flags.soundtrackNowPlaying == 2) parameters->flags.soundtrackNowPlaying = 0; //has to be
        else if(parameters->flags.soundtrackNowPlaying == 1) parameters->flags.soundtrackNowPlaying = 2; //like this, otherwise
        else if(parameters->flags.soundtrackNowPlaying == 0) parameters->flags.soundtrackNowPlaying = 1; //it crashes
        Mix_PlayMusic(parameters->soundtracks[parameters->flags.soundtrackNowPlaying].music, 0);
    }
    else {
        int index;
        if     (parameters->flags.soundtrack == 0) index = 0; //has to be
        else if(parameters->flags.soundtrack == 1) index = 1; //like this, otherwise
        else if(parameters->flags.soundtrack == 2) index = 2; //it crashes
        else return; //impossible, yet somehow it could happen
        Mix_PlayMusic(parameters->soundtracks[index].music, 0);
        parameters->flags.soundtrackNowPlaying = parameters->flags.soundtrack;
    }
    Mix_VolumeMusic(parameters->soundtracksVolume);
}



Mix_Chunk* loadSound(const char* path) {
    return Mix_LoadWAV(path);
}

void freeSound(Mix_Chunk* chunk) {
    Mix_FreeChunk(chunk);
}

void stopSound(const int channel) {
    Mix_HaltChannel(channel);
}

void playSound(SoundEffect* soundEffect, const unsigned short volume) {
    Mix_PlayChannel(-1, soundEffect->sound, 0);
    Mix_Volume(-1, volume);
}

void togglePause(ProgramParameters* parameters) {
    toggleMusic(!parameters->flags.paused);
    if(parameters->flags.paused) {
        parameters->flags.paused = false;
        /* for(int i = 0; i < tileColorAmount; i++) {
            SDL_SetTextureAlphaMod(parameters->baseTextures[i], 255);
        } */
    }
    else {
        parameters->flags.paused = true;
        /* for(int i = 0; i < tileColorAmount; i++) {
            SDL_SetTextureAlphaMod(parameters->baseTextures[i], 50);
        } */
    }
}

int** zeroMatrix(const Size size) {
    int** matrix = calloc(size.height, sizeof(int*));
    if(matrix == NULL) return NULL;

    for(unsigned int i = 0; i < size.height; i++) {
        matrix[i] = calloc(size.width, sizeof(int));
        if(matrix[i] == NULL) {
            for(unsigned int j = 0; j < i; j++) {
                free(matrix[j]);
            }
            free(matrix);
            return NULL;
        }
    }

    return matrix;
}

void freeMatrix(int** matrix, const int height) {
    for(int i = 0; i < height; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

void printMatrix(int** matrix, const Size size, FILE* stream) {
    for(unsigned int i = 0; i < size.height; i++) {
        for(unsigned int j = 0; j < size.width; j++) {
            if(matrix[i][j] < 0) fprintf(stream, "%d ", matrix[i][j]);
            else fprintf(stream, " %d ", matrix[i][j]);
        }
        fprintf(stream, "\n");
    }
}

void setMatrix(int** matrix, const Size size, const int value) {
    for(unsigned int i = 0; i < size.height; i++) {
        for(unsigned int j = 0; j < size.width; j++) {
            matrix[i][j] = value;
        }
    }
}

void absMatrix(int** matrix, const Size size) {
    for(unsigned int i = 0; i < size.height; i++) {
        for(unsigned int j = 0; j < size.width; j++) {
            matrix[i][j] = abs(matrix[i][j]);
        }
    }
}

int MaxIndex(const int* arr, const int n) {
    int maxIndex = 0;
    for(int i = 1; i < n; i++) {
        if(arr[i] > arr[maxIndex]) maxIndex = i;
    }

    return maxIndex;
}

int MinIndex(const int* arr, const int n) {
    int minIndex = 0;
    for(int i = 1; i < n; i++) {
        if(arr[i] < arr[minIndex]) minIndex = i;
    }

    return minIndex;
}

int Max(const int* arr, const int n) {
    int max = arr[0];
    for(int i = 1; i < n; i++) {
        if(arr[i] > max) max = arr[i];
    }

    return max;
}

int Min(const int* arr, const int n) {
    int min = arr[0];
    for(int i = 1; i < n; i++) {
        if(arr[i] < min) min = arr[i];
    }

    return min;
}