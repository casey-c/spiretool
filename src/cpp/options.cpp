#include "src/h/options.h"
#include "ui_options.h"
#include <QDebug>
#include <QFileDialog>
#include <QShortcut>

Options::Options(Config* config, QWidget* parent) :
    QWidget(parent),
    ui(new Ui::Options),
    config(config)
{
    ui->setupUi(this);
    connect( new QShortcut(QKeySequence("Q"), this), &QShortcut::activated, this, &Options::close );
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

void Options::browseUnc() {
    QString file = QFileDialog::getSaveFileName();
    if (!file.isEmpty())
        ui->edit_unc_out->setText(file);
}

void Options::browseRare() {
    QString file = QFileDialog::getSaveFileName();
    if (!file.isEmpty())
        ui->edit_rare_out->setText(file);
}

void Options::saveAndExit() {
    // TODO: write to config file
    config->updateConfig(
                this->ui->edit_saves->text(),
                this->ui->edit_runs->text(),

                // Output files
                this->ui->edit_potion_out->text(),
                this->ui->edit_unc_out->text(),
                this->ui->edit_rare_out->text(),

                // Enabled booleans
                this->ui->check_potion->isChecked(),
                this->ui->check_sozu->isChecked(),
                this->ui->check_wbs->isChecked(),
                this->ui->check_unc->isChecked(),
                this->ui->check_rare->isChecked(),

                // Format strings
                this->ui->edit_potion_format->text(),
                this->ui->edit_potion_format_sozu->text(),
                this->ui->edit_potion_format_wbs->text(),
                this->ui->edit_unc_format->text(),
                this->ui->edit_rare_format->text()
                );

    this->close();
}

void Options::loadPotionsFromConfig() {
    bool potionEnabled = config->getPotionWrite();
    this->ui->edit_potion_out->setText(config->getPotionLocation());
    this->ui->check_potion->setChecked(config->getPotionWrite());
    this->ui->edit_potion_format->setText(config->getPotionFormat());

    this->ui->label_potion_out->setEnabled(potionEnabled);
    this->ui->label_potion_format->setEnabled(potionEnabled);
    this->ui->edit_potion_out->setEnabled(potionEnabled);
    this->ui->edit_potion_format->setEnabled(potionEnabled);
    this->ui->browse_potion_out->setEnabled(potionEnabled);
    this->ui->label_potion_overrides->setEnabled(potionEnabled);

    // Sozu & WBS
    this->ui->edit_potion_format_sozu->setText(config->getSozuFormat());
    this->ui->edit_potion_format_wbs->setText(config->getWbsFormat());
    this->ui->check_sozu->setChecked(config->getSozuOverride());
    this->ui->check_wbs->setChecked(config->getWbsOverride());

    this->ui->edit_potion_format_sozu->setEnabled(potionEnabled && config->getSozuOverride());
    this->ui->edit_potion_format_wbs->setEnabled(potionEnabled && config->getWbsOverride());
    this->ui->check_sozu->setEnabled(potionEnabled);
    this->ui->check_wbs->setEnabled(potionEnabled);
}

void Options::loadCardRaritiesFromConfig() {
    // Uncommon
    bool uncEnabled = config->getUncWrite();
    this->ui->edit_unc_out->setText(config->getUncommonLocation());
    this->ui->check_unc->setChecked(uncEnabled);
    this->ui->edit_unc_format->setText(config->getUncommonFormat());

    this->ui->label_unc_out->setEnabled(uncEnabled);
    this->ui->label_unc_format->setEnabled(uncEnabled);
    this->ui->edit_unc_out->setEnabled(uncEnabled);
    this->ui->edit_unc_format->setEnabled(uncEnabled);
    this->ui->browse_uncommon_out->setEnabled(uncEnabled);
    //this->ui->check_unc->setEnabled(uncEnabled);

    // Rare
    bool rareEnabled = config->getRareWrite();
    this->ui->edit_rare_out->setText(config->getRareLocation());
    this->ui->check_rare->setChecked(rareEnabled);
    this->ui->edit_rare_format->setText(config->getRareFormat());

    this->ui->label_rare_out->setEnabled(rareEnabled);
    this->ui->label_rare_format->setEnabled(rareEnabled);
    this->ui->edit_rare_out->setEnabled(rareEnabled);
    this->ui->edit_rare_format->setEnabled(rareEnabled);
    this->ui->browse_rare_out->setEnabled(rareEnabled);
    //this->ui->check_rare->setEnabled(rareEnabled);


}

void Options::showWithConfig() {
    this->ui->edit_saves->setText(config->getSavesLocation());
    this->ui->edit_runs->setText(config->getRunsLocation());

    loadPotionsFromConfig();
    loadCardRaritiesFromConfig();

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
    this->ui->label_potion_overrides->setEnabled(potionEnabled);

    // sozu / wbs
    this->ui->check_sozu->setEnabled(potionEnabled);
    this->ui->check_wbs->setEnabled(potionEnabled);

    // TODO: fix the logic here. (don't want to think so hard right now)
    this->ui->edit_potion_format_sozu->setEnabled(potionEnabled &&
                                                  this->ui->check_sozu->isChecked());
    this->ui->edit_potion_format_wbs->setEnabled(potionEnabled &&
                                                 this->ui->check_wbs->isChecked());

//    if (potionEnabled) {
//        this->ui->edit_potion_format_sozu->setEnabled(config->getSozuOverride());
//        this->ui->edit_potion_format_wbs->setEnabled(config->getWbsOverride());
//    }
//    else {
//        this->ui->edit_potion_format_sozu->setEnabled(false);
//        this->ui->edit_potion_format_wbs->setEnabled(false);
//    }

}

void Options::onUncToggle() {
    bool uncEnabled = this->ui->check_unc->isChecked();

    this->ui->label_unc_out->setEnabled(uncEnabled);
    this->ui->label_unc_format->setEnabled(uncEnabled);
    this->ui->edit_unc_out->setEnabled(uncEnabled);
    this->ui->edit_unc_format->setEnabled(uncEnabled);
    this->ui->browse_uncommon_out->setEnabled(uncEnabled);
    //this->ui->check_unc->setEnabled(uncEnabled);
}

void Options::onRareToggle() {
    bool rareEnabled = this->ui->check_rare->isChecked();

    this->ui->label_rare_out->setEnabled(rareEnabled);
    this->ui->label_rare_format->setEnabled(rareEnabled);
    this->ui->edit_rare_out->setEnabled(rareEnabled);
    this->ui->edit_rare_format->setEnabled(rareEnabled);
    this->ui->browse_rare_out->setEnabled(rareEnabled);
    //this->ui->check_rare->setEnabled(rareEnabled);
}

void Options::onSozuToggle() {
    this->ui->edit_potion_format_sozu->setEnabled(this->ui->check_sozu->isChecked());
}

void Options::onWbsToggle() {
    this->ui->edit_potion_format_wbs->setEnabled(this->ui->check_wbs->isChecked());
}



























