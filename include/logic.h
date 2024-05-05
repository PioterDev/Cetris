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

#include "logic_movement.h"
#include "logic_other.h"
#include "logic_rotation.h"


#endif