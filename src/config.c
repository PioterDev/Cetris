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

const char keynames[178][32] = {
    //0
    "Unknown",   //0
    //1-5
    "Return",    //13
    "Escape",    //27
    "Backspace", //8
    "Tab",       //9
    "Space",     //32
    //6-17
    "F1", "F2", "F3", "F4", "F5", "F6", //1073741882-1073741893
    "F7", "F8", "F9", "F10", "F11", "F12", 
    //18-23
    "Print Screen", //1073741894
    "Scroll Lock",
    "Pause break",
    "Insert",
    "Home",
    "Page Up", //1073741899
    //24
    "Delete", //127
    //25-30
    "End", //1073741901
    "Page Down",
    "Right arrow", "Left arrow", "Down arrow", "Up arrow", //1073741906
    //31-47
    "Num Lock", //1073741907
    "Keypad /", "Keypad *", "Keypad -", "Keypad +", "Keypad Enter",
    "Keypad 1", "Keypad 2", "Keypad 3", "Keypad 4", "Keypad 5",
    "Keypad 6", "Keypad 7", "Keypad 8", "Keypad 9", "Keypad 0",
    "Keypad .", //1073741923
    //48-77
    "Application", //1073741925
    "Power",
    "Keypad =",
    "F13", "F14", "F15", "F16", "F17", "F18", 
    "F19", "F20", "F21", "F22", "F23", "F24",
    "Execute",
    "Help", "Menu", "Select", "Stop", "Repeat",
    "Undo", "Cut", "Copy", "Paste", "Find",
    "Volume up", "Volume down", "Keypad ,",
    "Keypad =400???", //1073741958
    //78-89
    "Alt erase?", //1073741977
    "Sysrq",
    "Cancel",
    "Clear",
    "Prior",
    "Backspace the 2nd",
    "Separator ? ? ?",
    "Out? Out of what?",
    "Oper? Operation?",
    "Clear again? Why?!",
    "CrSel (whatever this is...)",
    "ExSel (whatever this is...)", //1073741988
    //90-135
    "Keypad 00", //1073742000
    "Keypad 000",
    "Thousands separator? Fancy!",
    "Decimal separator? Calc gaming",
    "key unit...",
    "Currency sub-unit? HUH",
    "Keypad (", "Keypad )", "Keypad {", "Keypad }",
    "Keypad Tab",
    "Keypad Backspace",
    "Keypad A", "Keypad B", "Keypad C",
    "Keypad D", "Keypad E", "Keypad F",
    "Keypad XOR",
    "Keypad Power",
    "Keypad %",
    "Keypad <", "Keypad >",
    "Keypad &", "Keypad &&",
    "Keypad |", "Keypad ||",
    "Keypad :", "Keypad #",
    "Keypad Space",
    "Keypad @", "Keypad !",
    "Keypad MemStore, damn", "Keypad MemRecall", "Keypad MemClear, bruh",
    "Keypad MemAdd", "Keypad MemSubtract",
    "Keypad MemMultiply", "Keypad MemDivide",
    "Keypad +/-",
    "Keypad Clear", "Keypad Clear entry",
    "Keypad Binary", "Keypad Octal",
    "Keypad Decimal", "Keypad Hexadecimal", //1073742045
    //136-143
    "Left Control", "Left Shift", "Left Alt", "Left GUI Key", //1073742048
    "Right Control", "Right Shift", "Right Alt", "Right GUI Key", //1073742055
    //144
    "Mode key", //1073742081
    //145-161
    "Audio next", //1073742082
    "Audio previous",
    "Audio stop",
    "Audio play",
    "Audio mute",
    "Media select",
    "Web",
    "Mail",
    "Calculator",
    "Computer",
    "American Search",
    "American Home",
    "American back",
    "American forward",
    "American stop",
    "Americal refresh",
    "American bookmarks", //1073742098
    //162-171
    "Brightness down", //1073742099
    "Brightness up",
    "Toggle display",
    "Toggle illumination",
    "Illumination down",
    "Illumination up",
    "Eject",
    "Sleep",
    "App 1",
    "App 2", //1073742108
    //172-173
    "Audio rewind", //1073742109
    "Audio fast-forward", //1073742110
    //174-177
    "Left soft key", //1073742111
    "Right soft key",
    "Call",
    "End call" //1073742114
};

int getKeystringIndex(SDL_Keycode key) {
    if((key >= 33 && key <= 64) || (key >= 91 && key <= 126)) return -1; //ASCII character
    
    int index = 0;
    //first check the irregular ones
    switch(key) {
        case SDLK_RETURN:
            index = 1;
            break;
        case SDLK_ESCAPE:
            index = 2;
            break;
        case SDLK_BACKSPACE:
            index = 3;
            break;
        case SDLK_TAB:
            index = 4;
            break;
        case SDLK_SPACE:
            index = 5;
            break;
        case SDLK_DELETE:
            index = 24;
            break;
        case SDLK_MODE:
            index = 144;
            break;
        case SDLK_AUDIOREWIND:
            index = 172;
            break;
        case SDLK_AUDIOFASTFORWARD:
            index = 173;
            break;
    }
    //if the key is not there, check the continous ones
    if(index == 0) {
        if(key >= SDLK_F1 && key <= SDLK_F12)                           index = key - SDLK_F1 + 6;
        else if(key >= SDLK_PRINTSCREEN && key <= SDLK_PAGEUP)          index = key - SDLK_PRINTSCREEN + 18;
        else if(key >= SDLK_END && key <= SDLK_UP)                      index = key - SDLK_END + 25;
        else if(key >= SDLK_NUMLOCKCLEAR && key <= SDLK_KP_PERIOD)      index = key - SDLK_NUMLOCKCLEAR + 31;
        else if(key >= SDLK_APPLICATION && key <= SDLK_KP_EQUALSAS400)  index = key - SDLK_APPLICATION + 48;
        else if(key >= SDLK_ALTERASE && key <= SDLK_EXSEL)              index = key - SDLK_ALTERASE + 78;
        else if(key >= SDLK_KP_00 && key <= SDLK_KP_HEXADECIMAL)        index = key - SDLK_KP_00 + 90;
        else if(key >= SDLK_LCTRL && key <= SDLK_RGUI)                  index = key - SDLK_LCTRL + 136;
        else if(key >= SDLK_AUDIONEXT && key <= SDLK_AC_BOOKMARKS)      index = key - SDLK_AUDIONEXT + 145;
        else if(key >= SDLK_BRIGHTNESSDOWN && key <= SDLK_APP2)         index = key - SDLK_BRIGHTNESSDOWN + 162;
        else if(key >= SDLK_SOFTLEFT && key <= SDLK_ENDCALL)            index = key - SDLK_SOFTLEFT + 174;
    }
    return index;
}

typedef enum Option {
    MOVELEFT,
    MOVERIGHT,
    ROTATE_CLOCKWISE,
    ROTATE_COUNTERCLOCKWISE,
    DROPSOFT,
    DROPHARD,
    HOLD,
    PAUSE,
    OPTION_TEST,
    SCREEN_WIDTH,
    SCREEN_HEIGHT,
    FPS,
    BASEFALLSPEED,
    SPEEDMULTIPLIER,
    SOUNDTRACK,
    SOUNDTRACK_VOLUME,
    SFX_VOLUME,
    GRIDHEIGHT,
    GRIDWIDTH
} Option;

void setParameter(ProgramParameters* parameters, Option key, const int value) {
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
        case OPTION_TEST:
            parameters->keymap.test = value;
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
        case SPEEDMULTIPLIER:
            parameters->speedMultiplier = value;
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
        case GRIDHEIGHT:
            parameters->gridSize.height = value > 4 ? value : 4;
            break;
        case GRIDWIDTH:
            parameters->gridSize.width = value > 4 ? value : 4;
            break;
    }
}

status_t loadConfig(FILE* configFile, FILE* debugFile, ProgramParameters* parameters) {
    if(parameters == NULL) return MEMORY_FAILURE;

    char buf[128] = {0};
    while(fgets(buf, sizeof(buf), configFile)) {
        buf[strcspn(buf, "\n")] = '\0';
#ifdef DEBUG
        snprintf(loggingBuffer, loggingBufferSize, "[loadConfig] Read line: %s", buf);
        logToStream(debugFile, LOGLEVEL_DEBUG, NULL);
#endif
        if(buf[0] == '#') continue; //comment line

        char* key1 = buf;
        //remove whitespaces in front of a key
        while((key1[0] == ' ' || key1[0] == '\t') && key1[0] != '\n' && key1[0] != '\0') { key1++; } 
        
        char* key2 = key1;
        //calculate the position of the last character of a key
        while(key2[0] != ':' && key2[0] != '\n' && key2[0] != '\0') { key2++; }
        char key[32] = {0};
        size_t keyLength = (size_t)(key2 - key1);
        if(keyLength == 0) continue; //empty line
        else if(keyLength >= sizeof(key)) continue;
        strncpy(key, key1, keyLength);
        // strncpy(key, buf, strcspn(buf, ":"));

        char* value = strstr(buf, ":") + 1;
        //remove whitespaces in front of a value
        while((value[0] == ' ' || value[0] == '\t') && value[0] != '\n' && value[0] != '\0') { value++; }
#ifdef DEBUG
        snprintf(loggingBuffer, loggingBufferSize, "[loadConfig] Key: %s, Value: %s", key, value);
        logToStream(debugFile, LOGLEVEL_DEBUG, NULL);
#endif
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
        else if(!strcmp(key, "test"))                       option = OPTION_TEST;

        //numerical options
        else if(!strcmp(key, "width"))                      option = SCREEN_WIDTH;
        else if(!strcmp(key, "height"))                     option = SCREEN_HEIGHT;
        else if(!strcmp(key, "fps"))                        option = FPS;
        else if(!strcmp(key, "basefallspeed"))              option = BASEFALLSPEED;
        else if(!strcmp(key, "speed_multiplier"))           option = SPEEDMULTIPLIER;
        else if(!strcmp(key, "soundtrack"))                 option = SOUNDTRACK;
        else if(!strcmp(key, "soundtrack_volume"))          option = SOUNDTRACK_VOLUME;
        else if(!strcmp(key, "sfx_volume"))                 option = SFX_VOLUME;
        else if(!strcmp(key, "gridheight"))                 option = GRIDHEIGHT;
        else if(!strcmp(key, "gridwidth"))                  option = GRIDWIDTH;

        if(strlen(value) == 1) {
            if(value[0] >= '0' && value[0] <= '9') {
                switch(option) {
                    case SCREEN_WIDTH:
                    case SCREEN_HEIGHT:
                    case FPS:
                    case BASEFALLSPEED:
                    case SPEEDMULTIPLIER:
                    case SOUNDTRACK:
                    case SOUNDTRACK_VOLUME:
                    case SFX_VOLUME:
                    case GRIDHEIGHT:
                    case GRIDWIDTH:
                        setParameter(parameters, option, (int)value[0] - '0');
                        break;
                    default: setParameter(parameters, option, (int)value[0]);
                }
            }
            //ASCII
            //<[, `> + <a, z> + <{, ~> or <!, /> + <0, 9> + <:, @>
            else if( (value[0] >= '[' && value[0] <= '~') || (value[0] >= '!' || value[0] <= '@')) {
                setParameter(parameters, option, (int)value[0]);
            }
        }
        else if(value[0] == 'F' || value[0] == 'f') { //F1-24 keys
            if(value[1] == '1') {
                if     (value[2] == '\0')setParameter(parameters, option, SDLK_F1);
                else if(value[2] == '0') setParameter(parameters, option, SDLK_F10);
                else if(value[2] == '1') setParameter(parameters, option, SDLK_F11);
                else if(value[2] == '2') setParameter(parameters, option, SDLK_F12);
                else                     setParameter(parameters, option, SDLK_F13 - 3 + value[2] - '0'); //weird offsetting due to SDL's internal mapping
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

    return SUCCESS;
}

status_t loadBaseTextures(ProgramParameters* parameters) {
    char path[256];
    strcpy(path, tilesPath);
    char* pos = path + strlen(tilesPath);
    SDL_Rect rect;

    //The first one is necessary to get the base tile size
    strcpy(pos, baseTexturePaths[0]);
    parameters->baseTextures[0] = loadTextureRect(path, parameters->renderer, &rect);
    if(parameters->baseTextures[0] == NULL) return FAILURE;
    parameters->baseTileSize = rect.h;

    for(int i = 1; i < tileColorAmount; i++) {
        strcpy(pos, baseTexturePaths[i]);
        parameters->baseTextures[i] = loadTexture(path, parameters->renderer);
        if(parameters->baseTextures[i] == NULL) return FAILURE; //no need to worry about freeing textures, it's done on exit
    }

    return SUCCESS;
}

status_t loadDigits(ProgramParameters* parameters) {
    char path[256];
    strcpy(path, digitsPath);
    char* pos = path + sizeof(digitsPath) - 1;

    for(int i = 0; i < 10; i++) {
        sprintf(pos, "%d.png", i);
        parameters->digits[i] = loadTexture(path, parameters->renderer);
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

void freeTextures(ProgramParameters* params) {
        for(int i = 0; i < tileColorAmount; i++) {
        if(params->baseTextures[i] != NULL) {
            SDL_DestroyTexture(params->baseTextures[i]);
        }
    }
    for(int i = 0; i < 10; i++) {
        if(params->digits[i] != NULL) {
            SDL_DestroyTexture(params->digits[i]);
        }
    }
}

void freeProgramConfig(ProgramParameters* params) {
    freeTextures(params);
    if(params->grid != NULL) freeMatrix(params->grid, params->gridSize.height);
    for(int i = 0; i < soundtracksAmount; i++) {
        freeMusic(params->soundtracks[i].music);
    }
    for(int i = 0; i < soundEffectAmount; i++) {
        if(params->soundEffects[i].sound != NULL) {
            freeSound(params->soundEffects[i].sound);
        }
    }
}

void printKeymap(Keymap* keymap, FILE* stream) {
    Keymap_array* keymap_array = (Keymap_array*) keymap;
    for(int i = 0; i < (int) (sizeof(Keymap) / sizeof(int)); i++) {
        int32_u key;
        key.integer = keymap_array->keys[i];
        int index = getKeystringIndex(key.integer);
        if(index == -1) {
            snprintf(loggingBuffer, loggingBufferSize, "[printKeymap] %s key: %c", bindings[i], key.bytes[0]);
            logToStream(stream, LOGLEVEL_INFO, NULL);
        }
        else {
            snprintf(loggingBuffer, loggingBufferSize, "[printKeymap] %s key: %s", bindings[i], keynames[index]);
            logToStream(stream, LOGLEVEL_INFO, NULL);
        }
    }
}

void printConfig(ProgramParameters* params, FILE* stream) {
    if(stream == NULL) return;
    snprintf(loggingBuffer, loggingBufferSize, "[printConfig] Screen width: %d px", params->screenSize.width);
    logToStream(stream, LOGLEVEL_INFO, NULL);

    snprintf(loggingBuffer, loggingBufferSize, "[printConfig] Screen height: %d px", params->screenSize.height);
    logToStream(stream, LOGLEVEL_INFO, NULL);

    snprintf(loggingBuffer, loggingBufferSize, "[printConfig] FPS: %d", params->fps);
    logToStream(stream, LOGLEVEL_INFO, NULL);

    snprintf(loggingBuffer, loggingBufferSize, "[printConfig] Base fall speed: %d ms", params->baseFallSpeed);
    logToStream(stream, LOGLEVEL_INFO, NULL);

    snprintf(loggingBuffer, loggingBufferSize, "[printConfig] Speed multiplier: %d", params->speedMultiplier);
    logToStream(stream, LOGLEVEL_INFO, NULL);

    snprintf(loggingBuffer, loggingBufferSize, "[printConfig] Scaling factor: %g", params->scalingFactor);
    logToStream(stream, LOGLEVEL_INFO, NULL);

    snprintf(loggingBuffer, loggingBufferSize, "[printConfig] Grid height: %d", params->gridSize.height);
    logToStream(stream, LOGLEVEL_INFO, NULL);

    snprintf(loggingBuffer, loggingBufferSize, "[printConfig] Grid width: %d", params->gridSize.width);
    logToStream(stream, LOGLEVEL_INFO, NULL);

    printKeymap(&params->keymap, stream);
}