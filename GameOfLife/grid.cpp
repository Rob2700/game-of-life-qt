#include "grid.h"

Grid::Grid()
{
    rows = 10;
    cols = 10;
}

Grid::Grid(int r, int c)
{
    rows = r;
    cols = c;
    initializeEmpty();
}

void Grid::initializeRandom()
{
    //These 2 functions handle the random state values
    static std::default_random_engine gen(std::random_device{}());
    static std::bernoulli_distribution dist(0.5);

    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < cols; j++)
        {
            bool state = dist(gen);
            if(state)
            {
                grid.at(i).at(j).toggle();
            }
        }
    }
}

void Grid::initializeEmpty()
{
    for(int i = 0; i < rows; i++)
    {
        grid.push_back(std::vector<Cell>());
        for(int j = 0; j < cols; j++)
        {
            Cell c(i,j);
            grid.at(i).push_back(c);
        }
    }
}

void Grid::calculateNextState()
{
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < cols; j++)
        {
            Cell &c = grid.at(i).at(j);
            int numAliveNeighbors = 0;

            for(auto &coords : c.getNeighbors())
            {
                if(grid.at(coords.first).at(coords.second).getState())
                {
                    numAliveNeighbors++;
                }
            }

            if(c.getState())
            {
                c.setNextState((numAliveNeighbors == 2 | numAliveNeighbors == 3));
            }
            else
            {
                c.setNextState((numAliveNeighbors == 3));
            }
        }
    }

    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < cols; j++)
        {
            grid.at(i).at(j).applyNextState();
        }
    }
}

bool Grid::getCellState(int x, int y)
{
    return grid.at(x).at(y).getState();
}

void Grid::setCellState(int x, int y, bool state)
{
    grid.at(x).at(y).setState(state);
}

void Grid::toggleCellState(int x, int y)
{
    grid.at(x).at(y).toggle();
}

void Grid::clear()
{
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < cols; j++)
        {
            grid.at(i).at(j).setState(false);
        }
    }
}

