#include <stdlib.h>
#include <string.h>
#include <SDL.h>
#include <SDL_image.h>

#include "structs_unions.h"

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