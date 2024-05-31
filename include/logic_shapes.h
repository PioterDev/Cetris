#ifndef LOGIC_SHAPES_H
#define LOGIC_SHAPES_H


#include "deus.h"

/**
 * @brief Sets the appropiate tile positions that it should occupy for a given tile state. 
 * Used in conjunction with `checkPositions`.
 * 
 * @param state tile state
 * @param position tile position, as per convention
 * @param positions preallocated array of Point structs, its length has to be set appropiately based on the tile size
 * @return SUCCESS on successfully setting positions, FAILURE on failure
 */
status_t getPositions(TileState state, Point position, Point* positions);

/**
 * @brief Checks if given positions are unoccupied.
 * Used in conjunction with `getPositions`.
 * 
 * @param tetrisGrid game matrix
 * @param positions preallocated array of Point structs, its positions can't be negative.
 * @param n number of points to check
 * @return SUCCESS on all free, FAILURE on failure
 */
status_t checkPositions(int** tetrisGrid, Point* positions, int n);

/**
 * @brief Sets the given position to the given value.
 * 
 * @param tetrisGrid game matrix
 * @param positions preallocated array of Point structs, its positions can't be negative
 * @param n number of points
 * @param value value to set
 */
void setPositions(int** tetrisGrid, Point* positions, int n, int value);

/**
 * @brief Returns how much space does the tile occupy in a game matrix.
 * 
 * @param state tile state
 * @return amount of cells needed, -1 on error
 */
int getOccupiedAmount(TileState state);


#endif