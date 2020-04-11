#include "src/h/tutorial.h"
#include "ui_tutorial.h"
#include <QShortcut>

Tutorial::Tutorial(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Tutorial)
{
    ui->setupUi(this);
    connect( new QShortcut(QKeySequence("Q"), this), &QShortcut::activated, this, &Tutorial::close );
}

Tutorial::~Tutorial()
{
    delete ui;
}
