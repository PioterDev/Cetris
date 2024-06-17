#ifndef CONFIG_H
#define CONFIG_H


#include <stdio.h>

#include "deus.h"

/**
 * @brief Loads program configuration and returns a pointer to ProgramParameters struct, NULL on error.
 * 
 * @param configFile stream to config file
 * @param debugFile stream to debug file, NULL if not needed
 * @param parameters pointer to ProgramParameters struct
 * @return SUCCESS on success, MEMORY_FAILURE on invalid arguments
 */
status_t loadConfig(FILE* configFile, FILE* debugFile, ProgramParameters* parameters);

/**
 * @brief Loads base tile textures into program parameters.
 * 
 * @param parameters pointer to ProgramParameters struct
 * @param renderer pointer to SDL2's renderer
 * @return SUCCESS on success, FAILURE on failure
 */
status_t loadBaseTextures(ProgramParameters* parameters, SDL_Renderer* renderer);

/**
 * @brief Loads digit textures into program parameters.
 * 
 * @param parameters pointer to ProgramParameters struct
 * @param renderer pointer to SDL2's renderer
 * @return SUCCESS on success, FAILURE on failure 
 */
status_t loadDigits(ProgramParameters* parameters, SDL_Renderer* renderer);

/**
 * @brief Loads a soundtrack from specified ID.
 * 
 * @param parameters pointer to ProgramParameters struct
 * @return SUCCESS on success, FAILURE on failure
 */
status_t loadSoundtracks(ProgramParameters* parameters);

/**
 * @brief Loads sound effects into program parameters.
 * 
 * @param parameters pointer to ProgramParameters struct
 * @return SUCCESS on success, FAILURE on failure 
 */
status_t loadSoundEffects(ProgramParameters* parameters);

/**
 * @brief Frees game configuration.
 * 
 * @param params pointer to ProgramParameters struct
 */
void freeProgramConfig(ProgramParameters* params);

/**
 * @brief Prints current keymap to given stream.
 * 
 * @param keymap pointer to Keymap struct
 * @param stream stream
 */
void printKeymap(Keymap* keymap, FILE* stream);

/**
 * @brief Prints current program configuration to given stream.
 * 
 * @param params pointer to ProgramParameters struct
 * @param stream stream
 */
void printConfig(ProgramParameters* params, FILE* stream);


#endif