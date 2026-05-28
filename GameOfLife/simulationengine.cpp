#include "simulationengine.h"

int SimulationEngine::nextSimID = 1;

SimulationEngine::SimulationEngine()
{
    isRunning = false;
    speed = 1.0;
    generation = 0;
    simID = nextSimID;
    nextSimID++;

    connect(&timer, &QTimer::timeout, this, &SimulationEngine::advanceGeneration);
}

SimulationEngine::SimulationEngine(int rows, int cols) : grid(rows, cols)
{
    isRunning = false;
    speed = 1.0;
    generation = 0;
    simID = nextSimID;
    nextSimID++;

    connect(&timer, &QTimer::timeout, this, &SimulationEngine::advanceGeneration);
}

void SimulationEngine::start()
{
    int timeInt = static_cast<int>(1000/speed);
    timer.start(timeInt);
}

void SimulationEngine::stop()
{
    timer.stop();
}

void SimulationEngine::reset()
{
    generation = 0;
    grid.clear();
    emit updateGUI();
}

void SimulationEngine::saveSim()
{
    QJsonObject root;
    root["simID"] = simID;
    root["generation"] = generation;
    root["speed"] = speed;

    QJsonArray gridArray;
    for(int x = 0; x < 10; x++)
    {
        QJsonArray row;
        for(int y = 0; y < 10; y++)
        {
            row.append(grid.getCellState(x,y));
        }
        gridArray.append(row);
    }
    root["grid"] = gridArray;

    QString fileName = QString("%1/saves/sim_%2.json").arg(QCoreApplication::applicationDirPath()).arg(simID);
    QFile file(fileName);
    if(file.open(QIODevice::WriteOnly | QIODevice::Truncate)) //WriteOnly overwrites original file, truncate ensures it clears extra data if shorter
    {
        QJsonDocument doc(root);
        file.write(doc.toJson());
        file.flush();
        file.close();
    }
}

void SimulationEngine::advanceGeneration()
{
    grid.calculateNextState();
    generation++;
    emit updateGUI();
}

void SimulationEngine::toggleCell(int x, int y)
{
    grid.toggleCellState(x, y);
}

void SimulationEngine::setSpeed(double speedNum)
{
    speed = speedNum;
}

void SimulationEngine::generateRandom()
{
    grid.initializeRandom();
}

void SimulationEngine::loadData(const Grid& newGrid, int id, double newSpeed, int gen)
{
    this->grid = newGrid;
    this->simID = id;
    this->speed = newSpeed;
    this->generation = gen;

    if(id >= nextSimID)
    {
        nextSimID = id + 1;
    }
    emit updateGUI();
}

int SimulationEngine::getNextSimID()
{
    int id = nextSimID;
    nextSimID++;
    return id;
}

bool SimulationEngine::cellState(int x, int y)
{
    return grid.getCellState(x, y);
}

int SimulationEngine::getGeneration()
{
    return generation;
}

void SimulationEngine::step()
{
    advanceGeneration();
}
//added for testing purposes
double SimulationEngine::getSpeed()
{
    return speed;
}