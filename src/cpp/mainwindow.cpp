#include "src/h/mainwindow.h"
#include "ui_mainwindow.h"
#include <QtDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>
#include <QDir>

#include "src/h/options.h"

// fwd decl
QJsonObject readSaveFileToObject(QString file);
QString getMostRecentlyUpdatedFile(QString directory);
QString extractCharacterName(QString file);
QString formatPotion(int chance, QString format);

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    config = new Config("config.json");

    data = new Data();
    optionsWindow = new Options(config);
    tutorialWindow = new Tutorial();
    aboutWindow = new About();

    // Refresh timer
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&MainWindow::refresh));
    timer->start(1000);
}

void MainWindow::refresh(){
    QString dir = config->getSavesLocation();
    QString file = getMostRecentlyUpdatedFile(dir);

    QString fullPath =  dir.endsWith("/") ?
                QString("%1%2").arg(dir, file) :
                QString("%1/%2").arg(dir, file) ;

    if (!fullPath.isEmpty()) {
        updateData(file, readSaveFileToObject(fullPath));
    }
    else {
        qDebug() << "something went wrong";
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showOptionsWindow() {
    optionsWindow->showWithConfig();
}

void MainWindow::showTutorialWindow() {
    tutorialWindow->show();
}

void MainWindow::showAboutWindow() {
    aboutWindow->show();
}

QJsonObject readSaveFileToObject(QString file) {
    QFile loadFile(file);

    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning() << "ERROR: could not read save file";
        return QJsonObject();
    }
    else {
        QByteArray data = loadFile.readAll();
        QJsonDocument loadDoc(QJsonDocument::fromJson(data));

        return loadDoc.object();
    }
}

void MainWindow::updateData(QString file, QJsonObject obj) {
    QString character = extractCharacterName(file);

    // Update the potion chance
    if (obj.contains("potion_chance")) {
        int chance = obj["potion_chance"].toInt() + 40;
        this->ui->label_pc->setText(QString("%1\%").arg(chance));

        // Write to file if desired
        if (config->getPotionWrite()) {
            writePotionFile(chance);
        }
    }

    // Try and include the floor number with the character name
    if (obj.contains("floor_num")) {
        int floor = obj["floor_num"].toInt();
        QString combined = QString("%1 (floor %2)").arg(character).arg(floor);
        this->ui->label_char->setText(combined);
    }
    else {
        this->ui->label_char->setText(character);
    }




}


QString getMostRecentlyUpdatedFile(QString directory) {
    QDir dir(directory);
    if (dir.exists()) {
        QStringList filters;
        filters << "*.autosaveBETA";

        dir.setSorting(QDir::Time);
        dir.setNameFilters(filters);

        QStringList l = dir.entryList();
        if (l.size() >= 1) {
            return l.first();
        }
    }
    return QString();
}

QString extractCharacterName(QString file) {
    QStringList list = file.split(".");
    if (list.size() > 1) {
        return list[0];
    }
    else {
        return QString("Unknown Character");
    }
}

void MainWindow::writePotionFile(int chance) {
    QString filename = config->getPotionOut();
    QString format = formatPotion(chance, config->getPotionFormat());
    qDebug() << "Final format string: " << format;

    QFile file(filename);
    if (file.open(QFile::WriteOnly | QFile::Truncate)) {
        file.write(format.toLocal8Bit());
        file.close();
    }
    else {
        qWarning() << "ERROR: could not write potion file";
    }
}

QString formatPotion(int chance, QString format) {
    QString chance_string = QString("%1").arg(chance);

    QStringList list = format.split("$");
    QStringList* final = new QStringList();

    for (QString s : list) {
        final->push_back(s);
        final->push_back(chance_string);
    }
    final->pop_back();

    QString combined = QString();

    for (QString s : *final) {
        combined = QString("%1%2").arg(combined).arg(s);
    }

    return combined;
}























