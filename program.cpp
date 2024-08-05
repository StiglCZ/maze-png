#include <cstdlib>
#include <iostream>
#include <stack>
#include <vector>
#include <png.h>
#include <raylib.h>

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
const Point start = {0, 0};

bool isPossible(u8* field, Point p) {
    if(p.x < 0 || p.y < 0) return 0;
    if(p.x >= W || p.y >= H) return 0;
    if(field[p.x + p.y * W] == 1) return 0;
    return 1;
}

#define createDirections(p) Point                                       \
    up = {p.x, p.y - 2}, down = {p.x, p.y + 2},                         \
    left = {p.x - 2, p.y}, right = {p.x + 2, p.y}

#define checkAll() createDirections(pos);                     \
    if(isPossible(field, up)) possible.push_back(up);            \
    if(isPossible(field, down)) possible.push_back(down);      \
    if(isPossible(field, left)) possible.push_back(left);    \
    if(isPossible(field, right)) possible.push_back(right);
void fill(u8* field, Point o, Point n) {
    if(o.x != n.x) field[((n.x - o.x) / 2) + o.y * W + o.x] = 1;
    else field[((n.y - o.y) / 2 + o.y) * W + o.x] = 1;
}

void createMaze(u8* field) {
    srand(time(NULL));
    std::stack<Point> history = {};
    Point pos = start;
    while(1){
        std::vector<Point> possible;
        checkAll();
        if(possible.empty()){
            if(history.empty()) return;
            pos = history.top();
            history.pop();
            continue;
        }
        history.push(pos);
        pos = possible[rand() % possible.size()];
        field[pos.y * W + pos.x] = 1;
        fill(field, history.top(), pos);
    }
}

bool isUsable(u8* field, Point p) {
    if(p.x < 0 || p.y < 0) return 0;
    if(p.x >= W || p.y >= H) return 0;
    if(field[p.x + p.y * W] != 1) return 0;
    return 1;
}

void beatMaze(u8* field) {
    Point nextPos;
    std::stack<Point> history;
    Point pos = start;
    int counter = 0;
    while(1) {
        if(isUsable(field, {pos.x - 1, pos.y})) nextPos = {pos.x - 1, pos.y};
        else if(isUsable(field, {pos.x + 1, pos.y})) nextPos = {pos.x + 1, pos.y};
        else if(isUsable(field, {pos.x, pos.y - 1})) nextPos = {pos.x, pos.y - 1};
        else if(isUsable(field, {pos.x, pos.y + 1})) nextPos = {pos.x, pos.y + 1};
        else {
            field[pos.y * W + pos.x] = 3;
            pos = history.top();
            history.pop();
            counter++;
            continue;
        }
        counter++;
        history.push(pos);
        pos = nextPos;
        field[pos.y * W + pos.x] = 2;
        if(pos.x > W -3 && pos.y > H -3) return;
    }
}

void image_create(u8* data, int w, int h) {
    FILE *fp = fopen("output.png", "wb");
    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_infop info = png_create_info_struct(png);
    setjmp(png_jmpbuf(png));
    png_init_io(png, fp);
    png_set_IHDR(png, info, w, h, 8, 2, 0, 0, 0);
    png_write_info(png, info);
    for (uint y = 0; y < h; y++) {
        uint index = w * 3 * y;
        png_bytep row = (png_bytep)&data[index];
        png_write_row(png, row);
    }
    png_write_end(png, NULL);
    fclose(fp);
}


void drawSquare(u8* output, Point pos, Color col) {
    for(int i =0; i < Scale; i++)
        for(int j =0; j < Scale; j++) {
            output[3 * (Scale * pos.x + Scale * pos.y * W * Scale + i * W * Scale + j) + 0] = col.r;
            output[3 * (Scale * pos.x + Scale * pos.y * W * Scale + i * W * Scale + j) + 1] = col.g;
            output[3 * (Scale * pos.x + Scale * pos.y * W * Scale + i * W * Scale + j) + 2] = col.b;
        }
}
void createImage(u8* field) {
    u8 *output = new u8[W * Scale * H * Scale * 3];
    for(int x = 0; x < W; x++)
        for(int y =0; y < H; y++){
            u8 color = field[x + y * W];
            Color c;
            switch(field[x + y * W]){
                case 0: c = BLUE; break;
                case 1: c = BLACK; break;
                case 2: c = RED; break;
                default: c = BLACK; break;
            }
            drawSquare(output, {x, y}, c);
        }
    drawSquare(output, {0, 0}, {255, 255, 255, 255});
    drawSquare(output, {W -1, H -1}, {255, 255, 255, 255});
    image_create(output, W * Scale, H * Scale);
}
int main() {
    u8 *field = new u8[W * H];
    std::cout << "Generating...\n";
    createMaze(field);
    std::cout << "Beating...\n";
    beatMaze(field);
    std::cout << "Exporting...\n";
    createImage(field);
    std::cout << "Done!\n";
}
