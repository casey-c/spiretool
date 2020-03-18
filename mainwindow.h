#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "options.h"
#include "config.h"
#include "data.h"
#include "tutorial.h"
#include "about.h"

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

    Data* data;

    void updateData(QString file, QJsonObject obj);
    void writePotionFile(int chance);

////////////////////////////////////////////////////////////////////////////////

private slots:
    void showOptionsWindow();
    void showTutorialWindow();
    void showAboutWindow();

private slots:
    void refresh();

};
#endif // MAINWINDOW_H
