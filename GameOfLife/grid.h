#ifndef GRID_H
#define GRID_H
#include "cell.h"
#include <vector>
#include <utility>
#include <random>

class Grid {

public:
    Grid();
    Grid(int r, int c);
    void setCellState(int x, int y, bool state);

private:

    int rows;
    int cols;
    std::vector<std::vector<Cell>> grid;

    void initializeRandom();
    void initializeEmpty();
    void calculateNextState();
    bool getCellState(int x, int y);
    void toggleCellState(int x, int y);
    void clear();

    friend class SimulationEngine;
    friend class GameOfLifeTests;
};

#endif // GRID_H
