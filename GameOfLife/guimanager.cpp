#include "guimanager.h"
#include "./ui_guimanager.h"
#include "simulationengine.h"

GUIManager::GUIManager(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GUIManager)
{
    ui->setupUi(this);

    // Forces white GUI regardless of system settings
    // Ensures consistent whuite UI, readable text and visible controls
    // Using hex color codes for most
    this->setStyleSheet(R"(

/* Style for generation label */
#labelGen {
    color: black;
    background-color: white;
    border: 1px solid #ccc;
    padding: 3px;
}

/* Global widget styling (sets white background + black text) */
QWidget {
    background-color: white;
    color: black;
}

/* Button styling */
QPushButton {
    background-color: #f0f0f0;
    color: black;
    border: 1px solid #ccc;
    padding: 5px;
}

/* Button hover effect */
QPushButton:hover {
    background-color: #e0e0e0;
}

/* Label text color fix */
QLabel {
     color: black;
}

/* Slider track styling */
QSlider::groove:horizontal {
    border: 1px solid #ccc;
    height: 6px;
    background: #e0e0e0;
    border-radius: 3px;
}

/* Slider handle  */
QSlider::handle:horizontal {
    background: #0078d7;
    border: 1px solid #555;
    width: 14px;
    margin: -5px 0;
    border-radius: 7px;
}

/* Filled portion of slider */
QSlider::sub-page:horizontal {
    background: #0078d7;
    border-radius: 3px;
}

/* Unfilled portion of slider */
QSlider::add-page:horizontal {
    background: #e0e0e0;
    border-radius: 3px;
}

/* Disabled button styling*/
QPushButton:disabled {
    background-color: #e0e0e0;
    color: #a0a0a0;
    border: 1px solid #d3d3d3;
}

/* Save list styling */
QListWidget {
    background-color: #d0d0d0;
    border-color: black;
    border-width: 10px;
}

/* Text box styling */
QTextEdit {
    background-color: white;
    color: black;
    border: 0px;
}

/* Other text box styling */
QPlainTextEdit {
    background-color: white;
    color: black;
    border: 0px;
}
    )");

    initializeSavesFolder();
    syncNextSimID();

    ui->stopButton->setEnabled(false);
    ui->saveLabel->setVisible(false);

    connect(ui->startButton, &QPushButton::clicked, this, [this]()
            {
                engine.start();

                enableDisableCell(false); //Disable all cells

                ui->stepButton->setEnabled(false);
                ui->startButton->setEnabled(false);
                ui->stopButton->setEnabled(true);
                ui->resetButton->setEnabled(false);
                ui->speedControl->setEnabled(false);
                ui->saveButton->setEnabled(false);
                ui->saveLabel->setVisible(false);
                ui->exitButtonM->setEnabled(false);
            });

    connect(ui->stopButton, &QPushButton::clicked, this, [this]()
            {
                engine.stop();

                enableDisableCell(true); //Enable all cells

                ui->stepButton->setEnabled(true);
                ui->startButton->setEnabled(true);
                ui->stopButton->setEnabled(false);
                ui->resetButton->setEnabled(true);
                ui->speedControl->setEnabled(true);
                ui->saveButton->setEnabled(true);
                ui->saveLabel->setVisible(false);
                ui->exitButtonM->setEnabled(true);
            });

    connect(ui->resetButton, &QPushButton::clicked, this, [this]()
            {
                engine.reset();
                engine.setSpeed(1.0);

                ui->speedControl->setValue(100);
                ui->speedLabel->setText("Speed: 1.00x");

                enableDisableCell(true); //Enable all cells

                ui->startButton->setEnabled(true);
                ui->stopButton->setEnabled(false);
                ui->resetButton->setEnabled(false);
                ui->speedControl->setEnabled(true);
                ui->saveButton->setEnabled(true);
                ui->saveLabel->setVisible(false);
                ui->exitButtonM->setEnabled(true);
            });

    connect(ui->speedControl, &QSlider::valueChanged, this, [this](int value){
        double newSpeed = value / 100.0;

        engine.setSpeed(newSpeed);

        ui->speedLabel->setText("Speed: " + QString::number(newSpeed, 'f', 2) + "x");
    });

    connect(ui->stepButton, &QPushButton::clicked, this, [this]()
            {
                engine.step();

                ui->saveLabel->setVisible(false);
                ui->saveButton->setEnabled(true);
            });

    connect(&engine, &SimulationEngine::updateGUI, this, &GUIManager::updateDisplay);

    if(!ui->gridContainerMain->layout())
    {
        ui->gridContainerMain->setLayout(new QGridLayout());
    }

    QGridLayout *gridLayout = qobject_cast<QGridLayout*>(ui->gridContainerMain->layout());
    if(gridLayout)
    {
        gridLayout->setSpacing(0);

        for(int x = 0; x < 10; x++)
        {
            for(int y = 0; y < 10; y++)
            {
                QPushButton *btn = new QPushButton();

                btn->setText("");
                btn->setFixedSize(50, 50);
                btn->setStyleSheet("background-color: white; border: 1px solid gray;");

                connect(btn, &QPushButton::clicked, this, [this, x, y]() {
                    bool origState = engine.cellState(x, y);
                    engine.toggleCell(x, y);
                    if(origState)
                    {
                        buttonGrid[x][y]->setStyleSheet("background-color: white; border: 1px solid gray;");
                    }
                    else
                    {
                        buttonGrid[x][y]->setStyleSheet("background-color: black; border: 1px solid gray;");
                    }
                });

                gridLayout->addWidget(btn, y, x);
                buttonGrid[x][y] = btn;
            }
        }
    }
    else
    {
        qDebug() << "MainLayout not found";
    }

    if(!ui->gridContainerManual->layout())
    {
        ui->gridContainerManual->setLayout(new QGridLayout());
    }

    QGridLayout *gridLayoutManual = qobject_cast<QGridLayout*>(ui->gridContainerManual->layout());
    if(gridLayoutManual)
    {
        gridLayoutManual->setSpacing(0);

        for(int x = 0; x < 10; x++)
        {
            for(int y = 0; y < 10; y++)
            {
                QPushButton *btn = new QPushButton();

                btn->setText("");
                btn->setFixedSize(50, 50);
                btn->setStyleSheet("background-color: white; border: 1px solid gray;");

                connect(btn, &QPushButton::clicked, this, [this, x, y]() {
                    bool origState = engine.cellState(x, y);
                    engine.toggleCell(x, y);
                    if(origState)
                    {
                        buttonGridManual[x][y]->setStyleSheet("background-color: white; border: 1px solid gray;");
                    }
                    else
                    {
                        buttonGridManual[x][y]->setStyleSheet("background-color: black; border: 1px solid gray;");
                    }
                });

                gridLayoutManual->addWidget(btn, y, x);
                buttonGridManual[x][y] = btn;
            }
        }
    }
    else
    {
        qDebug() << "ManualLayout not found";
    }

    ui->stackedWidget->setCurrentIndex(0);
}

GUIManager::~GUIManager()
{
    delete ui;
}

void GUIManager::updateDisplay()
{
    for(int x = 0; x < 10; x++)
    {
        for(int y = 0; y < 10; y++)
        {
            bool state = engine.cellState(x, y);
            if(state)
            {
                buttonGrid[x][y]->setStyleSheet("background-color: black; border: 1px solid gray;");
            }
            else
            {
                buttonGrid[x][y]->setStyleSheet("background-color: white; border: 1px solid gray;");
            }
        }
    }

    ui->labelGen->setText("Generation: " + QString::number(engine.getGeneration()));
}

void GUIManager::enableDisableCell(bool enable)
{
    for(int x = 0; x < 10; x++)
    {
        for(int y = 0; y < 10; y++)
        {
            buttonGrid[x][y]->setEnabled(enable);
        }
    }
}

void GUIManager::on_createNewButton_clicked()
{
    engine.loadData(Grid(10, 10), SimulationEngine::getNextSimID(), 1.0, 0);
    ui->stackedWidget->setCurrentIndex(1);
}

void GUIManager::on_loadExistingButton_clicked()
{
    displaySimulationList(true);
    ui->stackedWidget->setCurrentIndex(3);
}

void GUIManager::on_submitGrid_clicked()
{
    updateDisplay();

    // Clear manual grid so it resets visually for next use
    for(int x = 0; x < 10; x++)
    {
        for(int y = 0; y < 10; y++)
        {
            buttonGridManual[x][y]->setStyleSheet("background-color: white; border: 1px solid gray;");
        }
    }

    ui->stackedWidget->setCurrentIndex(4);
}

void GUIManager::on_createManual_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void GUIManager::on_generateRandom_clicked()
{
    engine.generateRandom();
    updateDisplay();
    ui->stackedWidget->setCurrentIndex(4);
}

void GUIManager::initializeSavesFolder()
{
    QString savePath = QCoreApplication::applicationDirPath() + "/saves";
    QDir dir(savePath);
    if(!dir.exists())
    {
        dir.mkpath(".");
    }
}

void GUIManager::on_saveButton_clicked()
{
    ui->saveButton->setEnabled(false);
    engine.saveSim();
    ui->saveLabel->setVisible(true);
}

void GUIManager::displaySimulationList(bool load)
{
    if(load)
    {
        ui->loadListWidget->clear();
    }
    else
    {
        ui->deleteListWidget->clear();
    }

    QString path = QCoreApplication::applicationDirPath() + "/saves";
    QDir dir(path);
    QStringList filters;
    filters << "*.json";

    if(load)
    {
        for (const QString &filename : dir.entryList(filters, QDir::Files))
        {
            ui->loadListWidget->addItem(filename);
        }
    }
    else
    {
        for (const QString &filename : dir.entryList(filters, QDir::Files))
        {
            ui->deleteListWidget->addItem(filename);
        }
    }

}


void GUIManager::on_loadListWidget_itemDoubleClicked(QListWidgetItem *item)
{
    QString fileName = QCoreApplication::applicationDirPath() + "/saves/" + item->text();
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "File Open Failed:" << file.errorString();
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    QJsonObject root = doc.object();

    int id = root["simID"].toInt();
    int gen = root["generation"].toInt();
    double speed = root["speed"].toDouble();

    Grid loadedGrid(10,10);
    QJsonArray gridArray = root["grid"].toArray();
    for(int x = 0; x < 10; x++)
    {
        QJsonArray row = gridArray[x].toArray();
        for(int y = 0; y < 10; y++)
        {
            loadedGrid.setCellState(x, y, row[y].toBool());
        }
    }

    engine.loadData(loadedGrid, id, speed, gen);

    // Fix slider + label
    ui->speedControl->setValue(speed * 100);
    ui->speedLabel->setText("Speed: " + QString::number(speed, 'f', 2) + "x");

    updateDisplay();
    ui->stackedWidget->setCurrentIndex(4);
    file.close();
}

void GUIManager::syncNextSimID()
{
    int maxID = 0;
    QDir dir(QCoreApplication::applicationDirPath() + "/saves");
    QStringList files = dir.entryList(QStringList() << "sim_*.json", QDir::Files);

    for(const QString &filename : files)
    {
        int id = filename.section('_', 1).section('.', 0, 0).toInt();
        if (id > maxID)
        {
            maxID = id;
        }
    }

    SimulationEngine::setNextSimID(maxID + 1);
}


void GUIManager::on_exitButtonL_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void GUIManager::on_exitButtonD_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void GUIManager::on_exitButtonM_clicked()
{
    ui->startButton->setEnabled(true);
    ui->stopButton->setEnabled(false);
    ui->resetButton->setEnabled(true);
    ui->speedControl->setEnabled(true);
    ui->saveButton->setEnabled(true);
    ui->saveLabel->setVisible(false);
    ui->speedLabel->setText("Speed: 1.00x");
    ui->stackedWidget->setCurrentIndex(0);
}


void GUIManager::on_confirmDelete_clicked()
{
    QString fileName = QString("sim_%1.json").arg(selectedDeleteID);
    QString filePath = QCoreApplication::applicationDirPath() + "/saves/" + fileName;

    if (QFile::remove(filePath))
    {
        displaySimulationList(false);

        ui->stackedWidget->setCurrentIndex(5);
    }
    else
    {
        qDebug() << "Failed to delete file at " << filePath;
    }
}

void GUIManager::on_cancelDelete_clicked()
{
    ui->stackedWidget->setCurrentIndex(5);
}


void GUIManager::on_deleteExistingButton_clicked()
{
    displaySimulationList(false);
    ui->stackedWidget->setCurrentIndex(5);
}


void GUIManager::on_deleteListWidget_itemDoubleClicked(QListWidgetItem *item)
{
    QString fileName = item->text();
    selectedDeleteID = fileName.section('_', 1).section('.', 0, 0).toInt();

    ui->deleteConfirmText->setPlainText("\nAre you sure you want to delete simulation " + QString::number(selectedDeleteID) + "?\nOnce a simulation is deleted, it cannot be recovered.");
    ui->stackedWidget->setCurrentIndex(6);
}


