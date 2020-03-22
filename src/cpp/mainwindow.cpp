#include "src/h/mainwindow.h"
#include "ui_mainwindow.h"
#include <QtDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTimer>
#include <QDir>

#include "src/h/options.h"
#include "src/h/utils.h"
#include "src/h/streaks.h"

// fwd decl
QJsonObject readSaveFileToObject(QString file);
QString getMostRecentlyUpdatedFile(QString directory);
QString extractCharacterName(QString file);
QString formatPotion(int chance, QString format);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    pixUnknown(":/icons/unknown.svg"),
    pixIronclad(":/icons/ironclad.svg"),
    pixSilent(":/icons/silent.svg"),
    pixDefect(":/icons/defect.svg"),
    pixWatcher(":/icons/watcher.svg")
{
    ui->setupUi(this);

    config = new Config("config.json");

    data = new Data();
    optionsWindow = new Options(config);
    tutorialWindow = new Tutorial();
    aboutWindow = new About();
    statsWindow = new StatisticsWindow(config);
    referenceWindow = new ReferenceWindow();

    ui->label_char_image->setPixmap(pixUnknown);

    // Refresh imer
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&MainWindow::refresh));
    timer->start(1000);
}

void MainWindow::refreshRecentSaves() {
    QString savesDir = config->getSavesLocation();
    QString recentSave = Utils::getMostRecentlyUpdatedFile(savesDir);

    QString fullSavePath =  savesDir.endsWith("/") ?
                QString("%1%2").arg(savesDir, recentSave) :
                QString("%1/%2").arg(savesDir, recentSave) ;

    if (!fullSavePath.isEmpty()) {
        updateRecentSaveData(recentSave, Utils::readSaveFileToObject(fullSavePath));
    }
}

void MainWindow::refresh(){
    refreshRecentSaves();
    statsWindow->refreshRuns();
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
void MainWindow::showStatsWindow() {
    statsWindow->show();
}
void MainWindow::showReferenceWindow() {
    referenceWindow->show();
}


void MainWindow::updateRecentSaveData(QString file, QJsonObject obj) {
    QString character = Utils::extractCharacterName(file);

    // Update the potion chance
    if (obj.contains("potion_chance")) {
        int chance = obj["potion_chance"].toInt() + 40;

        // BUGFIX: Show the beginning act values at the boss chest
        // (one floor earlier than the save file displays)
        if (obj.contains("current_floor")) {
            int floor = obj["current_floor"].toInt();
            if (floor == 17 || floor == 34 || floor == 52) {
                chance = 40;
            }
        }

        // BUGFIX: Special Relics (Sozu/WhiteBeast)
        bool hasWhiteBeastStatue = false;
        bool hasSozu = false;

        if (obj.contains("relics")) {
            QJsonArray relics = obj["relics"].toArray();
            QJsonArray::iterator itr = relics.begin();

            //qDebug() << "Relic List:";
            for (; itr != relics.end(); ++itr) {
                QString r = itr->toString();
                if (r == "White Beast Statue")
                    hasWhiteBeastStatue = true;
                else if (r == "Sozu")
                    hasSozu = true;

                //qDebug() << "RELIC" << r;
            }
            //qDebug() << "---------";
        }

        if (hasSozu) {
          //chance = 0;
          this->ui->label_pc->setText(QString("0% (SOZU)"));
        }
        else if (hasWhiteBeastStatue) {
          //chance = 100;
          this->ui->label_pc->setText(QString("100% (WhiteBeastStatue)"));
        }
        else {
          this->ui->label_pc->setText(QString("%1\%").arg(chance));
        }


        // Write to file if desired
        if (config->getPotionWrite()) {
          writePotionFile(chance, hasSozu, hasWhiteBeastStatue);
        }
    }

    // Try and include the floor number with the character name
    setFormattedCharacterName(obj, character);
    //    if (obj.contains("floor_num")) {
    //        int floor = obj["floor_num"].toInt();
    //        QString combined = QString("%1 (floor %2)").arg(character).arg(floor);
    //        this->ui->label_char->setText(combined);
    //    }
    //    else {
    //        this->ui->label_char->setText(character);
    //    }




}



void MainWindow::writePotionFile(int chance, bool hasSozu, bool hasWhiteBeast) {
  QString filename = config->getPotionOut();
  QString format;

  // Pick the proper format based on the strings written in the options menu
  if (hasSozu && config->getSozuOverride())
    format = Utils::formatPotion(chance, config->getSozuFormat());
  else if (hasWhiteBeast && config->getWbsOverride())
    format = Utils::formatPotion(chance, config->getWbsFormat());
  else
    format = Utils::formatPotion(chance, config->getPotionFormat());

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


void MainWindow::setFormattedCharacterName(QJsonObject obj, QString ugly) {
  QString formatted = QString();
  QPixmap p;

  // Set the nicer string and pixmap image
  if (ugly == "IRONCLAD") {
    formatted = "Ironclad";
    p = pixIronclad;
  }
  else if (ugly == "THE_SILENT") {
    formatted = "Silent";
    p = pixSilent;
  }
  else if (ugly == "DEFECT") {
    formatted = "Defect";
    p = pixDefect;
  }
  else if (ugly == "WATCHER") {
    formatted = "Watcher";
    p = pixWatcher;
  }
  else {
    formatted = "Unknown Character";
    p = pixUnknown;
  }

  // Try to add the floor number
  if (obj.contains("floor_num")) {
    int floor = obj["floor_num"].toInt();
    formatted.append(QString(" (floor %1)").arg(floor));
  }

  // Update the text and image on the UI
  ui->label_char_image->setPixmap(p);
  ui->label_char->setText(formatted);
}






















