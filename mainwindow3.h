#ifndef MAINWINDOW3_H
#define MAINWINDOW3_H

#include <QMainWindow>

#include "src/h/config.h"
#include "src/h/options.h"
#include "src/h/tutorial.h"
#include "src/h/about.h"
#include "src/h/statisticswindow.h"
#include "src/h/referencewindow.h"
#include "src/h/run.h"

#include "potiondisplay.h"
#include "carddisplay.h"

namespace Ui {
class MainWindow3;
}

class MainWindow3 : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow3(QWidget *parent = nullptr);
    ~MainWindow3();

private:
    Ui::MainWindow3 *ui;

    // Displays
    PotionDisplay* potionDisplay;
    CardDisplay* cardDisplay;

    // Child windows
    Config* config;
    Options* optionsWindow;
    Tutorial* tutorialWindow;
    About* aboutWindow;
    StatisticsWindow* statsWindow;
    ReferenceWindow* referenceWindow;

    // Setup
    void setupUI();

    // Current autosave
    Run* currentRun;

    // Updates
    void updateCurrentSaveData(const QString& fullSavePath);
    void updateCurrentPotionInfo();
    void updateCurrentCardChances();

    void writePotionFile(int chance, bool hasSozu, bool hasWhiteBeast);
    void writeRareFile(double rareChance, double eliteRareChance);
    void writeUncFile(double uncChance, double eliteUncChance);

private slots:
    void refresh();
    void showOptionsWindow();
    void showTutorialWindow();
    void showAboutWindow();
    void showStatsWindow();
    void showReferenceWindow();

    void shutdown();
};

#endif // MAINWINDOW3_H
