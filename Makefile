CC=clang++

build: program.cpp program.hpp
    $(CC) program.cpp -lpng -o ./maze_png

run: build
    ./maze_png
