#include "src/h/about.h"
#include "ui_about.h"

#include <QShortcut>

About::About(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);
    connect( new QShortcut(QKeySequence("Q"), this), &QShortcut::activated, this, &About::close );
}

About::~About()
{
    delete ui;
}
