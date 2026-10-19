#pragma once
#include <vector>
#include <utility>

class Maze {
public:
    Maze(int width, int height);
    void reset();
    bool isWall(int x, int y) const;
    void setWall(int x, int y, bool wall);
    bool isStart(int x, int y) const;
    bool isFinish(int x, int y) const;
    void setVisited(int x, int y, bool visited);
    bool isVisited(int x, int y) const;
    int getWidth() const;
    int getHeight() const;
    std::pair<int,int> getStart() const;
    std::pair<int,int> getFinish() const;
    void setStart(int x, int y);
    void setFinish(int x, int y);
    void setCell(int x, int y, int value);
    int getCell(int x, int y) const;
    bool hasStart() const;
    bool hasFinish() const;
    void generateMaze(); 
private:
    int w, h;
    std::vector<std::vector<int>> grid;
    int startX = -1, startY = -1;
    int finishX = -1, finishY = -1;
    bool inBounds(int x, int y) const;
    void divide(int x1, int y1, int x2, int y2);
};