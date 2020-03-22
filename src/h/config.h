#ifndef CONFIG_H
#define CONFIG_H

#include <QString>


class Config
{
public:
    Config(QString filename);

    void updateConfig(QString savesLocation,
                      QString runsLocation,
                      bool potionWrite,
                      QString potionOut,
                      QString potionFormat,
                      bool sozuOverride,
                      bool wbsOverride,
                      QString sozuFormat,
                      QString wbsFormat
                      );

    bool getPotionWrite() { return potionWrite; }
    QString getPotionOut() { return potionOut; }
    QString getPotionFormat() { return potionFormat; }
    QString getSavesLocation() { return savesLocation; }
    QString getRunsLocation() { return runsLocation; }

    bool getSozuOverride() { return sozuOverride; }
    bool getWbsOverride() { return wbsOverride; }
    QString getSozuFormat() { return sozuFormat; }
    QString getWbsFormat() { return wbsFormat; }

private:
    QString configLocation;
    QString savesLocation;
    QString runsLocation;

    bool potionWrite;
    QString potionOut;
    QString potionFormat;
    bool sozuOverride;
    bool wbsOverride;
    QString sozuFormat;
    QString wbsFormat;

    void loadFromFile();
    void writeToFile();

};

#endif // CONFIG_H
