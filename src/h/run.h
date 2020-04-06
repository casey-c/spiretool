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

    bool hasSozuRelic() { return hasSozu; }
    bool hasWhiteBeastStatue() { return hasWBS; }

    // Special formatted getters
    QString getCharNameAndFloor();
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

    QString filename;
    long date;
    QString char_name;

    bool is_asc;
    bool post_combat;

    int asc_level;
    int floor_reached;

    int potion_chance;
    int cbr;

    QVector<QString> current_relics;
    bool hasSozu;
    bool hasWBS;

    //QVector<CardDetails*> cards;
    //QVector<RelicDetails*> relics;
    //QVector<int> gold_at_floor;
    //QVector<int> gold_gained_at_floor;
    //QVector<int> current_hp;
    //QVector<int> max_hp;
};

#endif // RUN_H