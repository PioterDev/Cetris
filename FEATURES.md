This projects features an implementation of the game "Tetris". It contains:

- dynamic data structures (a queue to store future tiles)
- I/O operations (a modular configuration file as well as asset loading)
- a window (written using SDL2)
- dynamic asset scaling based on the window size
- variable game speed (basefallspeed option in the config)
- delta time rendering


Function descriptions are in header files.

Unfortunately, some functions are *really* long (over 250 lines), 
this was unavoidable due to the complexity of Tetris itself.
There are Tetris implementations that fit in 200 lines of code,
but in my opinion, having a robust-but-long code is okay.