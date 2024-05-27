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
void setMatrix(int** matrix, Size size, char value);

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