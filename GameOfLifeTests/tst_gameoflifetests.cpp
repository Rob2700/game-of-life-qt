#include <QtTest>
#include "../GameOfLife/grid.h"
#include "../GameOfLife/cell.h"
#include "../GameOfLife/simulationengine.h"

class GameOfLifeTests : public QObject
{
    Q_OBJECT

private slots:
    void testSetCell();
    void testClear();
    void testToggleCell();
    void testMultipleCells();
    void testOverpopulation();
    void testRevival();
    void testReproduction();
    void testBlinkerOscillator();
    void testNextSimID();
    void testSetSpeed();
};

void GameOfLifeTests::testSetCell()
{
    Grid g(10,10);

    g.setCellState(1,1,true);

    QVERIFY(g.getCellState(1,1));
}

void GameOfLifeTests::testClear()
{
    Grid g(10,10);

    g.setCellState(1,1,true);
    g.clear();

    QVERIFY(!g.getCellState(1,1));
}

void GameOfLifeTests::testToggleCell()
{
    Grid g(10,10);

    g.toggleCellState(1,1);
    QVERIFY(g.getCellState(1,1));

    g.toggleCellState(1,1);
    QVERIFY(!g.getCellState(1,1));
}

void GameOfLifeTests::testMultipleCells()
{
    Grid g(10,10);

    g.setCellState(0,0,true);
    g.setCellState(1,1,true);
    g.setCellState(2,2,true);

    QVERIFY(g.getCellState(0,0));
    QVERIFY(g.getCellState(1,1));
    QVERIFY(g.getCellState(2,2));
}

void GameOfLifeTests::testOverpopulation()
{
    Grid g(10,10);

    g.setCellState(1,1,true);
    g.setCellState(0,1,true);
    g.setCellState(1,0,true);
    g.setCellState(1,2,true);
    g.setCellState(2,1,true);

    g.calculateNextState();

    QVERIFY(!g.getCellState(1,1));
}

void GameOfLifeTests::testRevival()
{
    Grid g(10,10);

    g.setCellState(0,1,true);
    g.setCellState(1,1,true);
    g.setCellState(2,1,true);

    g.calculateNextState();

    QVERIFY(g.getCellState(1,0));
}

void GameOfLifeTests::testReproduction()
{
    Grid g(10,10);

    g.setCellState(4,5,true);
    g.setCellState(5,4,true);
    g.setCellState(5,6,true);

    g.calculateNextState();

    QVERIFY(g.getCellState(5,5));
}

void GameOfLifeTests::testBlinkerOscillator()
{
    Grid g(10,10);

    // Vertical blinker
    g.setCellState(4,5,true);
    g.setCellState(5,5,true);
    g.setCellState(6,5,true);

    g.calculateNextState();

    // Should become horizontal
    QVERIFY(g.getCellState(5,4));
    QVERIFY(g.getCellState(5,5));
    QVERIFY(g.getCellState(5,6));

    QVERIFY(!g.getCellState(4,5));
    QVERIFY(!g.getCellState(6,5));
}

void GameOfLifeTests::testNextSimID()
{
    int id1 = SimulationEngine::getNextSimID();
    int id2 = SimulationEngine::getNextSimID();

    QCOMPARE(id2, id1 + 1);
}

void GameOfLifeTests::testSetSpeed()
{
    SimulationEngine engine;

    engine.setSpeed(2.0);

    QCOMPARE(engine.getSpeed(), 2.0);
}

QTEST_APPLESS_MAIN(GameOfLifeTests)
#include "tst_gameoflifetests.moc"