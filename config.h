#ifndef CONFIG_H
#define CONFIG_H

#include <QString>


class Config
{
public:
    Config(QString filename);

    void updateConfig(QString savesLocation, bool potionWrite, QString potionOut, QString potionFormat);

    bool getPotionWrite() { return potionWrite; }
    QString getPotionOut() { return potionOut; }
    QString getPotionFormat() { return potionFormat; }
    QString getSavesLocation() { return savesLocation; }

private:
    QString configLocation;
    QString savesLocation;

    bool potionWrite;
    QString potionOut;
    QString potionFormat;

    void loadFromFile();
    void writeToFile();

};

#endif // CONFIG_H
