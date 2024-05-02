CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -I$(INCDIR) -I$(SDLINCLUDE) -L$(SDLLIB) -I$(SDLIMAGEINCLUDE) -L$(SDLIMAGELIB)

SUBSYSTEM = -Wl,-subsystem,windows

SRCDIR = src
INCDIR = include
BUILDDIR = build
LIBDIR = lib

SDLINCLUDE = G:/Projects/CLibs/SDL2/include/SDL2
SDLLIB = G:/Projects/CLibs/SDL2/lib
SDLIMAGEINCLUDE = G:/Projects/CLibs/SDL2_image/include/SDL2
SDLIMAGELIB = G:/Projects/CLibs/SDL2_image/lib

# Define source files and output executable
SRCS = $(wildcard $(SRCDIR)/*.c)
OBJS = $(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.o,$(filter-out main.c, $(SRCS)))
#OBJS = $(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.o,$(SRCS))
EXEC = ./out/main

# Specify the library name here
LIBRARYSDL = SDL2
LIBRARYSDLMAIN = SDL2main
LIBRARYSDLIMAGE = SDL2_image

# Default target
all: $(EXEC)

# Rule to build executable
$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -lmingw32 -l$(LIBRARYSDLMAIN) -l$(LIBRARYSDL) -l$(LIBRARYSDLIMAGE) -mwindows -lwinmm -g -o $@

# Rule to compile source files
$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -Dmain=SDL_main -c $< -o $@

#Rule to compile main.c | DOESN'T WORK!
#$(BUILDDIR)/main.o: ./main.c
#	$(CC) $(CFLAGS) -Dmain=SDL_main -c $< -o $@

# Clean rule
clean:
	rm -f $(EXEC) $(OBJS)