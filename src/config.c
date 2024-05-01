#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "structs_unions.h"

static inline void setParameter(ProgramParameters* parameters, const char* key, int value) {
    if     (!strcmp(key, "moveleft"))                   parameters->keymap.movePieceLeft = value;
    else if(!strcmp(key, "moveright"))                  parameters->keymap.movePieceRight = value;
    else if(!strcmp(key, "rotate_clockwise"))           parameters->keymap.rotateClockwise = value;
    else if(!strcmp(key, "rotate_counter_clockwise"))   parameters->keymap.rotateCounterClockwise = value;
    else if(!strcmp(key, "dropsoft"))                   parameters->keymap.dropSoft = value;
    else if(!strcmp(key, "drophard"))                   parameters->keymap.dropHard = value;
    else if(!strcmp(key, "hold"))                       parameters->keymap.hold = value;
    else if(!strcmp(key, "pause"))                      parameters->keymap.pause = value;

    else if(!strcmp(key, "width"))                      parameters->screen_width = value;
    else if(!strcmp(key, "height"))                     parameters->screen_height = value;
    else if(!strcmp(key, "fps"))                        parameters->fps = value;
}

ProgramParameters* loadConfig(FILE* configFile, FILE* debugFile) {
    ProgramParameters* parameters = malloc(sizeof(ProgramParameters));
    if(parameters == NULL)return NULL;

    char buf[128] = {0};
    while(fgets(buf, 128, configFile)) {
        if(debugFile != NULL)fprintf(debugFile, "Read line: %s", buf);
        
        if(buf[0] == '#')continue; //comment line
        
        buf[strcspn(buf, "\n")] = '\0';
        char key[32] = {0};
        strncpy(key, buf, strcspn(buf, ":"));

        if(debugFile != NULL)fprintf(debugFile, "Key: %s\n", key);

        char* value = strstr(buf, ":") + 1;
        while((value[0] == ' ' || value[0] == '\t') && value[0] != '\n')value++;
        
        if(debugFile != NULL)fprintf(debugFile, "Value: %s\n", value);
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
                if(value[2] == '\0')setParameter(parameters, key, SDLK_F1);
                else if(value[2] == '0')setParameter(parameters, key, SDLK_F10);
                else if(value[2] == '1')setParameter(parameters, key, SDLK_F11);
                else if(value[2] == '2')setParameter(parameters, key, SDLK_F12);
                else setParameter(parameters, key, SDLK_F13 - 3 + value[2] - '0');
            }
            else if(value[1] == '2') {
                if(value[2] == '\0')setParameter(parameters, key, SDLK_F2);
                else if(value[2] <= '0' && value[2] >= '4')setParameter(parameters, key, SDLK_F20 + value[2] - '0');
            }
        }
        else if(!strcmp(value, "leftarrow"))    setParameter(parameters, key, SDLK_LEFT);
        else if(!strcmp(value, "rightarrow"))   setParameter(parameters, key, SDLK_RIGHT);
        else if(!strcmp(value, "uparrow"))      setParameter(parameters, key, SDLK_UP);
        else if(!strcmp(value, "downarrow"))    setParameter(parameters, key, SDLK_DOWN);
        else if(!strcmp(value, "lshift"))        setParameter(parameters, key, SDLK_LSHIFT);
        else if(!strcmp(value, "space"))        setParameter(parameters, key, SDLK_SPACE);
        else if(!strcmp(value, "esc"))          setParameter(parameters, key, SDLK_ESCAPE);

        int valueNumerical = atoi(value);
        if((!strcmp(key, "width") || !strcmp(key, "height") || !strcmp(key, "fps")) && valueNumerical != 0)setParameter(parameters, key, valueNumerical);
    }

    return parameters;
}

inline void freeProgramConfig(ProgramParameters* params) {
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
        "Pause"
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
    fprintf(stream, "Screen width: %d\n", params->screen_width);
    fprintf(stream, "Screen height: %d\n", params->screen_height);
    fprintf(stream, "FPS: %d\n", params->fps);

    printKeymap(&params->keymap, stream);
}