#ifndef LOGIC_H
#define LOGIC_H

/**
 * I HEREBY DECREE THAT:
 * 
 * Position convention is as follows:
 * (x, y) point to left-most square of the current tile.
 * If there are multiple, it points to the top-most one.
 * For T, J and L-shape, it points to the tile that "sticks out". 
 * 
 * In a tiles table, 1st tile is the background tile.
 * 2nd tile is the current falling tile.
 * The rest are queue.
 */

/**
 * @brief Loads the tile, given its "focal point", to the game matrix.
 * 
 * @param tetrisGrid game matrix
 * @param tile tile
 * @return SUCCESS on success, FAILURE on failure
 */
status_t loadTileIntoGrid(char** tetrisGrid, Tile* tile);

/**
 * @brief Moves the tile one square left.
 * 
 * @param tetrisGrid game matrix
 * @param tile tile
 * @return SUCCESS on success, FAILURE on failure
 */
status_t moveLeft(char** tetrisGrid, Tile* tile);

/**
 * @brief Moves the tile one square right.
 * 
 * @param tetrisGrid game matrix
 * @param tile tile
 * @return SUCCESS on success, FAILURE on failure
 */
status_t moveRight(char** tetrisGrid, Tile* tile);

/**
 * @brief Moves the tile one square down.
 * 
 * @param tetrisGrid game matrix
 * @param tile tile
 * @return SUCCESS on success, FAILURE on failure 
 */
status_t moveDown(char** tetrisGrid, Tile* tile);

/**
 * @brief Hard drop functionality. Uses moveDown.
 * 
 * @param tetrisGrid game matrix
 * @param tile tile
 * @return status 
 */
status_t dropHard(char** tetrisGrid, Tile* tile);

/**
 * @brief Hard drop functionality.
 * 
 * @param tetrisGrid game matrix
 * @param tile tile
 * @param tetrisGridHeight height of the game matrix 
 * @return SUCCESS on success, FAILURE on failure
 */
status_t dropHardOld(char** tetrisGrid, Tile* tile, const int tetrisGridHeight, FILE* debug);

/**
 * @brief Rotates piece clockwise.
 * 
 * @param tetrisGrid game matrix
 * @param tile tile
 * @return SUCCESS on success, FAILURE on failure 
 */
status_t rotateClockwise(char** tetrisGrid, Tile* tile);


#endif