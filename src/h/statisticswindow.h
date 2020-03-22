#ifndef STATISTICSWINDOW_H
#define STATISTICSWINDOW_H

#include <QWidget>
#include "src/h/config.h"
#include "src/h/streaks.h"

namespace Ui {
class StatisticsWindow;
}

class StatisticsWindow : public QWidget
{
    Q_OBJECT

public:
    explicit StatisticsWindow(Config* config, QWidget *parent = nullptr);
    ~StatisticsWindow();

    void refreshRuns();

private:
    Ui::StatisticsWindow *ui;
    Config* config;
    Streaks* streaks;
};

#endif // STATISTICSWINDOW_H
