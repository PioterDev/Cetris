#ifndef CONFIG_H
#define CONFIG_H


#include <stdio.h>

#include "structs_unions.h"

/**
 * @brief Loads game configuration and returns a pointer to GameParameters struct, NULL on error.
 * 
 * @param configFile stream to config file
 * @param debugFile stream to debug file, NULL if not needed
 * @return pointer to GameParameters struct, NULL on error 
 */
ProgramParameters* loadConfig(FILE* configFile, FILE* debugFile);

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