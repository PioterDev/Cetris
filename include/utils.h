#ifndef UTILS_H
#define UTILS_H


#include <SDL.h>

typedef unsigned long long size_t;

/**
 * @brief Simple enum for a status of an operation. Used in otherwise 'void' functions.
 * Defined in utils.h.
 */
typedef enum status_t {
    SUCCESS,
    FAILURE,
    BASEOUTOFRANGE,
    BUFFERTOOSMALL
} status_t;

/**
 * @brief Converts an integer to a string.
 * 
 * @param in input integer
 * @param base base of conversion, max is 16
 * @param buf buffer and an output string
 * @param bufsize size of a buffer
 * @return Status of the conversion
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


#endif