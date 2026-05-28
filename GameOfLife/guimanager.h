#ifndef GUIMANAGER_H
#define GUIMANAGER_H

#include "simulationengine.h"
#include <QMainWindow>
#include <QPushButton>
#include <QGridLayout>
#include <QDir>
#include <QCoreApplication>
#include <QListWidgetItem>

QT_BEGIN_NAMESPACE
namespace Ui {
class GUIManager;
}
QT_END_NAMESPACE

class GUIManager : public QMainWindow
{
    Q_OBJECT

public:
    explicit GUIManager(QWidget *parent = nullptr);
    ~GUIManager() override;

private slots:
    void on_createNewButton_clicked();

    void on_loadExistingButton_clicked();

    void on_submitGrid_clicked();

    void on_createManual_clicked();

    void on_generateRandom_clicked();

    void on_saveButton_clicked();

    void on_loadListWidget_itemDoubleClicked(QListWidgetItem *item);

    void on_exitButtonL_clicked();

    void on_exitButtonD_clicked();

    void on_exitButtonM_clicked();

    void on_confirmDelete_clicked();

    void on_cancelDelete_clicked();

    void on_deleteExistingButton_clicked();

    void on_deleteListWidget_itemDoubleClicked(QListWidgetItem *item);

private:
    Ui::GUIManager *ui;
    SimulationEngine engine;
    QPushButton* buttonGrid[10][10] = {};
    QPushButton* buttonGridManual[10][10] = {};
    int selectedDeleteID;

    void updateDisplay();
    void enableDisableCell(bool enable);
    void displaySimulationList(bool load);
    void initializeSavesFolder();
    void syncNextSimID();
};
#endif // GUIMANAGER_H
