#include "src/h/statisticswindow.h"
#include "ui_statisticswindow.h"
#include <QtDebug>

StatisticsWindow::StatisticsWindow(Config* config, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StatisticsWindow),
    config(config)
{
    ui->setupUi(this);

    streaks = new Streaks(config->getRunsLocation());
    refreshRuns();
}

StatisticsWindow::~StatisticsWindow()
{
    delete ui;
}

void StatisticsWindow::refreshRuns() {
    // Check if the folder location changed since last update
    streaks->checkForReset(config->getRunsLocation());

    // Update the UI if anything changes in the streaks
    if (streaks->updateStreak()) {
        qDebug() << "streaks updated streak";
        qDebug() << streaks->getIroncladCurrString();
        qDebug() << streaks->getSilentCurrString();

        ui->label_ironclad_curr->setText(streaks->getIroncladCurrString());
        ui->label_silent_curr->setText(streaks->getSilentCurrString());
        ui->label_defect_curr->setText(streaks->getDefectCurrString());
        ui->label_watcher_curr->setText(streaks->getWatcherCurrString());
        ui->label_rot_curr->setText(streaks->getRotCurrString());
    }
}
