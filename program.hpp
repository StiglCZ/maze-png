#ifndef PROGRAM_HPP
#define PROGRAM_HPP

typedef unsigned char u8;
struct Point {
    int x;
    int y;
    friend bool operator==(const Point& a, const Point& b){
        return a.x == b.x && a.y == b.y;
    }
};

const int
    W = 121,
    H = 121,
    Scale = 3;

const bool
    solve_maze = true,
    export_unsolved = true;

const char
    output[] = "./solved.png",
    unsolved_output[] = "./unsolved.png";

const Point start = {0, 0};
#endif

// Maping utils
#define createDirections(p) Point                                       \
    up = {p.x, p.y - 2}, down = {p.x, p.y + 2},                         \
    left = {p.x - 2, p.y}, right = {p.x + 2, p.y}

#define checkAll() createDirections(pos);                        \
    if(isPossible(field, up)) possible.push_back(up);            \
    if(isPossible(field, down)) possible.push_back(down);        \
    if(isPossible(field, left)) possible.push_back(left);        \
    if(isPossible(field, right)) possible.push_back(right);

inline void fill(u8* field, Point o, Point n) {
    if(o.x != n.x) field[((n.x - o.x) / 2) + o.y * W + o.x] = 1;
    else field[((n.y - o.y) / 2 + o.y) * W + o.x] = 1;
}
