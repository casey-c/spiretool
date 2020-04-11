#ifndef UTILS_H
#define UTILS_H

#include <QJsonObject>
#include <QString>

class Utils
{
public:

    enum RARITY {
        COMMON,
        UNCOMMON,
        RARE
    };

    static QJsonObject readSaveFileToObject(QString file);
    static QString getMostRecentlyUpdatedFile(QString directory);
    static QString extractCharacterName(QString file);

    static QString formatPotion(int chance, QString format);
    //static QString formatStrings(QString chance, QString format);

    static QString formatStrings(QString replacement, QString target, QString format);

    static QStringList getFilesSortedByNameEarliestFirst(QString directory);

    static QString docGetQStringOr(QJsonObject obj, QString key, QString def);
    static int docGetIntOr(QJsonObject obj, QString key, int def);
    static double docGetDoubleOr(QJsonObject obj, QString key, double def);
    static long docGetLongOr(QJsonObject obj, QString key, long def);
    static bool docGetBoolOr(QJsonObject obj, QString key, bool def);

    static int determineNumRewardCards(bool hasQuestionCard, bool hasBustedCrown, bool hasPrayerWheel);
    static int determineRareChance(int startChance, bool hasNloths);

    static double calculateProb(int cbr, int numCards, int rareChance, RARITY rarity);

    static void writeStringToFile(QString s, QString filename);

//    static double calculateAtLeast1Rare(int cbr, bool hasQuestionCard, bool hasBustedCrown, bool hasNloths);
//    static double calculateAtLeast1Uncommon(int cbr, bool hasQuestionCard, bool hasBustedCrown, bool hasNloths);
//    static double calculateAtLeast1Common(int cbr, bool hasQuestionCard, bool hasBustedCrown, bool hasNloths);

private:
    Utils() {}
};

#endif // UTILS_H
