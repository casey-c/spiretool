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

    operator QString();

private:
    Card* card;
    void see() { ++timesSeen; }

    int timesSeen, timesPicked, timesChoseOther, timesSkipped;
};

#endif // CARDSTATISTICS_H
