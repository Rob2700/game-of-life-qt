#ifndef CELL_H
#define CELL_H
#include <vector>
#include <utility>

class Cell {
private:
    Cell(int xCoord, int yCoord);
    std::vector<std::pair<int,int>> neighbors;
    int x;
    int y;
    bool isAlive;
    bool nextState;

    void toggle();
    void findNeighbors();
    void setNextState(bool s);
    void applyNextState();
    bool getState();
    void setState(bool s);
    std::vector<std::pair<int,int>> getNeighbors();

    friend class Grid;
    friend class GameOfLifeTests;
};

#endif // CELL_H
