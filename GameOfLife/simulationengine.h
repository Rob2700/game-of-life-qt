#ifndef SIMULATIONENGINE_H
#define SIMULATIONENGINE_H
#include "grid.h"
#include <QObject>
#include <QTimer>
#include <functional>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QCoreApplication>

class SimulationEngine : public QObject {
    Q_OBJECT
private:
    Grid grid;
    bool isRunning;
    double speed;
    int generation;
    static int nextSimID;
    int simID;
    QTimer timer;

private slots:
    void advanceGeneration();

signals:
    void updateGUI();

public:
    SimulationEngine();
    SimulationEngine(int rows, int cols);
    void loadData(const Grid& newGrid, int id, double newSpeed, int gen);
    void start();
    void stop();
    void reset();
    void saveSim();
    void toggleCell(int x, int y);
    void setSpeed(double speedNum);
    void generateRandom();
    bool cellState(int x, int y);
    int getGeneration();
    static void setNextSimID(int nextID){nextSimID = nextID;}
    static int getNextSimID();
    void step();
    double getSpeed(); // Getter added for testing purposes
};

#endif // SIMULATIONENGINE_H
