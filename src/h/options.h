#ifndef OPTIONS_H
#define OPTIONS_H

#include <QWidget>
#include "config.h"

namespace Ui {
class Options;
}

class Options : public QWidget
{
    Q_OBJECT

public:
    explicit Options(Config* config, QWidget *parent = nullptr);
    ~Options();

    void showWithConfig();

private:
    Ui::Options *ui;
    Config* config;

    void loadPotionsFromConfig();

private slots:
    void browseSaves();
    void browseRuns();
    void browsePotionFile();

    void saveAndExit();
    void onPotionToggle();
};

#endif // OPTIONS_H
