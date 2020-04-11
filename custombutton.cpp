#include "custombutton.h"
#include "ui_custombutton.h"

#include <QDebug>
//#inclue <QCursor>

CustomButton::CustomButton(const QString& text, const QPixmap &pix, QWidget *parent) :
    QPushButton(parent),
    ui(new Ui::CustomButton)
{
    ui->setupUi(this);

    this->setText(text);

    QPixmap s = pix.scaled(32, 32);
    this->ui->label_img->setPixmap(s);
}

CustomButton::~CustomButton()
{
    delete ui;
}


void CustomButton::setText(const QString &text) {
    this->ui->label->setText(text);
}
