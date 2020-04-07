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
    void loadCardRaritiesFromConfig();

private slots:
    void browseSaves();
    void browseRuns();
    void browsePotionFile();

    void browseUnc();
    void browseRare();

    void saveAndExit();
    void onPotionToggle();

    void onSozuToggle();
    void onWbsToggle();

    void onUncToggle();
    void onRareToggle();
};

#endif // OPTIONS_H
