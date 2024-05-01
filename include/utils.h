#ifndef UTILS_H
#define UTILS_H


#include <windows.h>
#include <SDL.h>

#include "structs_unions.h"

/**
 * @brief Converts an integer to a string.
 * 
 * @param in input integer
 * @param base base of conversion, max is 16
 * @param buf buffer and an output string
 * @param bufsize size of a buffer
 * @return Status of the conversion: 
 * BASEOUTOFRANGE if given base is too big, 
 * FAILURE if conversion fails,
 * SUCCESS if conversion succeeds.
 */
status_t itos(int in, int base, char* buf, size_t bufsize);

/**
 * @brief Loads an SDL texture.
 * 
 * @param path path to file with texture
 * @param renderer SDL renderer
 * @return texture, NULL if error
 */
SDL_Texture* loadTexture(const char* path, SDL_Renderer* renderer);

/**
 * @brief Loads an SDL texture and sets rectangle parameters for it.
 * 
 * @param path path to file with texture
 * @param renderer SDL renderer
 * @param rect SDL rectangle
 * @return texture, NULL if error
 */
SDL_Texture* loadTextureRect(const char* path, SDL_Renderer* renderer, SDL_Rect* rect);


#endif