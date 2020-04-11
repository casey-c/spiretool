#include "carddisplay.h"
#include "ui_carddisplay.h"

CardDisplay::CardDisplay(QWidget *parent) :
    QPushButton(parent),
    ui(new Ui::CardDisplay)
{
    ui->setupUi(this);
}

CardDisplay::~CardDisplay()
{
    delete ui;
}

void CardDisplay::setRareChances(double rare, double eliteRare) {
  this->ui->label_rare_chance->setText(QString("%1\%").arg( QString::number(rare, 'f', 2) ));
  this->ui->label_elite_rare_chance->setText(QString("%1\%").arg( QString::number(eliteRare, 'f', 2) ));
}
