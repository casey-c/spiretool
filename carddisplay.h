#ifndef CARDDISPLAY_H
#define CARDDISPLAY_H

#include <QWidget>
#include <QPushButton>

namespace Ui {
class CardDisplay;
}

class CardDisplay : public QPushButton
{
    Q_OBJECT

public:
    explicit CardDisplay(QWidget *parent = nullptr);
    ~CardDisplay();

    void setRareChances(double rare, double eliteRare);

private:
    Ui::CardDisplay *ui;
};

#endif // CARDDISPLAY_H
