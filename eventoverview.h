#ifndef EVENTOVERVIEW_H
#define EVENTOVERVIEW_H

#include <QWidget>

namespace Ui {
class EventOverview;
}

class EventOverview : public QWidget
{
    Q_OBJECT

public:
    explicit EventOverview(QWidget *parent = nullptr);
    ~EventOverview();

private:
    Ui::EventOverview *ui;
};

#endif // EVENTOVERVIEW_H
