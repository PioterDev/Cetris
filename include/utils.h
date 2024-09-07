#ifndef UTILS_H
#define UTILS_H


#include <windows.h>
#include <SDL.h>
#include <SDL_mixer.h>

#include "deus.h"

#define isFunctionalKey(key) \
    ((key) >= SDLK_LCTRL && (key) <= SDLK_RGUI) || \
    ((key) >= SDLK_AUDIONEXT && (key) <= SDLK_AUDIOMUTE) || \
    ((key) >= SDLK_APPLICATION && (key) <= SDLK_KP_EQUALSAS400) || \
    ((key) >= SDLK_F1 && (key) <= SDLK_F12) || \
    ((key) >= SDLK_BRIGHTNESSDOWN && (key) <= SDLK_APP2) || \
    ((key) == SDLK_TAB) || ((key) == SDLK_ESCAPE) || \
    ((key) == SDLK_RETURN) || ((key) == SDLK_BACKSPACE) || \
    ((key) >= SDLK_PRINTSCREEN && (key) <= SDLK_PAUSE) || \
    ((key) == SDLK_NUMLOCKCLEAR) || ((key) == SDLK_CAPSLOCK)

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
status_t itos(int in, const unsigned int base, char* buf, const size_t bufsize);

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



/**
 * @brief Loads a music file.
 * 
 * @param path path to audio file
 * @return pointer to Mix_Music struct, NULL on error
 */
Mix_Music* loadMusic(const char* path);

/**
 * @brief Frees a music struct.
 * 
 * @param music music
 */
void freeMusic(Mix_Music* music);

/**
 * @brief Toggles the music.
 * 
 * @param playing whether the music is playing or not
 */
void toggleMusic(int playing);

/**
 * @brief Stops the music.
 * 
 */
void stopMusic();

/**
 * @brief Plays music.
 * 
 * @param parameters pointer to ProgramParameters struct
 */
void playMusic(ProgramParameters* parameters);


/**
 * @brief Loads a generic audio file.
 * 
 * @param path path to audio file
 * @return pointer to Mix_Chunk struct, NULL on error 
 */
Mix_Chunk* loadSound(const char* path);

/**
 * @brief Frees a sound struct.
 * 
 * @param chunk chunk
 */
void freeSound(const Mix_Chunk* chunk);

/**
 * @brief Stops the sound in a given channel.
 * 
 * @param channel channel ID
 */
void stopSound(const int channel);

/**
 * @brief Plays a sound effect on any free channel.
 * 
 * @param soundEffect pointer to SoundEffect struct
 */
void playSound(const SoundEffect* soundEffect, const unsigned short volume);



/**
 * @brief Pauses or unpauses the game.
 * 
 * @param parameters pointer to ProgramParameters struct
 */
void togglePause(ProgramParameters* parameters);



/**
 * @brief Creates a matrix filled with 0s with elements of given size.
 * 
 * @param elementSize element size in bytes
 * @param height height of the matrix
 * @param width width of the matrix
 * @return pointer to the matrix, NULL on allocation error
 */
int** zeroMatrix(const Size size);

/**
 * @brief Frees the matrix from memory.
 * 
 * @param matrix matrix
 * @param height height of the matrix
 */
void freeMatrix(int** matrix, const int height);

/**
 * @brief Prints given matrix to given stream.
 * 
 * @param matrix matrix
 * @param size size of the matrix
 * @param stream stream
 */
void printMatrix(int** matrix, const Size size, FILE* stream);

/**
 * @brief Sets all cells in the matrix to given value.
 * 
 * @param matrix matrix
 * @param size size of the matrix
 */
void setMatrix(int** matrix, const Size size, const int value);

/**
 * @brief Replaces all negative values in the matrix to positive ones.
 * 
 * @param matrix matrix
 * @param size size of the matrix
 */
void absMatrix(int** matrix, const Size size);

/**
 * @brief Returns the index of the biggest element in an int array.
 * 
 * @param arr array
 * @param n number of elements
 * @return index to max element
 */
int MaxIndex(const int* arr, const int n);

/**
 * @brief Returns the index of the smallest element in an int array.
 * 
 * @param arr array
 * @param n number of elements
 * @return index to min element
 */
int MinIndex(const int* arr, const int n);

/**
 * @brief Returns the biggest element in an int array.
 * 
 * @param arr array
 * @param n number of elements
 * @return max element 
 */
int Max(const int* arr, const int n);

/**
 * @brief Returns the biggest element in an int array.
 * 
 * @param arr array
 * @param n number of elements
 * @return min element
 */
int Min(const int* arr, const int n);


#endif