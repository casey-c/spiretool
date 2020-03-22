#ifndef STREAKS_H
#define STREAKS_H

#include <QString>
#include <QDir>
#include <QVector>
#include <QPair>

const int NUM_CHARACTERS = 4;

typedef struct CHARACTER {
    QString folderName;
    int count;

    int streak;
    bool isWinStreak;
    bool requiresUpdate;

    int totalWins;
    int totalLosses;
    int longestWinStreak;
    int longestLossStreak;
} CHARACTER;

class Streaks
{
public:
    Streaks(QString runDir);

    bool refresh();
    void checkForReset(QString newDir);

    // Getters for formatted UI strings of the stored data
    QString getRotCurrString() { return formatRotOutput(); }
    QString getIroncladCurrString() { return formatOutput(IRONCLAD); }
    QString getSilentCurrString() { return formatOutput(SILENT); }
    QString getDefectCurrString() { return formatOutput(DEFECT); }
    QString getWatcherCurrString() { return formatOutput(WATCHER); }

    QString getRotTotalString();
    QString getIroncladTotalString() { return QString("%1").arg(chars[IRONCLAD]->count); }
    QString getSilentTotalString() { return QString("%1").arg(chars[SILENT]->count); }
    QString getDefectTotalString() { return QString("%1").arg(chars[DEFECT]->count); }
    QString getWatcherTotalString() { return QString("%1").arg(chars[WATCHER]->count); }

    // Overalls
    QString getRotTotalWins() { return QString("%1").arg(totalWins); }
    QString getRotTotalLosses() { return QString("%1").arg(totalLosses); }

    QString getIroncladTotalWins() { return QString("%1").arg(chars[IRONCLAD]->totalWins); }
    QString getIroncladTotalLosses() { return QString("%1").arg(chars[IRONCLAD]->totalLosses); }

    QString getSilentTotalWins() { return QString("%1").arg(chars[SILENT]->totalWins); }
    QString getSilentTotalLosses() { return QString("%1").arg(chars[SILENT]->totalLosses); }

    QString getDefectTotalWins() { return QString("%1").arg(chars[DEFECT]->totalWins); }
    QString getDefectTotalLosses() { return QString("%1").arg(chars[DEFECT]->totalLosses); }

    QString getWatcherTotalWins() { return QString("%1").arg(chars[WATCHER]->totalWins); }
    QString getWatcherTotalLosses() { return QString("%1").arg(chars[WATCHER]->totalLosses); }

    // Overall Streaks
    QString getIroncladLongestWinStreak() { return QString("%1-0").arg(chars[IRONCLAD]->longestWinStreak); }
    QString getIroncladLongestLossStreak() { return QString("0-%1").arg(chars[IRONCLAD]->longestLossStreak); }

    QString getSilentLongestWinStreak() { return QString("%1-0").arg(chars[SILENT]->longestWinStreak); }
    QString getSilentLongestLossStreak() { return QString("0-%1").arg(chars[SILENT]->longestLossStreak); }

    QString getDefectLongestWinStreak() { return QString("%1-0").arg(chars[DEFECT]->longestWinStreak); }
    QString getDefectLongestLossStreak() { return QString("0-%1").arg(chars[DEFECT]->longestLossStreak); }

    QString getWatcherLongestWinStreak() { return QString("%1-0").arg(chars[WATCHER]->longestWinStreak); }
    QString getWatcherLongestLossStreak() { return QString("0-%1").arg(chars[WATCHER]->longestLossStreak); }

    // Win rate
    QString getIroncladWinRate() { return formattedWinRate(chars[IRONCLAD]->totalWins, chars[IRONCLAD]->totalLosses); }
    QString getSilentWinRate() { return formattedWinRate(chars[SILENT]->totalWins, chars[SILENT]->totalLosses); }
    QString getDefectWinRate() { return formattedWinRate(chars[DEFECT]->totalWins, chars[DEFECT]->totalLosses); }
    QString getWatcherWinRate() { return formattedWinRate(chars[WATCHER]->totalWins, chars[WATCHER]->totalLosses); }
    QString getRotWinRate() { return formattedWinRate(totalWins, totalLosses); }


private:
    QString runDir;
    CHARACTER* chars[4];
    int rotStreak;
    bool rotIsWinStreak;
    bool requiresUpdate;

    int totalWins;
    int totalLosses;
    int totalRuns;

    enum Character {
        IRONCLAD,
        SILENT,
        DEFECT,
        WATCHER
    };

    // updates
    void checkForNewFiles();
    void clearUpdates();

    bool updateCurrStreak();
    bool updateOverall();
    //bool updateWinsAndLosses();

    void initialize();
    bool updateCounts(int index);
    void updateStreakFor(int index);
    void updateOverallFor(int index);

    void updateRotatingStreak();
    void updateBestRotatingStreak();

    // TODO: rename
    QString formatOutput(int index);
    QString formatRotOutput();

    QString formattedWinRate(int wins, int losses);

    QPair<int, bool> computeStreak(QStringList saveFilePaths);
    QPair< QPair<int, int>, QPair<int, int> > computeOverall(QStringList saveFilePaths);
};

#endif // STREAKS_H
