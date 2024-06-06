#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL.h>

#include "deus.h"
#include "logging.h"
#include "tile_queue.h"
#include "utils.h"

static const char tilesPath[]   = "./assets/tiles/";
static const char digitsPath[]  = "./assets/digits/";
static const char audioPath[]   = "./assets/audio/";

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

typedef enum Option {
    MOVELEFT,
    MOVERIGHT,
    ROTATE_CLOCKWISE,
    ROTATE_COUNTERCLOCKWISE,
    DROPSOFT,
    DROPHARD,
    HOLD,
    PAUSE,
    SCREEN_WIDTH,
    SCREEN_HEIGHT,
    FPS,
    BASEFALLSPEED,
    SOUNDTRACK,
    SOUNDTRACK_VOLUME,
    SFX_VOLUME
} Option;

void setParameter(ProgramParameters* parameters, Option key, int value) {
    switch(key) {
        case MOVELEFT:
            parameters->keymap.movePieceLeft = value;
            break;
        case MOVERIGHT:
            parameters->keymap.movePieceRight = value;
            break;
        case ROTATE_CLOCKWISE:
            parameters->keymap.rotateClockwise = value;
            break;
        case ROTATE_COUNTERCLOCKWISE:
            parameters->keymap.rotateCounterClockwise = value;
            break;
        case DROPSOFT:
            parameters->keymap.dropSoft = value;
            break;
        case DROPHARD:
            parameters->keymap.dropHard = value;
            break;
        case HOLD:
            parameters->keymap.hold = value;
            break;
        case PAUSE:
            parameters->keymap.pause = value;
            break;
        case SCREEN_WIDTH:
            parameters->screenSize.width = value > 0 ? value : 640;
            break;
        case SCREEN_HEIGHT:
            parameters->screenSize.height = value > 0 ? value : 360;
            break;
        case FPS:
            parameters->fps = value > 0 ? value : 60;
            break;
        case BASEFALLSPEED:
            parameters->baseFallSpeed = value;
            break;
        case SOUNDTRACK:
            parameters->flags.soundtrack = value - 1; //to ensure it doesn't overflow
            break;
        case SOUNDTRACK_VOLUME:
            parameters->soundtracksVolume = value <= 100 ? value << 7 / 100 : 128;
            break;
        case SFX_VOLUME:
            parameters->soundEffectsVolume = value <= 100 ? value << 7 / 100 : 128;
            break;
    }
}

ProgramParameters* loadConfig(FILE* configFile, FILE* debugFile) {
    ProgramParameters* parameters = calloc(1, sizeof(ProgramParameters));
    if(parameters == NULL)return NULL;

    char buf[128] = {0};
    while(fgets(buf, sizeof(buf), configFile)) {
        if(debugFile != NULL) fprintf(debugFile, "[loadConfig] Read line: %s", buf);
        
        if(buf[0] == '#')continue; //comment line
        
        buf[strcspn(buf, "\n")] = '\0';
        char key[32] = {0};
        strncpy(key, buf, strcspn(buf, ":"));


        char* value = strstr(buf, ":") + 1;
        while((value[0] == ' ' || value[0] == '\t') && value[0] != '\n')value++;
        
        if(debugFile != NULL) fprintf(debugFile, "[loadConfig] Key: %s, Value: %s\n", key, value);

        //...so anyway, let's begin this mess

        Option option;
        if     (!strcmp(key, "moveleft"))                   option = MOVELEFT;
        else if(!strcmp(key, "moveright"))                  option = MOVERIGHT;
        else if(!strcmp(key, "rotate_clockwise"))           option = ROTATE_CLOCKWISE;
        else if(!strcmp(key, "rotate_counter_clockwise"))   option = ROTATE_COUNTERCLOCKWISE;
        else if(!strcmp(key, "dropsoft"))                   option = DROPSOFT;
        else if(!strcmp(key, "drophard"))                   option = DROPHARD;
        else if(!strcmp(key, "hold"))                       option = HOLD;
        else if(!strcmp(key, "pause"))                      option = PAUSE;

        //numerical options
        else if(!strcmp(key, "width"))                      option = SCREEN_WIDTH;
        else if(!strcmp(key, "height"))                     option = SCREEN_HEIGHT;
        else if(!strcmp(key, "fps"))                        option = FPS;
        else if(!strcmp(key, "basefallspeed"))              option = BASEFALLSPEED;
        else if(!strcmp(key, "soundtrack"))                 option = SOUNDTRACK;
        else if(!strcmp(key, "soundtrack_volume"))          option = SOUNDTRACK_VOLUME;
        else if(!strcmp(key, "sfx_volume"))                 option = SFX_VOLUME;

        if(strlen(value) == 1) {
            //ASCII
            if( (value[0] >= 91 && value[0] <= 126) || (value[0] >= 33 || value[0] <= 64)) {
                //      <[, `> + <a, z> + <{, ~>    or        <!, /> + <0, 9> + <:, @>
                setParameter(parameters, option, (int)value[0]);
            }
        }
        else if(value[0] == 'F' || value[0] == 'f') { //F1-24 keys
            if(value[1] == '1') {
                if     (value[2] == '\0')setParameter(parameters, option, SDLK_F1);
                else if(value[2] == '0') setParameter(parameters, option, SDLK_F10);
                else if(value[2] == '1') setParameter(parameters, option, SDLK_F11);
                else if(value[2] == '2') setParameter(parameters, option, SDLK_F12);
                else                     setParameter(parameters, option, SDLK_F13 - 3 + value[2] - '0'); //weird offsetting due to SDL2's internal mapping
            }
            else if(value[1] == '2') {
                if     (value[2] == '\0')                   setParameter(parameters, option, SDLK_F2);
                else if(value[2] <= '0' && value[2] >= '4') setParameter(parameters, option, SDLK_F20 + value[2] - '0'); //same here
            }
        }
        else if(!strcmp(value, "leftarrow"))    setParameter(parameters, option, SDLK_LEFT);
        else if(!strcmp(value, "rightarrow"))   setParameter(parameters, option, SDLK_RIGHT);
        else if(!strcmp(value, "uparrow"))      setParameter(parameters, option, SDLK_UP);
        else if(!strcmp(value, "downarrow"))    setParameter(parameters, option, SDLK_DOWN);
        else if(!strcmp(value, "lshift"))       setParameter(parameters, option, SDLK_LSHIFT);
        else if(!strcmp(value, "space"))        setParameter(parameters, option, SDLK_SPACE);
        else if(!strcmp(value, "esc"))          setParameter(parameters, option, SDLK_ESCAPE);

        else {
            unsigned int valueNumerical = (unsigned int)atoi(value);
            setParameter(parameters, option, valueNumerical);
        }
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

status_t loadDigits(ProgramParameters* parameters, SDL_Renderer* renderer) {
    char path[256];
    strcpy(path, digitsPath);
    char* pos = path + strlen(digitsPath);

    for(int i = 0; i < 10; i++) {
        sprintf(pos, "%d.png", i);
        parameters->digits[i] = loadTexture(path, renderer);
        if(parameters->digits[i] == NULL) return FAILURE;
    }

    return SUCCESS;
}

status_t loadSoundtracks(ProgramParameters* parameters) {
    char path[256];
    strcpy(path, audioPath);
    char* pos = path + strlen(audioPath);

    for(int i = 0; i < soundtracksAmount; i++) {
        strcpy(pos, soundtrackPaths[i]);
        parameters->soundtracks[i].music = loadMusic(path);
        if(parameters->soundtracks[i].music == NULL) return FAILURE;
    }

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
    for(int i = 0; i < soundtracksAmount; i++) {
        freeMusic(params->soundtracks[i].music);
    }
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