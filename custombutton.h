#ifndef CUSTOMBUTTON_H
#define CUSTOMBUTTON_H

#include <QWidget>
#include <QPushButton>

namespace Ui {
class CustomButton;
}

class CustomButton : public QPushButton
{
    Q_OBJECT

public:
    explicit CustomButton(const QString &text, const QPixmap &pix, QWidget *parent = nullptr);
    ~CustomButton();

    void setText(const QString& text);

private:
    Ui::CustomButton *ui;
};

#endif // CUSTOMBUTTON_H
