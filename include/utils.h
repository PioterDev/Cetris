#ifndef UTILS_H
#define UTILS_H


#include <windows.h>
#include <SDL.h>

#include "deus.h"

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
 * @brief Stops the music.
 * 
 */
void stopMusic();

/**
 * @brief Plays music.
 * 
 * @param soundtrack pointer to Soundtrack struct
 */
void playMusic(Soundtrack* soundtrack);


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
void freeSound(Mix_Chunk* chunk);

/**
 * @brief Stops the sound in a given channel.
 * 
 * @param channel channel ID
 */
void stopSound(int channel);

/**
 * @brief Plays a sound effect on any free channel.
 * 
 * @param soundEffect pointer to SoundEffect struct
 */
void playSound(SoundEffect* soundEffect, unsigned short volume);




/**
 * @brief Creates a matrix filled with 0s with elements of given size.
 * 
 * @param elementSize element size in bytes
 * @param height height of the matrix
 * @param width width of the matrix
 * @return pointer to the matrix, NULL on allocation error
 */
int** zeroMatrix(Size size);

/**
 * @brief Frees the matrix from memory.
 * 
 * @param matrix matrix
 * @param height height of the matrix
 */
void freeMatrix(int** matrix, int height);

/**
 * @brief Prints given matrix to given stream.
 * 
 * @param matrix matrix
 * @param size size of the matrix
 * @param stream stream
 */
void printMatrix(int** matrix, Size size, FILE* stream);

/**
 * @brief Sets all cells in the matrix to given value.
 * 
 * @param matrix matrix
 * @param size size of the matrix
 */
void setMatrix(int** matrix, Size size, int value);

/**
 * @brief Replaces all negative values in the matrix to positive ones.
 * 
 * @param matrix matrix
 * @param size size of the matrix
 */
void absMatrix(int** matrix, Size size);

/**
 * @brief Returns the index of the biggest element in an int array.
 * 
 * @param arr array
 * @param n number of elements
 * @return index to max element
 */
int MaxIndex(int* arr, int n);

/**
 * @brief Returns the index of the smallest element in an int array.
 * 
 * @param arr array
 * @param n number of elements
 * @return index to min element
 */
int MinIndex(int* arr, int n);

/**
 * @brief Returns the biggest element in an int array.
 * 
 * @param arr array
 * @param n number of elements
 * @return max element 
 */
int Max(int* arr, int n);

/**
 * @brief Returns the biggest element in an int array.
 * 
 * @param arr array
 * @param n number of elements
 * @return min element
 */
int Min(int* arr, int n);


#endif