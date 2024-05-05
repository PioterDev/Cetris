#ifndef STRUCTS_UNIONS_H
#define STRUCTS_UNIONS_H

#include <stdio.h>
#include <windows.h>
#include <SDL.h>

#define tilesPath "./assets/tiles/"
#define tileColorAmount 7
#define GridHeight 20
#define GridWidth 10

typedef unsigned long long size_t;

typedef struct Point {
    int x, y;
} Point;
typedef union int32_u {
    int integer;
    char bytes[4];
} int32_u;

typedef union Color {
    unsigned int color;
    unsigned char rgba[4];
} Color;

typedef struct QueueElement {
    SDL_Event event;
    struct QueueElement* next;
} QueueElement;

typedef struct Queue {
    size_t size;
    QueueElement* head;
    QueueElement* last;
} Queue;

/**
 * @brief Simple enum for a status of an operation. Used in otherwise 'void' functions.
 */
typedef enum status_t {
    SUCCESS,
    FAILURE,
    BASEOUTOFRANGE,
    BUFFERTOOSMALL,
    MEMORY_FAILURE,
    FILEOPEN_FAILURE,
    LOADCONFIG_FAILURE,
    SDL_INIT_FAILURE,
    IMG_INIT_FAILURE,
    SDL_WINDOW_FAILURE,
    SDL_RENDERER_FAILURE,
    MUTEX_FAILURE,
    SEMAPHORE_FAILURE,
    THREAD_START_FAILURE
} status_t;

/**
 * @brief Loop status enum. Used for controlling execution of loops outside the main thread.
 */
typedef enum loopStatus_t {
    CONTINUE,
    STOP
} loopStatus_t;

/**
 * @brief Keymap struct to hold all keybinds. Maps to SDL's keycode.
 */
typedef struct Keymap {
    int movePieceLeft;
    int movePieceRight;
    int rotateClockwise;
    int rotateCounterClockwise;
    int dropSoft;
    int dropHard;
    int hold; //7 keys

    int pause;
    int test;
} Keymap;

/**
 * @brief Simple union to convert a Keymap struct to an array of integers.
 */
typedef union Keymap_array {
    Keymap keymap;
    int keys[8];
} Keymap_array;

/**
 * @brief Struct for holding program parameters.
 */
typedef struct ProgramParameters {
    int screen_width;
    int screen_height;
    int fps;
    short baseFallSpeed;
    short baseTileSize;
    Keymap keymap;
    //If it's > 0, scale up, if it's < 0, scale down
    char scalingFactor;
    LARGE_INTEGER* clockFrequency;
    LARGE_INTEGER* timer;
    FILE* generallog;
    FILE* errorlog;
    FILE* debugLog;
    SDL_Texture* baseTextures[tileColorAmount];
    char** tetrisGrid;
    int tetrisGridHeight;
    int tetrisGridWidth;
} ProgramParameters;

typedef enum TileColor{
    GHOST = -1,
    COLOR_UNKNOWN,
    AQUA,
    BLUE,
    GREEN,
    MAGENTA,
    ORANGE,
    RED,
    YELLOW
} TileColor;

typedef enum TileShape {
    SHAPE_UNKNOWN,
    BASE,
    BAR,
    J,
    L,
    S,
    SQUARE,
    T,
    Z,
    BACKGROUND
} TileShape;

typedef enum TileState {
    STATE_UNKNOWN,
    BAR_HORIZONTAL_UP,
    BAR_HORIZONTAL_DOWN,
    BAR_VERTICAL_LEFT,
    BAR_VERTICAL_RIGHT,
    J_0,
    J_90,
    J_180,
    J_270,
    L_0,
    L_90,
    L_180,
    L_270,
    S_0,
    S_90,
    S_180,
    S_270,
    SQR,
    T_0,
    T_90,
    T_180,
    T_270,
    Z_0,
    Z_90,
    Z_180,
    Z_270
} TileState;

/**
 * @brief Struct for holding tiles.
 */
typedef struct Tile {
    SDL_Texture* texture;
    SDL_Rect rect;
    double angle;
    SDL_Point center;
    TileColor color;
    TileShape shape;
    TileState state;
    Point position;
} Tile;

/**
 * @brief Struct for holding parameters for the render thread.
 */
typedef struct renderThreadParameters {
    loopStatus_t* renderStatus;
    HANDLE renderMutex;
    HANDLE tilesMutex;
    ProgramParameters* programParameters;
    SDL_Renderer* renderer;
    Color* backgroundColor;
    Tile** tiles;
    size_t tilesAmount;
} renderThreadParameters;

typedef struct logicParameters {
    ProgramParameters* programParameters;
    Tile** tiles;
    size_t tilesAmount;
} logicParameters;

typedef struct clockThreadParameters {
    loopStatus_t* clockStatus;
    LARGE_INTEGER* timer;
} clockThreadParameters;

typedef enum Rotation {
    ROTATION_CLOCKWISE = -1,
    ROTATION_COUNTERCLOCKWISE = 1
} Rotation;



#endif