#include "Maze.h"
#include <cassert>
#include <cstdlib> 
#include <ctime>   

Maze::Maze(int width, int height) : w(width), h(height), startX(-1), startY(-1), finishX(-1), finishY(-1) {
    grid.assign(h, std::vector<int>(w, 1));
}

void Maze::reset() {
    for (int y = 0; y < h; ++y)
        for (int x = 0; x < w; ++x)
            grid[y][x] = 1;
    startX = startY = -1;
    finishX = finishY = -1;
}

bool Maze::inBounds(int x, int y) const {
    return x >= 0 && x < w && y >= 0 && y < h;
}

bool Maze::isWall(int x, int y) const {
    if (!inBounds(x, y)) return true;
    return grid[y][x] == 0;
}

void Maze::setWall(int x, int y, bool wall) {
    if (!inBounds(x, y)) return;
    if (wall) {
        if ((x == startX && y == startY) || (x == finishX && y == finishY)) return;
        grid[y][x] = 0;
    } else if (grid[y][x] == 0) {
        grid[y][x] = 1;
    }
}

bool Maze::isStart(int x, int y) const {
    return inBounds(x, y) && (x == startX && y == startY);
}

bool Maze::isFinish(int x, int y) const {
    return inBounds(x, y) && (x == finishX && y == finishY);
}

void Maze::setVisited(int x, int y, bool visited) {
    if (!inBounds(x, y)) return;
    if (visited && !isStart(x,y) && !isFinish(x,y)) {
        grid[y][x] = 4;
    } else if (!visited && grid[y][x] == 4) {
        grid[y][x] = 1;
    }
}

bool Maze::isVisited(int x, int y) const {
    return inBounds(x, y) && grid[y][x] == 4;
}

int Maze::getWidth() const { return w; }
int Maze::getHeight() const { return h; }

std::pair<int,int> Maze::getStart() const { return {startX, startY}; }
std::pair<int,int> Maze::getFinish() const { return {finishX, finishY}; }

void Maze::setStart(int x, int y) {
    if (!inBounds(x, y)) return;
    if (isFinish(x, y)) return; 

    if (startX != -1 && startY != -1 && !isFinish(startX, startY)) {
        grid[startY][startX] = 1;
    }
    startX = x; startY = y;
    grid[startY][startX] = 2;

    if (startX == finishX && startY == finishY) {
        finishX = finishY = -1;
    }
}

void Maze::setFinish(int x, int y) {
    if (!inBounds(x, y)) return;
    if (isStart(x, y)) return;
    if (finishX != -1 && finishY != -1 && !isStart(finishX, finishY)) {
        grid[finishY][finishX] = 1;
    }
    finishX = x; finishY = y;
    grid[finishY][finishX] = 3;
    if (finishX == startX && finishY == startY) {
        startX = startY = -1;
    }
}

void Maze::setCell(int x, int y, int value) {
    if (!inBounds(x, y)) return;

    if ((value == 0 || value == 1) && (isStart(x, y) || isFinish(x, y))) return;
    grid[y][x] = value;

    if (value != 2 && isStart(x, y)) { startX = -1; startY = -1; }
    if (value != 3 && isFinish(x, y)) { finishX = -1; finishY = -1; }
}

int Maze::getCell(int x, int y) const {
    if (!inBounds(x, y)) return 0;
    return grid[y][x];
}

bool Maze::hasStart() const {
    return startX != -1 && startY != -1 && inBounds(startX, startY) && grid[startY][startX] == 2;
}

bool Maze::hasFinish() const {
    return finishX != -1 && finishY != -1 && inBounds(finishX, finishY) && grid[finishY][finishX] == 3;
}

void Maze::generateMaze() {
    static bool seeded = false;
    if (!seeded) {
        srand(static_cast<unsigned>(time(nullptr)));
        seeded = true;
    }

    for (int y = 1; y < h - 1; ++y)
        for (int x = 1; x < w - 1; ++x)
            grid[y][x] = 1;


    startX = startY = finishX = finishY = -1;


    divide(1, 1, w - 2, h - 2);
}

void Maze::divide(int x1, int y1, int x2, int y2) {

    if (x2 - x1 < 2 || y2 - y1 < 2) return;

    bool horizontal = (rand() % 2 == 0);
    if (x2 - x1 < 2) horizontal = true;  
    if (y2 - y1 < 2) horizontal = false;  

    if (horizontal) {
        int y = y1 + 1 + (rand() % (y2 - y1 - 1));
        for (int x = x1; x <= x2; ++x)
            grid[y][x] = 0;
        int passX = x1 + rand() % (x2 - x1 + 1);
        grid[y][passX] = 1;
        divide(x1, y1, x2, y - 1);
        divide(x1, y + 1, x2, y2);
    } else {
        int x = x1 + 1 + (rand() % (x2 - x1 - 1));
        for (int y = y1; y <= y2; ++y)
            grid[y][x] = 0;
        int passY = y1 + rand() % (y2 - y1 + 1);
        grid[passY][x] = 1;
        divide(x1, y1, x - 1, y2);
        divide(x + 1, y1, x2, y2);
    }
}