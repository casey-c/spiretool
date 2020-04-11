#ifndef MAINWINDOW2_H
#define MAINWINDOW2_H

#include <QWidget>
#include "custombutton.h"

namespace Ui {
class MainWindow2;
}

class MainWindow2 : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow2(QWidget *parent = nullptr);
    ~MainWindow2();

private:
    Ui::MainWindow2 *ui;
};

#endif // MAINWINDOW2_H
