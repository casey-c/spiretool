#include "mainwindow3.h"
#include "ui_mainwindow3.h"

#include "potiondisplay.h"
#include "carddisplay.h"
#include "custombutton.h"

#include "src/h/resourcemanager.h"
#include "src/h/utils.h"

#include <QTimer>
#include <QDebug>

MainWindow3::MainWindow3(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow3),
  potionDisplay(new PotionDisplay()),
  cardDisplay(new CardDisplay()),
  currentRun(nullptr)
{
  ui->setupUi(this);

  // Child windows
  config = new Config("config.json");
  optionsWindow = new Options(config);
  tutorialWindow = new Tutorial();
  aboutWindow = new About();
  statsWindow = new StatisticsWindow(config);
  referenceWindow = new ReferenceWindow();


  // Add all the custom QWidgets
  setupUI();

  // Refresh timer
  QTimer* timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, QOverload<>::of(&MainWindow3::refresh));
  timer->start(1000);
}

MainWindow3::~MainWindow3()
{
  delete ui;
}

void MainWindow3::setupUI() {
  CustomButton* optionsButton = new CustomButton("Options", ResourceManager::getInstance().getIconOptions(), this);
  CustomButton* statsButton = new CustomButton("Stats (WIP)", ResourceManager::getInstance().getIconStats(), this);
  CustomButton* referenceButton = new CustomButton("Reference (WIP)", ResourceManager::getInstance().getIconReference(), this);
  CustomButton* tutorialButton = new CustomButton("Tutorial", ResourceManager::getInstance().getIconHelp(), this);

  this->ui->layout_options->addWidget(optionsButton);
  this->ui->layout_options->addWidget(statsButton);
  this->ui->layout_options->addWidget(referenceButton);
  this->ui->layout_options->addWidget(tutorialButton);
  this->ui->layout_options->addStretch(1);

  this->ui->layout_potions->addWidget(potionDisplay);
  this->ui->layout_cards->addWidget(cardDisplay);

  // TODO: Connections
  connect(optionsButton,
          &QPushButton::clicked,
          this,
          &MainWindow3::showOptionsWindow
          );
  connect(statsButton,
          &QPushButton::clicked,
          this,
          &MainWindow3::showStatsWindow
          );
  connect(referenceButton,
          &QPushButton::clicked,
          this,
          &MainWindow3::showReferenceWindow
          );
  connect(tutorialButton,
          &QPushButton::clicked,
          this,
          &MainWindow3::showTutorialWindow
          );
}



void MainWindow3::refresh() {
  QString savesDir = config->getSavesLocation();
  QString recentSave = Utils::getMostRecentlyUpdatedFile(savesDir);

  QString fullSavePath =  savesDir.endsWith("/") ?
        QString("%1%2").arg(savesDir, recentSave) :
        QString("%1/%2").arg(savesDir, recentSave) ;

  // Refresh if necessary
  updateCurrentSaveData(fullSavePath);
}

void MainWindow3::updateCurrentSaveData(const QString& fullSavePath) {
  // TODO: rewrite this awful horrendous code. it's really really awful

  if (currentRun == nullptr || currentRun->differentFile(fullSavePath)) {
    // Rebuild the old file if necessary
    if (currentRun != nullptr)
      delete currentRun;
    currentRun = Run::build(fullSavePath);

    // Couldn't understand the run file, so quit early
    if (currentRun == nullptr)
      return;
  }
  // Current run file still exists and nothing changed means we can quit early
  else if (!currentRun->refresh()){
    return;
  }

  // Update GUI text
  ui->label_char->setText(currentRun->getCharNameAndFloor());
  ui->label_boss_asc->setText(currentRun->getBossAndAsc());
  ui->label_char_image->setPixmap(currentRun->getPixmap());

  updateCurrentPotionInfo();
  updateCurrentCardChances();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow3::updateCurrentPotionInfo() {
  // Update the potion chance
  int potion_chance = currentRun->getPotionChance() + 40;
  int floor = currentRun->getCurrentFloor();

  // TODO: verify this patch works
  // boss chest floors
  if (floor == 17 || floor == 34 || floor == 52) {
    potion_chance = 40;
  }
  // boss fight floors (post fight only)
  else if (currentRun->isPostCombat() && (floor == 16 || floor == 33 || floor == 51)) {
    potion_chance = 40;
  }

  // BUGFIX: Special Relics (Sozu/WhiteBeast)
  if (currentRun->hasSozuRelic())
    potionDisplay->setText( QString("0% (SOZU)") );
  else if (currentRun->hasWhiteBeastStatue())
    potionDisplay->setText( QString("100% (WBS)") );
  else
    potionDisplay->setText(  QString("%1\%").arg(potion_chance) );

  // Write to file if desired
  if (config->getPotionWrite()) {
    writePotionFile(potion_chance, currentRun->hasSozuRelic(), currentRun->hasWhiteBeastStatue());
  }
}


void MainWindow3::updateCurrentCardChances() {
    int cbr = currentRun->getCBR();
    int numCards = Utils::determineNumRewardCards(
                currentRun->hasQuestionCard(),
                currentRun->hasBustedCrown(),
                currentRun->hasPrayerWheel()
                );

    int baseRareChance = Utils::determineRareChance( 3, currentRun->hasNloths() );
    int baseEliteRareChance = Utils::determineRareChance( 10, currentRun->hasNloths() );

    double rareChance = Utils::calculateProb(cbr, numCards, baseRareChance, Utils::RARE) * 100.0;
    double eliteRareChance = Utils::calculateProb(cbr, numCards, baseEliteRareChance, Utils::RARE) * 100.0;

    double uncChance = Utils::calculateProb(cbr, numCards, baseRareChance, Utils::UNCOMMON) * 100.0;
    double eliteUncChance = Utils::calculateProb(cbr, numCards, baseEliteRareChance, Utils::UNCOMMON) * 100.0;

    // Update UI
    QString txt = QString("Rare Chance (Next %1 Cards)").arg(numCards);
    ui->label_card_display->setText(txt);

    cardDisplay->setRareChances(rareChance, eliteRareChance);

//    ui->label_cc->setText(QString("%1\%").arg(QString::number(commonChance, 'f', 2)));
//    ui->label_uc->setText(QString("%1\%").arg(QString::number(uncommonChance, 'f', 2)));
//    ui->label_rc->setText(QString("%1\%").arg(QString::number(rareChance, 'f', 2)));

    // Write to file if desired
    if (config->getUncWrite())
        writeUncFile(uncChance, eliteUncChance);
    if (config->getRareWrite())
        writeRareFile(rareChance, eliteRareChance);

}

void MainWindow3::writePotionFile(int chance, bool hasSozu, bool hasWhiteBeast) {
  QString filename = config->getPotionLocation();
  QString format("");

  // Pick the proper format based on the strings written in the options menu
  if (hasSozu && config->getSozuOverride())
    format = Utils::formatPotion(chance, config->getSozuFormat());
  else if (hasWhiteBeast && config->getWbsOverride())
    format = Utils::formatPotion(chance, config->getWbsFormat());
  else
    format = Utils::formatPotion(chance, config->getPotionFormat());

  qDebug() << "Final format string: " << format;
  Utils::writeStringToFile(format, config->getPotionLocation());
}

void MainWindow3::writeUncFile(double uncChance, double eliteUncChance) {
  QString format =  Utils::formatStrings(
              QString::number(uncChance, 'f', 2),
              QString("$"),
              config->getUncommonFormat()
              );

  format = Utils::formatStrings(
              QString::number(eliteUncChance, 'f', 2),
              QString("@"),
              format
              );

  qDebug() << "Final format string: " << format;
  Utils::writeStringToFile(format, config->getUncommonLocation());
}

void MainWindow3::writeRareFile(double rareChance, double eliteRareChance) {
  QString format =  Utils::formatStrings(
              QString::number(rareChance, 'f', 2),
              QString("$"),
              config->getRareFormat()
              );

  format = Utils::formatStrings(
              QString::number(eliteRareChance, 'f', 2),
              QString("@"),
              format
              );

  qDebug() << "Final format string: " << format;
  Utils::writeStringToFile(format, config->getRareLocation());
}

////////////////////////////////////////////////////////////////////////////////

// Slots for showing child windows
void MainWindow3::showOptionsWindow() { optionsWindow->showWithConfig(); }
void MainWindow3::showTutorialWindow() { tutorialWindow->show(); }
void MainWindow3::showAboutWindow() { aboutWindow->show(); }
void MainWindow3::showStatsWindow() { statsWindow->show(); }
void MainWindow3::showReferenceWindow() { referenceWindow->show(); }

void MainWindow3::shutdown() {
    QApplication::closeAllWindows();
}












