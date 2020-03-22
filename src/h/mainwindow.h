#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "options.h"
#include "config.h"
#include "data.h"
#include "tutorial.h"
#include "about.h"
#include "src/h/statisticswindow.h"
#include "src/h/referencewindow.h"

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

    Data* data;

    void updateRecentSaveData(QString file, QJsonObject obj);
    void writePotionFile(int chance, bool hasSozu, bool hasWhiteBeast);

    void refreshRecentSaves();
    void refreshRuns();

    void setFormattedCharacterName(QJsonObject obj, QString ugly);

    QPixmap pixUnknown, pixIronclad, pixSilent, pixDefect, pixWatcher;

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
