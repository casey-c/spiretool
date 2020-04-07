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

#include "src/h/resourcemanager.h"

// fwd decl
QJsonObject readSaveFileToObject(QString file);
QString getMostRecentlyUpdatedFile(QString directory);
QString extractCharacterName(QString file);
QString formatPotion(int chance, QString format);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    current_run(nullptr)
{
    ui->setupUi(this);
    config = new Config("config.json");

    optionsWindow = new Options(config);
    tutorialWindow = new Tutorial();
    aboutWindow = new About();
    statsWindow = new StatisticsWindow(config);
    referenceWindow = new ReferenceWindow();

//    cardDatabase = new CardDatabase();
//    cardDatabase->print();

    // Set the default unknown image
    ui->label_char_image->setPixmap(ResourceManager::getInstance().getUnknownPixmap());

    // Refresh imer
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&MainWindow::refresh));
    timer->start(1000);
}

void MainWindow::refresh(){
    refreshRecentSaves();
    statsWindow->refreshRuns();
}

void MainWindow::refreshRecentSaves() {
    QString savesDir = config->getSavesLocation();
    QString recentSave = Utils::getMostRecentlyUpdatedFile(savesDir);

    QString fullSavePath =  savesDir.endsWith("/") ?
                QString("%1%2").arg(savesDir, recentSave) :
                QString("%1/%2").arg(savesDir, recentSave) ;

    // Refresh if necessary
    updateCurrentSaveData(fullSavePath);
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


void MainWindow::updateCurrentPotionInfo() {
    // Update the potion chance
    int potion_chance = current_run->getPotionChance() + 40;
    int floor = current_run->getCurrentFloor();

    // TODO: verify this patch works
    // boss chest floors
    if (floor == 17 || floor == 34 || floor == 52) {
      potion_chance = 40;
    }
    // boss fight floors (post fight only)
    else if (current_run->isPostCombat() && (floor == 16 || floor == 33 || floor == 51)) {
        potion_chance = 40;
    }

    // BUGFIX: Special Relics (Sozu/WhiteBeast)
    if (current_run->hasSozuRelic()) {
      //potion_chance = 0;
      this->ui->label_pc->setText(QString("0% (SOZU)"));
    }
    else if (current_run->hasWhiteBeastStatue()) {
      //potion_chance = 100;
      this->ui->label_pc->setText(QString("100% (WhiteBeastStatue)"));
    }
    else {
      this->ui->label_pc->setText(QString("%1\%").arg(potion_chance));
    }

    // Write to file if desired
    if (config->getPotionWrite()) {
      writePotionFile(potion_chance, current_run->hasSozuRelic(), current_run->hasWhiteBeastStatue());
    }
}

void MainWindow::updateCurrentCardChances() {
    int cbr = current_run->getCBR();
    int numCards = Utils::determineNumRewardCards(
                current_run->hasQuestionCard(),
                current_run->hasBustedCrown(),
                current_run->hasPrayerWheel()
                );
    int baseRareChance = Utils::determineRareChance( current_run->hasNloths() );

    double rareChance = Utils::calculateProb(cbr, numCards, baseRareChance, Utils::RARE) * 100.0;
    double uncommonChance = Utils::calculateProb(cbr, numCards, baseRareChance, Utils::UNCOMMON) * 100.0;
    double commonChance = Utils::calculateProb(cbr, numCards, baseRareChance, Utils::COMMON) * 100.0;

    qDebug() << "Our cbr is" << cbr;
    qDebug() << "Num cards" << numCards;
    qDebug() << "Our base rare chance is" << baseRareChance;

    qDebug() << "final rare" << rareChance;
    qDebug() << "final unc" << uncommonChance;
    qDebug() << "final comm" << commonChance;

    QString txt = QString("Next %1 Cards (WIP - See tooltip)").arg(numCards);
    if ((current_run->hasQuestionCard() || current_run->hasBustedCrown() || current_run->hasPrayerWheel() || current_run->hasNloths()))
        txt.push_back("[*]");
    ui->label_card_info->setText(txt);


//    double rareChance = Utils::calculateAtLeast1Rare(cbr, false, false, false) * 100.0;
//    double uncommonChance = Utils::calculateAtLeast1Uncommon(cbr, false, false, false) * 100.0;
//    double commonChance = Utils::calculateAtLeast1Common(cbr, false, false, false) * 100.0;

    ui->label_cc->setText(QString("%1\%").arg(QString::number(commonChance, 'f', 2)));
    ui->label_uc->setText(QString("%1\%").arg(QString::number(uncommonChance, 'f', 2)));
    ui->label_rc->setText(QString("%1\%").arg(QString::number(rareChance, 'f', 2)));

    // Write to file if desired
    if (config->getUncWrite())
        writeUncFile(QString::number(uncommonChance, 'f', 2));
    if (config->getRareWrite())
        writeRareFile(QString::number(rareChance, 'f', 2));

}

void MainWindow::updateCurrentSaveData(QString fullSavePath) {
    // TODO: rewrite this awful horrendous code. it's really really awful

    if (current_run == nullptr || current_run->differentFile(fullSavePath)) {
      // Rebuild the old file if necessary
        if (current_run != nullptr)
            delete current_run;
        current_run = Run::build(fullSavePath);

        // Couldn't understand the run file, so quit early
        if (current_run == nullptr)
            return;
    }
    // Current run file still exists and nothing changed means we can quit early
    else if (!current_run->refresh()){
        return;
    }

    // Update GUI text
    ui->label_char->setText(current_run->getCharNameAndFloor());
    ui->label_char_image->setPixmap(current_run->getPixmap());

    updateCurrentPotionInfo();
    updateCurrentCardChances();
}


void MainWindow::writePotionFile(int chance, bool hasSozu, bool hasWhiteBeast) {
  QString filename = config->getPotionLocation();
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

void MainWindow::writeUncFile(QString chance) {
  QString filename = config->getUncommonLocation();
  QString format =  Utils::formatStrings(chance, config->getUncommonFormat());

  qDebug() << "Final format string: " << format;

  QFile file(filename);
  if (file.open(QFile::WriteOnly | QFile::Truncate)) {
    file.write(format.toLocal8Bit());
    file.close();
  }
  else {
    qWarning() << "ERROR: could not write uncommon file";
  }
}

void MainWindow::writeRareFile(QString chance) {
  QString filename = config->getRareLocation();
  QString format =  Utils::formatStrings(chance, config->getRareFormat());

  qDebug() << "Final format string: " << format;

  QFile file(filename);
  if (file.open(QFile::WriteOnly | QFile::Truncate)) {
    file.write(format.toLocal8Bit());
    file.close();
  }
  else {
    qWarning() << "ERROR: could not write rare file";
  }
}





















