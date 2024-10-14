#include "deus.h"

const int occupiedAmount[STATE_AMOUNT] = {
    4, 4, 4, 4, //BAR
    4, 4, 4, 4, //J
    4, 4, 4, 4, //L
    4, 4, 4, 4, //S
    4, //SQR
    4, 4, 4, 4, //T
    4, 4, 4, 4 //Z
};

const int basePositions[STATE_AMOUNT][maxTileSize][2] = {
    { {0, 0}, {1, 0}, {2, 0}, {3, 0} }, //BAR_HORIZONTAL
    { {0, 0}, {1, 0}, {2, 0}, {3, 0} }, //-||-
    { {0, 0}, {0, 1}, {0, 2}, {0, 3} }, //BAR_VERTICAL
    { {0, 0}, {0, 1}, {0, 2}, {0, 3} }, //-||-

    { {0, 0}, {1, 0}, {1, -1}, {1, -2} }, //J_0
    { {0, 0}, {0, 1}, {1, 1}, {2, 1} }, //J_90
    { {0, 0}, {-1, 0}, {-1, 1}, {-1, 2} }, //J_180
    { {0, 0}, {0, -1}, {-1, -1}, {-2, -1} }, //J_270

    { {0, 0}, {-1, 0}, {-1, -1}, {-1, -2} }, //L_0
    { {0, 0}, {0, -1}, {1, -1}, {2, -1} }, //L_90
    { {0, 0}, {1, 0}, {1, 1}, {1, 2} }, //L_180
    { {0, 0}, {0, 1}, {-1, 1}, {-2, 1} }, //L_270

    { {0, 0}, {1, 0}, {1, -1}, {2, -1} }, //S_0
    { {0, 0}, {0, 1}, {1, 1}, {1, 2} }, //S_90
    { {0, 0}, {1, 0}, {1, -1}, {2, -1} }, //S_180
    { {0, 0}, {0, 1}, {1, 1}, {1, 2} }, //S_270

    { {0, 0}, {0, 1}, {1, 0}, {1, 1} }, //SQR	

    { {0, 0}, {-1, -1}, {0, -1}, {1, -1} }, //T_0
    { {0, 0}, {1, -1}, {1, 0}, {1, 1} }, //T_90
    { {0, 0}, {-1, 1}, {0, 1}, {1, 1} }, //T_180
    { {0, 0}, {-1, -1}, {-1, 0}, {-1, 1} }, //T_270

    { {0, 0}, {1, 0}, {1, 1}, {2, 1} }, //Z_0
    { {0, 0}, {0, 1}, {1, -1}, {1, 0} }, //Z_90
    { {0, 0}, {1, 0}, {1, 1}, {2, 1} }, //Z_180
    { {0, 0}, {0, 1}, {1, -1}, {1, 0} }, //Z_270
};

const int holdPositionsAmount[SHAPE_AMOUNT] = {
    4, 4, 4, 4, 4, 4, 4
};

const int holdPositions[SHAPE_AMOUNT][maxTileSize][2] = {
    { {0, 1}, {1, 1}, {2, 1}, {3, 1} }, //BAR
    { {0, 0}, {0, 1}, {1, 1}, {2, 1} }, //J
    { {0, 1}, {1, 1}, {2, 1}, {2, 0} }, //L
    { {0, 1}, {1, 1}, {1, 0}, {2, 0} }, //S
    { {1, 0}, {2, 0}, {1, 1}, {2, 1} }, //SQUARE
    { {0, 1}, {1, 1}, {2, 1}, {1, 0} }, //T
    { {0, 0}, {1, 0}, {1, 1}, {2, 1} }  //Z
};