#include <QtDebug>

#include "src/h/streaks.h"
#include "src/h/utils.h"

// TODO: fix crash / fail to boot afterwards if the directory for runs is
// incorrect. This crash will not happen with the saves directory so I did
// something wrong with this one somewhere.

Streaks::Streaks(QString runDir) :
    runDir(runDir),
    rotStreak(0),
    rotIsWinStreak(true)
{
  initialize();

}

// Sets up the character structs to store the streak info
void Streaks::initialize() {
  for (int i = 0; i < NUM_CHARACTERS; ++i) {
    chars[i] = new CHARACTER;
    chars[i]->count = -1;
    chars[i]->streak = 0;
    chars[i]->isWinStreak = true;
  }

  if (!runDir.endsWith("/"))
    runDir.append('/');

  chars[IRONCLAD]->folderName = QString("%1%2").arg(runDir).arg("IRONCLAD/");
  chars[SILENT]->folderName = QString("%1%2").arg(runDir).arg("THE_SILENT/");
  chars[DEFECT]->folderName = QString("%1%2").arg(runDir).arg("DEFECT/");
  chars[WATCHER]->folderName = QString("%1%2").arg(runDir).arg("WATCHER/");

  qDebug() << "done streak initialization";
}

void Streaks::checkForReset(QString newDir) {
    if (!newDir.endsWith("/"))
      newDir.append('/');

    if (newDir != runDir) {
        qDebug() << "need to update directory";
        for (int i = 0; i < NUM_CHARACTERS; ++i)
            delete chars[i];

        // Remake with the new directory
        this->runDir = newDir;
        initialize();
    }
}


bool Streaks::updateCounts(int index) {
  QStringList runs = Utils::getFilesSortedByNameEarliestFirst(chars[index]->folderName);
  if (chars[index]->count != runs.size()) {
    qDebug() << "index " <<index << "count " << chars[index]->count << " runs size" << runs.size();
    chars[index]->count = runs.size();
    return true;
  }
  return false;
}

bool Streaks::updateStreak() {
  bool needsUpdate = false;

  for (int i = 0; i < NUM_CHARACTERS; ++i) {
    if (updateCounts(i)) {
      updateStreakFor(i);
      needsUpdate = true;
    }
  }

  if (needsUpdate)
    updateRotatingStreak();

  return needsUpdate;
}

void Streaks::updateStreakFor(int index) {
    QString folder = chars[index]->folderName;
    QStringList runs = Utils::getFilesSortedByNameEarliestFirst(folder);
    if (runs.isEmpty()) {
        qDebug() << "runs list is empty update streak, quitting early" << index;
        return;
    }

    if (!folder.endsWith('/'))
        folder.append('/');

    QStringList withFolder = QStringList();
    for (QString run : runs) {
      withFolder.push_back(run.prepend(folder));
      //qDebug() << "index" << index << run;
    }

    qDebug() << "need to compute now";
    qDebug() << "first was " << withFolder.first();

    QPair<int, bool> computedStreak = computeStreak(withFolder);
    int length = computedStreak.first;
    bool isWinStreak = computedStreak.second;

    qDebug() << index << "Streak: " << length << isWinStreak;

    chars[index]->streak = length;
    chars[index]->isWinStreak = isWinStreak;
}

bool isWin(QString path) {
  QJsonObject obj = Utils::readSaveFileToObject(path);

  if (obj.contains("floor_reached") && obj["floor_reached"].toInt() == 57)
    return true;
  else
    return false;
}


// This combines all the paths for every single character into one and looks
// at the overall pattern of play. We need to sort by the filename, not the
// full path (because other wise the folder names will sort by character still)
// so we use QMap for the intermediate
void Streaks::updateRotatingStreak() {
  QMap<QString, QString> map;

  for (int i = 0; i < NUM_CHARACTERS; ++i) {
    QString folder = chars[i]->folderName;
    if (!folder.endsWith('/'))
      folder.append('/');

    QStringList filenames = Utils::getFilesSortedByNameEarliestFirst(folder);

    for (QString s : filenames) {
      QString full = QString("%1%2").arg(folder).arg(s);
      //qDebug() << "inserting" << full;
      map.insert(s, full);
    }
  }

  QStringList combined = map.values();

  QPair<int, bool> computedStreak = computeStreak(combined);
  int length = computedStreak.first;
  bool isWinStreak = computedStreak.second;

  qDebug() << "Streak: " << length << isWinStreak;
  rotStreak = length;
  rotIsWinStreak = isWinStreak;

  qDebug() << "END COMBINED-----------------------";
}

QString Streaks::formatRotOutput() {
  int wins = (rotIsWinStreak) ? rotStreak : 0;
  int losses = (!rotIsWinStreak) ? rotStreak : 0;
  QString result = QString("%1-%2").arg(wins).arg(losses);
  return result;
}

QString Streaks::formatOutput(int index) {
  CHARACTER* c = chars[index];
  int wins = (c->isWinStreak) ? c->streak : 0;
  int losses = (!c->isWinStreak) ? c->streak : 0;
  QString result = QString("%1-%2").arg(wins).arg(losses);
  return result;
}

// Computes the streak based on a sorted list of save file paths
// The saveFilePaths list is sorted by name, which corresponds to the earliest
// runs first and the newest runs last. So we end up parsing from the back
// towards the front.
//
// A streak is determined by the length of the sequence of paths with the same
// type (win or lose) as the first in the sequence.
// e.g.: this has length 2 and isWin = TRUE
// (end) WIN WIN LOSS WIN .... (begin of saveFilePaths)
// e.g.: while this has length 1 and isWin = FALSE
// (end) LOSS WIN LOSS WIN .... (begin of saveFilePaths)
QPair<int, bool> Streaks::computeStreak(QStringList saveFilePaths) {
  if (saveFilePaths.empty())
    return QPair<int, bool>(0, true);

  int streakLength = 1;
  bool isWinStreak = true;

  QStringList::const_reverse_iterator itr = saveFilePaths.rbegin();

  // The first file will determine the streak type. The streak length is
  // how many files that occur after this item with the same type (win/loss).
  QString first = *itr;
  isWinStreak = isWin(first);
  qDebug() << "******** compute streak *********";
  qDebug() << first << " | first is " << isWinStreak;

  for (++itr; itr != saveFilePaths.rend(); ++itr) {
    if (isWin(*itr) == isWinStreak) {
      ++streakLength;
      qDebug() << *itr << " is part of the streak";
    }
    else {
      qDebug() << *itr << " ends the streak";
      break;
    }
  }


  return QPair<int, bool>(streakLength, isWinStreak);
}


















