#ifndef REFERENCEWINDOW_H
#define REFERENCEWINDOW_H

#include <QWidget>

namespace Ui {
class ReferenceWindow;
}

class ReferenceWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ReferenceWindow(QWidget *parent = nullptr);
    ~ReferenceWindow();

private:
    Ui::ReferenceWindow *ui;

};

#endif // REFERENCEWINDOW_H
