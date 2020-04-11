#include "src/h/mainwindow.h"
#include "mainwindow3.h"

#include <QApplication>
#include <QFontDatabase>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFontDatabase::addApplicationFont(":/fonts/Kreon-Regular.ttf");
//    MainWindow w;
//    w.show();


    MainWindow3 w2;
    w2.show();


    return a.exec();
}
