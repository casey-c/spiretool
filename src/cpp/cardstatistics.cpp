#include "src/h/cardstatistics.h"

CardStatistics::CardStatistics(Card* card) :
    card(card),
    timesSeen(0),
    timesPicked(0),
    timesChoseOther(0),
    timesSkipped(0)
{

}

CardStatistics::operator QString() {
    return QString("Pick: %1 | Skip %2 | Seen %3").arg(timesPicked).arg(timesSkipped).arg(timesSeen);
}
