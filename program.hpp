#ifndef PROGRAM_HPP
#define PROGRAM_HPP
#include <png.h>
typedef unsigned char u8;
struct Point {
    int x;
    int y;
    friend bool operator==(const Point& a, const Point& b){
        return a.x == b.x && a.y == b.y;
    }
};
struct Color {
    int r, g, b;
};

const Color
    WALL = {0x88, 0x88, 0x88},
    ENDS = {0xff, 0x00, 0x00},
    BACK = {0xee, 0xee, 0xee},
    AUTO = {0x00, 0x00, 0xfe};


const int
    W = 131,
    H = 131,
    Scale = 10;

const bool
    solve_maze = true,
    export_unsolved = true;

const char
    output[] = "./solved.png",
    unsolved_output[] = "./unsolved.png";

const Point start = {0, 0};
#endif

// Maping utils
#define createDirections(p, s) Point                                 \
    up = {p.x, p.y - s}, down = {p.x, p.y + s},                         \
    left = {p.x - s, p.y}, right = {p.x + s, p.y};

inline void fill(u8* field, Point o, Point n) {
    if(o.x != n.x) field[((n.x - o.x) / 2) + o.y * W + o.x] = 1;
    else field[((n.y - o.y) / 2 + o.y) * W + o.x] = 1;
}
