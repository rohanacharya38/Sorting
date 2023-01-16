# build script fors SDL in bash using gcc
#!/bin/bash

# Compile the program
gcc -o program main.c `sdl2-config --cflags --libs`

# Run the program
./program
