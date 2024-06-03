#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deus.h"
#include "logging.h"
#include "tile_queue.h"
#include "utils.h"

static const char tilesPath[] = "./assets/tiles/";
static const char audioPath[] = "./assets/audio/";

static const char baseTexturePaths[tileColorAmount][32] = {
    "tile_base_background.png",
    "tile_base_aqua.png",
    "tile_base_blue.png",
    "tile_base_green.png",
    "tile_base_magenta.png",
    "tile_base_orange.png",
    "tile_base_red.png",
    "tile_base_yellow.png"
};

static const char soundtrackPaths[soundtracksAmount][32] = {
    "Theme_music_box.mp3",
    "Theme_piano.mp3",
    "Theme_strings.mp3"
};

static const char soundEffectPaths[soundEffectAmount][32] = {
    "",
    "",
    "",
    "",
    "Rotate_clockwise.mp3",
    "Rotate_counterclockwise.mp3" //27 characters, wow
};

static inline void setParameter(ProgramParameters* parameters, const char* key, int value) {
    if     (!strcmp(key, "moveleft"))                   parameters->keymap.movePieceLeft = value;
    else if(!strcmp(key, "moveright"))                  parameters->keymap.movePieceRight = value;
    else if(!strcmp(key, "rotate_clockwise"))           parameters->keymap.rotateClockwise = value;
    else if(!strcmp(key, "rotate_counter_clockwise"))   parameters->keymap.rotateCounterClockwise = value;
    else if(!strcmp(key, "dropsoft"))                   parameters->keymap.dropSoft = value;
    else if(!strcmp(key, "drophard"))                   parameters->keymap.dropHard = value;
    else if(!strcmp(key, "hold"))                       parameters->keymap.hold = value;
    else if(!strcmp(key, "pause"))                      parameters->keymap.pause = value;

    else if(!strcmp(key, "width"))                      parameters->screenSize.width = value;
    else if(!strcmp(key, "height"))                     parameters->screenSize.height = value;
    else if(!strcmp(key, "fps"))                        parameters->fps = value;
    else if(!strcmp(key, "basefallspeed"))              parameters->baseFallSpeed = value;
    else if(!strcmp(key, "soundtrack"))                 parameters->soundtrack.id = value;
    else if(!strcmp(key, "soundtrack_volume"))          parameters->soundtrack.volume = value << 7 / 100;
    else if(!strcmp(key, "sfx_volume"))                 parameters->soundEffectsVolume = value << 7 / 100;
}

ProgramParameters* loadConfig(FILE* configFile, FILE* debugFile) {
    ProgramParameters* parameters = calloc(1, sizeof(ProgramParameters));
    if(parameters == NULL)return NULL;

    char buf[128] = {0};
    while(fgets(buf, sizeof(buf), configFile)) {
        if(debugFile != NULL)fprintf(debugFile, "[loadConfig] Read line: %s", buf);
        
        if(buf[0] == '#')continue; //comment line
        
        buf[strcspn(buf, "\n")] = '\0';
        char key[32] = {0};
        strncpy(key, buf, strcspn(buf, ":"));

        if(debugFile != NULL)fprintf(debugFile, "[loadConfig] Key: %s\n", key);

        char* value = strstr(buf, ":") + 1;
        while((value[0] == ' ' || value[0] == '\t') && value[0] != '\n')value++;
        
        if(debugFile != NULL)fprintf(debugFile, "[loadConfig] Value: %s\n", value);

        //...so anyway, let's begin this mess
        if(strlen(value) == 1) {
            //a-z
            if( (value[0] >= 91 && value[0] <= 126) || (value[0] >= 33 || value[0] <= 64)) {
                //<[, `> + <a, z> + <{, ~> or <!, /> + <0, 9> + <:, @>
                setParameter(parameters, key, (int)value[0]);
            }
        }
        else if(value[0] == 'F' || value[0] == 'f') {
            if(value[1] == '1') {
                if     (value[2] == '\0')setParameter(parameters, key, SDLK_F1);
                else if(value[2] == '0')setParameter(parameters, key, SDLK_F10);
                else if(value[2] == '1')setParameter(parameters, key, SDLK_F11);
                else if(value[2] == '2')setParameter(parameters, key, SDLK_F12);
                else setParameter(parameters, key, SDLK_F13 - 3 + value[2] - '0'); //weird offsetting due to SDL2's internal mapping
            }
            else if(value[1] == '2') {
                if(value[2] == '\0')setParameter(parameters, key, SDLK_F2);
                else if(value[2] <= '0' && value[2] >= '4')setParameter(parameters, key, SDLK_F20 + value[2] - '0'); //same here
            }
        }
        else if(!strcmp(value, "leftarrow"))    setParameter(parameters, key, SDLK_LEFT);
        else if(!strcmp(value, "rightarrow"))   setParameter(parameters, key, SDLK_RIGHT);
        else if(!strcmp(value, "uparrow"))      setParameter(parameters, key, SDLK_UP);
        else if(!strcmp(value, "downarrow"))    setParameter(parameters, key, SDLK_DOWN);
        else if(!strcmp(value, "lshift"))       setParameter(parameters, key, SDLK_LSHIFT);
        else if(!strcmp(value, "space"))        setParameter(parameters, key, SDLK_SPACE);
        else if(!strcmp(value, "esc"))          setParameter(parameters, key, SDLK_ESCAPE);

        unsigned int valueNumerical = (unsigned int)atoi(value);
        if((!strcmp(key, "width") || !strcmp(key, "height") || !strcmp(key, "fps") || !strcmp(key, "basefallspeed")) && valueNumerical > 0) setParameter(parameters, key, valueNumerical);
        else if((!strcmp(key, "soundtrack") || !strcmp(key, "soundtrack_volume") || !strcmp(key, "sfx_volume")) && valueNumerical <= 100)      setParameter(parameters, key, valueNumerical);
    }

    return parameters;
}

status_t loadBaseTextures(ProgramParameters* parameters, SDL_Renderer* renderer) {
    char path[256];
    strcpy(path, tilesPath);
    char* pos = path + strlen(tilesPath);
    SDL_Rect rect;

    //The first one is necessary to get the base tile size
    strcpy(pos, baseTexturePaths[0]);
    parameters->baseTextures[0] = loadTextureRect(path, renderer, &rect);
    if(parameters->baseTextures[0] == NULL) return FAILURE;
    parameters->baseTileSize = rect.h;

    for(int i = 1; i < tileColorAmount; i++) {
        strcpy(pos, baseTexturePaths[i]);
        parameters->baseTextures[i] = loadTexture(path, renderer);
        if(parameters->baseTextures[i] == NULL) return FAILURE; //no need to worry about freeing textures, it's done on exit
    }

    return SUCCESS;
}

status_t loadSoundtrack(ProgramParameters* parameters) {
    char path[256];
    strcpy(path, audioPath);
    char* pos = path + strlen(audioPath);

    if(parameters->soundtrack.id < 1 || parameters->soundtrack.id > 3)return BASEOUTOFRANGE;
    strcpy(pos, soundtrackPaths[parameters->soundtrack.id - 1]);
    parameters->soundtrack.music = loadMusic(path);

    if(parameters->soundtrack.music == NULL) return FAILURE;

    return SUCCESS;
}

status_t loadSoundEffects(ProgramParameters* parameters) {
    char path[256];
    strcpy(path, audioPath);
    char* pos = path + strlen(audioPath);

    for(int i = 0; i < soundEffectAmount; i++) {
        if(strlen(soundEffectPaths[i]) > 0) {
            strcpy(pos, soundEffectPaths[i]);
            parameters->soundEffects[i].sound = loadSound(path);
            if(parameters->soundEffects[i].sound == NULL) return FAILURE;
        }
    }

    return SUCCESS;
}

void freeProgramConfig(ProgramParameters* params) {
    for(int i = 0; i < tileColorAmount; i++) {
        if(params->baseTextures[i] != NULL) {
            SDL_DestroyTexture(params->baseTextures[i]);
        }
    }
    freeTileQueue(params->tileQueue);
    freeMatrix(params->tetrisGrid, params->tetrisGridSize.height);
    freeMusic(params->soundtrack.music);
    for(int i = 0; i < soundEffectAmount; i++) {
        if(params->soundEffects[i].sound != NULL) {
            freeSound(params->soundEffects[i].sound);
        }
    }

    free(params);
}

void printKeymap(Keymap* keymap, FILE* stream) {
    static char bindings[sizeof(Keymap) / sizeof(int)][32] = {
        "Move left", 
        "Move right", 
        "Rotate clockwise", 
        "Rotate counter-clockwise",
        "Soft drop", 
        "Hard drop", 
        "Hold", 
        "Pause",
        "Test"
    };
    Keymap_array* keymap_array = (Keymap_array*) keymap;
    for(int i = 0; i < (int) (sizeof(Keymap) / sizeof(int)); i++) {
        int32_u current;
        current.integer = keymap_array->keys[i];
        char buf[16] = {0};
        if((current.integer >= 91 && current.integer <= 126) || (current.integer >= 33 && current.integer <= 64)) { //ASCII
            buf[0] = current.bytes[0];
            buf[1] = '\0';
        }
        else if(current.integer == SDLK_LEFT)   strcpy(buf, "Left arrow");
        else if(current.integer == SDLK_RIGHT)  strcpy(buf, "Right arrow");
        else if(current.integer == SDLK_UP)     strcpy(buf, "Up arrow");
        else if(current.integer == SDLK_DOWN)   strcpy(buf, "Down arrow");
        else if(current.integer == SDLK_SPACE)  strcpy(buf, "Space");
        else if(current.integer == SDLK_ESCAPE) strcpy(buf, "Escape");
        else if(current.integer == SDLK_LSHIFT) strcpy(buf, "LShift");
        fprintf(stream, "%s key: %s\n", bindings[i], buf);
    }
}

void printConfig(ProgramParameters* params, FILE* stream) {
    if(stream == NULL)return;
    fprintf(stream, "Screen width: %d px\n", params->screenSize.width);
    fprintf(stream, "Screen height: %d px\n", params->screenSize.height);
    fprintf(stream, "FPS: %d\n", params->fps);
    fprintf(stream, "Base fall speed: %d ms\n", params->baseFallSpeed);
    fprintf(stream, "Scaling factor: %d\n", params->scalingFactor);
    printKeymap(&params->keymap, stream);
}