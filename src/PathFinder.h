#pragma once
#include "Maze.h"
#include <stack>
#include <utility>

class PathFinder {
public:
    PathFinder(Maze& maze);
    void start();
    bool step();              
    bool isFinished() const;
    bool pathFound() const;
    void reset();
private:
    Maze& maze;
    struct State {
        int x, y;
        size_t nextDir;
    };
    std::stack<State> stack;
    bool finished = false;
    bool success = false;
};