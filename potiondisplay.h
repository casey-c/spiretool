#ifndef POTIONDISPLAY_H
#define POTIONDISPLAY_H

#include <QWidget>
#include <QPushButton>

namespace Ui {
class PotionDisplay;
}

class PotionDisplay : public QPushButton
{
    Q_OBJECT

public:
    explicit PotionDisplay(QWidget *parent = nullptr);
    ~PotionDisplay();

    void setText(const QString& text);

private:
    Ui::PotionDisplay *ui;
};

#endif // POTIONDISPLAY_H
