#include "src/h/referencewindow.h"
#include "ui_referencewindow.h"

ReferenceWindow::ReferenceWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReferenceWindow)
{
    ui->setupUi(this);
}

ReferenceWindow::~ReferenceWindow()
{
    delete ui;
}
