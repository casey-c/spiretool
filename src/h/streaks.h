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
} CHARACTER;

class Streaks
{
public:
    Streaks(QString runDir);

    bool updateStreak();

    QString getIroncladCurrString() { return formatOutput(IRONCLAD); }
    QString getSilentCurrString() { return formatOutput(SILENT); }
    QString getDefectCurrString() { return formatOutput(DEFECT); }
    QString getWatcherCurrString() { return formatOutput(WATCHER); }

    QString getRotCurrString() { return formatRotOutput(); }

    void checkForReset(QString newDir);

private:
    QString runDir;
    CHARACTER* chars[4];
    int rotStreak;
    bool rotIsWinStreak;

    enum Character {
        IRONCLAD,
        SILENT,
        DEFECT,
        WATCHER
    };

    void initialize();
    bool updateCounts(int index);
    void updateStreakFor(int index);
    void updateRotatingStreak();

    QString formatOutput(int index);
    QString formatRotOutput();
    QPair<int, bool> computeStreak(QStringList saveFilePaths);
};

#endif // STREAKS_H
