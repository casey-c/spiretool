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
    if (streaks->refresh()) {
        qDebug() << "streaks updated streak";
        qDebug() << streaks->getIroncladCurrString();
        qDebug() << streaks->getSilentCurrString();

        // update current streaks
        ui->label_ironclad_curr->setText(streaks->getIroncladCurrString());
        ui->label_silent_curr->setText(streaks->getSilentCurrString());
        ui->label_defect_curr->setText(streaks->getDefectCurrString());
        ui->label_watcher_curr->setText(streaks->getWatcherCurrString());
        ui->label_rot_curr->setText(streaks->getRotCurrString());

        // update best win streaks
        ui->label_ironclad_best_win->setText(streaks->getIroncladLongestWinStreak());
        ui->label_silent_best_win->setText(streaks->getSilentLongestWinStreak());
        ui->label_defect_best_win->setText(streaks->getDefectLongestWinStreak());
        ui->label_watcher_best_win->setText(streaks->getWatcherLongestWinStreak());

        // update best loss streaks
        ui->label_ironclad_best_loss->setText(streaks->getIroncladLongestLossStreak());
        ui->label_silent_best_loss->setText(streaks->getSilentLongestLossStreak());
        ui->label_defect_best_loss->setText(streaks->getDefectLongestLossStreak());
        ui->label_watcher_best_loss->setText(streaks->getWatcherLongestLossStreak());

        // update total counts
        ui->label_rot_total->setText(streaks->getRotTotalString());
        ui->label_ironclad_total->setText(streaks->getIroncladTotalString());
        ui->label_silent_total->setText(streaks->getSilentTotalString());
        ui->label_defect_total->setText(streaks->getDefectTotalString());
        ui->label_watcher_total->setText(streaks->getWatcherTotalString());

        // update total wins
        ui->label_rot_total_win->setText(streaks->getRotTotalWins());
        ui->label_ironclad_total_win->setText(streaks->getIroncladTotalWins());
        ui->label_silent_total_win->setText(streaks->getSilentTotalWins());
        ui->label_defect_total_win->setText(streaks->getDefectTotalWins());
        ui->label_watcher_total_win->setText(streaks->getWatcherTotalWins());

        // update total losses
        ui->label_rot_total_loss->setText(streaks->getRotTotalLosses());
        ui->label_ironclad_total_loss->setText(streaks->getIroncladTotalLosses());
        ui->label_silent_total_loss->setText(streaks->getSilentTotalLosses());
        ui->label_defect_total_loss->setText(streaks->getDefectTotalLosses());
        ui->label_watcher_total_loss->setText(streaks->getWatcherTotalLosses());

        // update win rates
        ui->label_ironclad_wr->setText(streaks->getIroncladWinRate());
        ui->label_silent_wr->setText(streaks->getSilentWinRate());
        ui->label_defect_wr->setText(streaks->getDefectWinRate());
        ui->label_watcher_wr->setText(streaks->getWatcherWinRate());
        ui->label_rot_wr->setText(streaks->getRotWinRate());
    }
}



























