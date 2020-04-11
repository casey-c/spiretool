#include "potiondisplay.h"
#include "ui_potiondisplay.h"

PotionDisplay::PotionDisplay(QWidget *parent) :
    QPushButton(parent),
    ui(new Ui::PotionDisplay)
{
    ui->setupUi(this);
}

PotionDisplay::~PotionDisplay()
{
    delete ui;
}

void PotionDisplay::setText(const QString &text) {
    this->ui->label_potion_chance->setText(text);
}
