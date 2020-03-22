#include "src/h/options.h"
#include "ui_options.h"
#include <QDebug>
#include <QFileDialog>

Options::Options(Config* config, QWidget* parent) :
    QWidget(parent),
    ui(new Ui::Options),
    config(config)
{
    ui->setupUi(this);
}

Options::~Options()
{
    delete ui;
}

void Options::browseSaves() {
    qDebug() << "browsing saves";


    QString saveFolder = QFileDialog::getExistingDirectory();
    qDebug() << "saveFolder?" << saveFolder;

    if (!saveFolder.isEmpty()) {
        ui->edit_saves->setText(saveFolder);
    }
}

void Options::browseRuns() {
    qDebug() << "browsing runs";

    QString runsFolder = QFileDialog::getExistingDirectory();
    qDebug() << "runsFolder?" << runsFolder;

    if (!runsFolder.isEmpty()) {
        ui->edit_runs->setText(runsFolder);
    }
}

void Options::browsePotionFile() {
    qDebug() << "browsing potions";


    QString potionFile = QFileDialog::getSaveFileName();
    qDebug() << "potionFile?" << potionFile;

    if (!potionFile.isEmpty()) {
        ui->edit_potion_out->setText(potionFile);
    }
}

void Options::saveAndExit() {
    // TODO: write to config file
    config->updateConfig(
                this->ui->edit_saves->text(),
                this->ui->edit_runs->text(),
                this->ui->check_potion->isChecked(),
                this->ui->edit_potion_out->text(),
                this->ui->edit_potion_format->text()
                );

    this->close();
}

void Options::loadPotionsFromConfig() {
    bool potionEnabled = config->getPotionWrite();
    this->ui->edit_potion_out->setText(config->getPotionOut());
    this->ui->check_potion->setChecked(config->getPotionWrite());
    this->ui->edit_potion_format->setText(config->getPotionFormat());

    this->ui->label_potion_out->setEnabled(potionEnabled);
    this->ui->label_potion_format->setEnabled(potionEnabled);
    this->ui->edit_potion_out->setEnabled(potionEnabled);
    this->ui->edit_potion_format->setEnabled(potionEnabled);
    this->ui->browse_potion_out->setEnabled(potionEnabled);
}

void Options::showWithConfig() {
    this->ui->edit_saves->setText(config->getSavesLocation());
    this->ui->edit_runs->setText(config->getRunsLocation());

    loadPotionsFromConfig();

    this->show();
}

// Link the potion controls' enabled status with the potion checkbox
void Options::onPotionToggle() {
    bool potionEnabled = this->ui->check_potion->isChecked();
    this->ui->label_potion_out->setEnabled(potionEnabled);
    this->ui->label_potion_format->setEnabled(potionEnabled);
    this->ui->edit_potion_out->setEnabled(potionEnabled);
    this->ui->edit_potion_format->setEnabled(potionEnabled);
    this->ui->browse_potion_out->setEnabled(potionEnabled);
}
