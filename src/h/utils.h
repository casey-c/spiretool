#ifndef UTILS_H
#define UTILS_H

#include <QJsonObject>
#include <QString>

class Utils
{
public:
    static QJsonObject readSaveFileToObject(QString file);
    static QString getMostRecentlyUpdatedFile(QString directory);
    static QString extractCharacterName(QString file);
    static QString formatPotion(int chance, QString format);
    static QStringList getFilesSortedByNameEarliestFirst(QString directory);

private:
    Utils() {}
};

#endif // UTILS_H
