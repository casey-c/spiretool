#ifndef EVENTDISPLAY_H
#define EVENTDISPLAY_H

#include <QWidget>
#include <QPushButton>

namespace Ui {
class EventDisplay;
}

class EventDisplay : public QPushButton
{
    Q_OBJECT

public:
    explicit EventDisplay(QWidget *parent = nullptr);
    ~EventDisplay();

    void setChances(double event, double monster, double shop, double chest);

private:
    Ui::EventDisplay *ui;
};

#endif // EVENTDISPLAY_H
