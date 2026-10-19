#include "PathFinder.h"
#include <vector>

PathFinder::PathFinder(Maze& maze) : maze(maze) {}

void PathFinder::reset() {
    while (!stack.empty()) stack.pop();
    for (int i = 0; i < maze.getWidth(); ++i)
        for (int j = 0; j < maze.getHeight(); ++j)
            if (maze.isVisited(i, j))
                maze.setVisited(i, j, false);
    finished = false;
    success = false;
}

void PathFinder::start() {
    reset();
    auto [sx, sy] = maze.getStart();
    stack.push({sx, sy, 0});
    maze.setVisited(sx, sy, true);
}

bool PathFinder::step() {
    if (finished) return true;
    if (stack.empty()) {
        finished = true;
        success = false;
        return true;
    }
    State& cur = stack.top();
    int x = cur.x, y = cur.y;

    const int dx[] = {0, 1, 0, -1};
    const int dy[] = {1, 0, -1, 0};

    while (cur.nextDir < 4) {
        int nx = x + dx[cur.nextDir];
        int ny = y + dy[cur.nextDir];
        cur.nextDir++;

        if (nx >= 0 && ny >= 0 && nx < maze.getWidth() && ny < maze.getHeight()) {
            if (!maze.isWall(nx, ny) && !maze.isVisited(nx, ny)) {
                if (maze.isFinish(nx, ny)) {
                    finished = true;
                    success = true;
                    stack.push({nx, ny, 0});
                    return true;
                }
                maze.setVisited(nx, ny, true);
                stack.push({nx, ny, 0});
                return false;
            }
        }
    }

    if (!maze.isStart(x, y) && !maze.isFinish(x, y)) {
        maze.setCell(x, y, 1);
        maze.setVisited(x, y, false);
    }
    stack.pop();
    return false;
}

bool PathFinder::isFinished() const { return finished; }
bool PathFinder::pathFound() const { return success; }