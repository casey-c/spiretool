#ifndef CARDDATABASE_H
#define CARDDATABASE_H

#include <QMap>
#include <QString>
#include "src/h/card.h"
#include "src/h/cardstatistics.h"

class CardDatabase
{
public:
    CardDatabase();
    void print();

private:
    QMap<QString, Card*> cards;
    QMap<QString, CardStatistics*> cardStats;

    void loadCardsFromFile(QString filename);

    void checkRun(QString filename);
};

#endif // CARDDATABASE_H
