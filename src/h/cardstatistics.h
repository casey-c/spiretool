#ifndef CARDSTATISTICS_H
#define CARDSTATISTICS_H

#include <QString>
#include "src/h/card.h"

class CardStatistics
{
public:
    CardStatistics(Card* card);

    void pick() { ++timesPicked; see(); }
    void chooseOther() { ++timesChoseOther; see(); }
    void skip() { ++timesSkipped; see(); }

    //Card* getCard() { return card; }
    QString getName() { return card->getCardName(); }
    QString getClassName() { return card->getClassName(); }

    int getTimesSeen() { return timesSeen; }
    int getTimesPicked() { return timesPicked; }
    //int getTimesChoseOther() { return timesSeen - timesSkipped - timesPicked; }
    int getTimesSkipped() { return timesSkipped; }

    operator QString();

private:
    Card* card;
    void see() { ++timesSeen; }

    int timesSeen, timesPicked, timesChoseOther, timesSkipped;
};

#endif // CARDSTATISTICS_H
