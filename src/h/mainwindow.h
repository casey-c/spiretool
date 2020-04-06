#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "src/h/options.h"
#include "src/h/config.h"
#include "src/h/tutorial.h"
#include "src/h/about.h"
#include "src/h/statisticswindow.h"
#include "src/h/referencewindow.h"
#include "src/h/carddatabase.h"
#include "src/h/run.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Config* config;
    Options* optionsWindow;
    Tutorial* tutorialWindow;
    About* aboutWindow;
    StatisticsWindow* statsWindow;
    ReferenceWindow* referenceWindow;

    CardDatabase* cardDatabase;

    Run* current_run;

    void updateCurrentSaveData(QString fullSavePath);
    void writePotionFile(int chance, bool hasSozu, bool hasWhiteBeast);

    void refreshRecentSaves();
    void refreshRuns();

////////////////////////////////////////////////////////////////////////////////

private slots:
    void showOptionsWindow();
    void showTutorialWindow();
    void showAboutWindow();
    void showStatsWindow();
    void showReferenceWindow();

private slots:
    void refresh();

};
#endif // MAINWINDOW_H
