#include <stdlib.h>
#include <string.h>
#include <SDL.h>
#include <SDL_image.h>

#include "structs_unions_defines.h"

status_t itos(int in, int base, char* buf, size_t bufsize) {
    static char* digits = "0123456789ABCDEF";
    if(base > 16)return BASEOUTOFRANGE;

    char tmp[64] = {0};
    unsigned long long i = 0, sign = 0;
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
        if(i == bufsize - 1)return FAILURE;
        char digit = in % base;
        tmp[sign + len] = digits[digit];
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
    if(tex == NULL)return NULL;

    SDL_FreeSurface(surface);

    return tex;
}

SDL_Texture* loadTextureRect(const char* path, SDL_Renderer* renderer, SDL_Rect* rect) {
    SDL_Texture* tex;
    SDL_Surface* surface = IMG_Load(path);

    if(surface == NULL) return NULL;
    
    tex = SDL_CreateTextureFromSurface(renderer, surface);
    if(tex == NULL)return NULL;

    rect->w = surface->w;
    rect->h = surface->h;
    
    SDL_FreeSurface(surface);

    return tex;
}

char** zeroMatrix(int height, int width) {
    char** matrix = calloc(height, sizeof(char*));
    if(matrix == NULL)return NULL;

    for(int i = 0; i < height; i++) {
        matrix[i] = calloc(width, 1);
        if(matrix[i] == NULL) {
            for(int j = 0; j < i; j++) {
                free(matrix[j]);
            }
            free(matrix);
            return NULL;
        }
    }

    return matrix;
}

void freeMatrix(char** matrix, int height) {
    for(int i = 0; i < height; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

void printMatrix(char** matrix, int height, int width, FILE* stream) {
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            if(matrix[i][j] < 0)fprintf(stream, "%d ", matrix[i][j]);
            else fprintf(stream, " %d ", matrix[i][j]);
        }
        fprintf(stream, "\n");
    }
}

void setMatrix(char** matrix, int height, int width, char value) {
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            matrix[i][j] = value;
        }
    }
}

void absMatrix(char** matrix, int height, int width) {
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            if(matrix[i][j] < 0)matrix[i][j] = abs(matrix[i][j]);
        }
    }
}

int MaxIndex(int* arr, int n) {
    int maxIndex = 0;
    for(int i = 1; i < n; i++) {
        if(arr[i] > arr[maxIndex])maxIndex = i;
    }

    return maxIndex;
}

int MinIndex(int* arr, int n) {
    int minIndex = 0;
    for(int i = 1; i < n; i++) {
        if(arr[i] < arr[minIndex])minIndex = i;
    }

    return minIndex;
}

int Max(int* arr, int n) {
    int max = arr[0];
    for(int i = 1; i < n; i++) {
        if(arr[i] > max)max = arr[i];
    }

    return max;
}

int Min(int* arr, int n) {
    int min = arr[0];
    for(int i = 1; i < n; i++) {
        if(arr[i] < min)min = arr[i];
    }

    return min;
}