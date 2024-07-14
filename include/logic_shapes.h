#ifndef LOGIC_SHAPES_H
#define LOGIC_SHAPES_H


#include "deus.h"

//How many cells in a game matrix does a tile
//of given state occupy.
extern const int occupiedAmount[STATE_AMOUNT];

//Base positions, based on the convention described
//in logic.h.
extern const int basePositions[STATE_AMOUNT][maxTileSize][2];


#endif