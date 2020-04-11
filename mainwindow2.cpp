#include "mainwindow2.h"
#include "ui_mainwindow2.h"

#include "potiondisplay.h"
#include "carddisplay.h"

MainWindow2::MainWindow2(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow2)
{
    ui->setupUi(this);

//    CustomButton* button = new CustomButton(this, "Runs");
//    this->ui->layout_options->addWidget(button);
//    this->ui->layout_options->addWidget(new CustomButton(this, "statistics"));
//    this->ui->layout_options->addWidget(new CustomButton(this, "help"));
//    this->ui->layout_options->addWidget(new CustomButton(this, "options"));
//    this->ui->layout_options->addStretch(1);

//    this->ui->layout_potions->addWidget(new PotionDisplay());

//    this->ui->layout_cards->addWidget(new CardDisplay());

    //this->ui->gridLayout->addItem(new QSpacerItem(Qt::Horizontal));

}

MainWindow2::~MainWindow2()
{
    delete ui;
}
