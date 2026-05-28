#include "cell.h"

Cell::Cell(int xCoord, int yCoord)
{
    x = xCoord;
    y = yCoord;
    isAlive = false;
    findNeighbors();
}

void Cell::toggle()
{
    isAlive = !isAlive;
}

void Cell::findNeighbors()
{
    for(int i = -1; i <= 1; i++)
    {
        for(int j = -1; j <= 1; j++)
        {
            int numI = x + i;
            int numJ = y + j;
            if((i != 0 || j != 0) && numI >= 0 && numI < 10 && numJ >= 0 && numJ < 10)
            {
                neighbors.push_back({numI, numJ});
            }
        }
    }
}

void Cell::applyNextState()
{
    isAlive = nextState;
}

void Cell::setNextState(bool s)
{
    nextState = s;
}

bool Cell::getState()
{
    return isAlive;
}

std::vector<std::pair<int,int>> Cell::getNeighbors()
{
    return neighbors;
}

void Cell::setState(bool s)
{
    isAlive = s;
}