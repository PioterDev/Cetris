#ifndef STRUCTS_UNIONS_H
#define STRUCTS_UNIONS_H


#include <windows.h>
#include <SDL.h>

typedef unsigned long long size_t;

typedef struct Point {
    int x, y;
} Point;
typedef union int32_u {
    int integer;
    char bytes[4];
} int32_u;

/**
 * @brief Simple enum for a status of an operation. Used in otherwise 'void' functions.
 */
typedef enum status_t {
    SUCCESS,
    FAILURE,
    BASEOUTOFRANGE,
    BUFFERTOOSMALL
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
    Keymap keymap;
} ProgramParameters;

/**
 * @brief Struct for holding tiles.
 */
typedef struct Tile {
    SDL_Texture* texture;
    SDL_Rect rect;
} Tile;

/**
 * @brief Struct for holding parameters for the input thread. Unused, as you can't put the input loop on a separate thread.
 */
/* typedef struct inputThreadParameters {
    loopStatus_t* inputStatus;
    HANDLE inputMutex;
    HANDLE rectMutex;
    ProgramParameters* programParameters;
    SDL_Rect** rects;
} inputThreadParameters; */

/**
 * @brief Struct for holding parameters for the render thread.
 */
typedef struct renderThreadParameters {
    loopStatus_t* renderStatus;
    HANDLE renderMutex;
    HANDLE tilesMutex;
    SDL_Renderer* renderer;
    Tile** tiles;
} renderThreadParameters;

typedef enum TileColor{
    COLOR_UNKNOWN,
    AQUA,
    BLUE,
    GREEN,
    MAGENTA,
    RED,
    YELLOW
} TileColor;

typedef enum TileShape {
    SHAPE_UNKNOWN,
    BASE,
    BAR,
    L,
    S,
    SQUARE,
    T
} TileShape;


#endif