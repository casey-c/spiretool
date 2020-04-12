#include "eventdisplay.h"
#include "ui_eventdisplay.h"

EventDisplay::EventDisplay(QWidget *parent) :
    QPushButton(parent),
    ui(new Ui::EventDisplay)
{
    ui->setupUi(this);
}

EventDisplay::~EventDisplay()
{
    delete ui;
}

void EventDisplay::setChances(double event, double monster, double shop, double chest) {
    this->ui->label_event_chance->setText(QString("%1\%").arg(QString::number(event, 'f', 0)));
    this->ui->label_monster_chance->setText(QString("%1\%").arg(QString::number(monster, 'f', 0)));
    this->ui->label_shop_chance->setText(QString("%1\%").arg(QString::number(shop, 'f', 0)));
    this->ui->label_chest_chance->setText(QString("%1\%").arg(QString::number(chest, 'f', 0)));
}
