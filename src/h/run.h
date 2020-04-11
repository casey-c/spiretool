#ifndef RUN_H
#define RUN_H

#include <QString>
#include <QPixmap>
#include <QVector>

//#define MAX_FLOORS 57

class Run
{
public:
    static Run* build(QString filename);
    bool refresh();

    void print();

    // Getters
    QString getCharName() { return char_name; }
    QString getFormattedDate();

    int getPotionChance() { return potion_chance; }
    int getCBR() { return cbr; }
    int getCurrentFloor() { return floor_reached; }

    bool isPostCombat() { return post_combat; }

    // Relic getters
    bool hasSozuRelic() { return hasSozu; }
    bool hasWhiteBeastStatue() { return hasWBS; }
    bool hasQuestionCard() { return hasQC; }
    bool hasBustedCrown() { return hasBC; }
    bool hasPrayerWheel() { return hasPW; }
    bool hasNloths() { return hasNL; }

    bool differentFile(QString other) { return filename != other; }

    // Special formatted getters
    QString getCharNameAndFloor();
    QString getBossAndAsc();
    QPixmap getPixmap();

    // Filter support (usage TODO in future update)
    bool ascLevelE(int x) { return asc_level == x; }
    bool ascLevelL(int x) { return asc_level < x; }
    bool ascLevelLE(int x) { return asc_level <= x; }
    bool ascLevelG(int x) { return asc_level > x; }
    bool ascLevelGE(int x) { return asc_level >= x; }

    bool floorReachedE(int x) { return floor_reached == x; }
    bool floorReachedL(int x) { return floor_reached < x; }
    bool floorReachedLE(int x) { return floor_reached <= x; }
    bool floorReachedG(int x) { return floor_reached > x; }
    bool floorReachedGE(int x) { return floor_reached >= x; }

private:
    Run(QString filename);

    QString filename, char_name, boss;
    long date;

    bool is_asc, post_combat;
    bool hasSozu, hasWBS, hasQC, hasBC, hasPW, hasNL;

    int asc_level, floor_reached;
    int potion_chance, cbr;

    // relics
    QVector<QString> current_relics;


    //QVector<CardDetails*> cards;
    //QVector<RelicDetails*> relics;
    //QVector<int> gold_at_floor;
    //QVector<int> gold_gained_at_floor;
    //QVector<int> current_hp;
    //QVector<int> max_hp;
};

#endif // RUN_H
