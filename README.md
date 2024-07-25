# Cetris

Welcome to my repository, here you will find
a pretty (terrible) implementation of the game similar to "Tetris".

For feedback, go to Issues -> Feedback.

## Legal notice
This is not a commercial project and it never will be commercialized.

Textures for the game are made by myself and other contributors,
while the soundtrack is made by GregorQuendel (see README in out/assets/audio)
and used under the CC BY-NC 4.0 license (see [reference](https://www.classicals.de/licensing)).

## Features
- dynamic data structures (a queue to store future tiles)
- I/O operations (a modular configuration file as well as asset and sound loading)
- a resizable window (written using SDL2)
- dynamic video asset scaling based on the window size
- multithreaded input & rendering

The config itself gives you all the freedom to customize keybinds, target FPS,
window size, audio volume and game settings.

Currently, only Windows is supported because of the use of its threads.
Linux support will be added if I ever decide to do it (which may be never).

## How to build from source
1. Download SDL2's MinGW Devel ZIP files for SDL itself, SDL_image and SDL_mixer.
2. Change the `LIBDIR` variable in the makefile to a directory of your choice.
3. Run make.
4. Wait.
5. Done!
