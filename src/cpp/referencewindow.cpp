#include "src/h/referencewindow.h"
#include "ui_referencewindow.h"

#include <QShortcut>
#include <QDebug>

ReferenceWindow::ReferenceWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReferenceWindow)
{
    ui->setupUi(this);

    //QShortcut *shortcut = new QShortcut(QKeySequence("Q"), this);
    //connect( shortcut, &QShortcut::activated, this, &ReferenceWindow::closeThisWindow );
    connect( new QShortcut(QKeySequence("Q"), this), &QShortcut::activated, this, &ReferenceWindow::close );
}

ReferenceWindow::~ReferenceWindow()
{
    delete ui;
}
