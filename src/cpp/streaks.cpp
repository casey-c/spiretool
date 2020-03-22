#include <QtDebug>

#include "src/h/streaks.h"
#include "src/h/utils.h"

////////////////////////////////////////////////////////////////////////////////

// Checks if the save file got to the final VICTORY floor
bool isWin(QString path) {
  QJsonObject obj = Utils::readSaveFileToObject(path);
  return (obj.contains("floor_reached") && obj["floor_reached"].toInt() == 57);
}

////////////////////////////////////////////////////////////////////////////////

// TODO: fix crash / fail to boot afterwards if the directory for runs is
// incorrect. This crash will not happen with the saves directory so I did
// something wrong with this one somewhere.

Streaks::Streaks(QString runDir) :
  runDir(runDir),
  rotStreak(0),
  rotIsWinStreak(true),
  requiresUpdate(true),
  totalWins(0),
  totalLosses(0),
  totalRuns(0)
{
  initialize();

}

// Sets up the character structs to store the streak info
void Streaks::initialize() {
  for (int i = 0; i < NUM_CHARACTERS; ++i) {
    chars[i] = new CHARACTER;

    //chars[i]->count = -1;
    chars[i]->count = 0;

    chars[i]->streak = 0;
    chars[i]->isWinStreak = true;
    chars[i]->requiresUpdate = true;

    chars[i]->totalWins = 0;
    chars[i]->totalLosses = 0;
    chars[i]->longestWinStreak = 0;
    chars[i]->longestLossStreak = 0;
  }

  if (!runDir.endsWith("/"))
    runDir.append('/');

  chars[IRONCLAD]->folderName = QString("%1%2").arg(runDir).arg("IRONCLAD/");
  chars[SILENT]->folderName = QString("%1%2").arg(runDir).arg("THE_SILENT/");
  chars[DEFECT]->folderName = QString("%1%2").arg(runDir).arg("DEFECT/");
  chars[WATCHER]->folderName = QString("%1%2").arg(runDir).arg("WATCHER/");

  qDebug() << "done streak initialization";
}

// If the directory has changed, remake the data structs
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

////////////////////////////////////////////////////////////////////////////////


// Checks to see if the number of runs changes, in which case we're gonna
// need to refresh all of the data
void Streaks::checkForNewFiles() {
    //requiresUpdate = false;
    for (int i = 0; i < NUM_CHARACTERS; ++i) {
        chars[i]->requiresUpdate = updateCounts(i);
        requiresUpdate |= chars[i]->requiresUpdate;
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

// Refresh the streak data
bool Streaks::updateCurrStreak() {
  for (int i = 0; i < NUM_CHARACTERS; ++i) {
    if (chars[i]->requiresUpdate)
      updateStreakFor(i);
  }

  // If any of the individual characters changed, we must also update the
  // combined rotating streak
  if (requiresUpdate)
    updateRotatingStreak();

  return requiresUpdate;
}

// Pick a certain character and update their individual streak
void Streaks::updateStreakFor(int index) {
  QString folder = chars[index]->folderName;
  if (!folder.endsWith('/'))
    folder.append('/');

  // A list of filenames (e.g. 511415.run) in the current folder
  QStringList runs = Utils::getFilesSortedByNameEarliestFirst(folder);
  if (runs.isEmpty())
    return;

  // Add the folder prefix to the filenames to get full paths
  QStringList withFolder = QStringList();
  for (QString run : runs)
    withFolder.push_back(run.prepend(folder));

  // Compute the streak (length, isWin) for those paths
  QPair<int, bool> computedStreak = computeStreak(withFolder);
  int length = computedStreak.first;
  bool isWinStreak = computedStreak.second;

  // Update the stored data
  chars[index]->streak = length;
  chars[index]->isWinStreak = isWinStreak;
}


QString Streaks::getRotTotalString() {
  int sum = 0;
  for (int i = 0; i < NUM_CHARACTERS; ++i)
    sum += chars[i]->count;

  return QString("%1").arg(sum);
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

    for (QString s : filenames)
      map.insert(s, QString("%1%2").arg(folder).arg(s));
  }

  QStringList combined = map.values();

  QPair<int, bool> computedStreak = computeStreak(combined);
  int length = computedStreak.first;
  bool isWinStreak = computedStreak.second;

  qDebug() << "Streak: " << length << isWinStreak;
  rotStreak = length;
  rotIsWinStreak = isWinStreak;

  //qDebug() << "END COMBINED-----------------------";
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

////////////////////////////////////////////////////////////////////////////////

// TODO: refactor this common code from updateCurrStreak() since I copy pasted
// it / 90% the same
bool Streaks::updateOverall() {
  for (int i = 0; i < NUM_CHARACTERS; ++i) {
    if (chars[i]->requiresUpdate)
      updateOverallFor(i);
  }

  // If any of the individual characters changed, we must also update the
  // combined rotating streak
  if (requiresUpdate) {
      totalWins = 0;
      totalLosses = 0;

      for (int i = 0; i < NUM_CHARACTERS; ++i) {
          totalWins += chars[i]->totalWins;
          totalLosses += chars[i]->totalLosses;
      }

      totalRuns = totalWins + totalLosses;

  }

  return requiresUpdate;
}

void Streaks::updateOverallFor(int index) {
  QString folder = chars[index]->folderName;
  if (!folder.endsWith('/'))
    folder.append('/');

  // A list of filenames (e.g. 511415.run) in the current folder
  QStringList runs = Utils::getFilesSortedByNameEarliestFirst(folder);
  if (runs.isEmpty())
    return;

  // Add the folder prefix to the filenames to get full paths
  QStringList withFolder = QStringList();
  for (QString run : runs)
    withFolder.push_back(run.prepend(folder));

  // Compute overall stats (longest win, longest loss, total wins, total losses)
  QPair< QPair<int, int>, QPair<int, int>  > data = computeOverall(withFolder);
  QPair<int, int> lengths = data.first;
  QPair<int, int> totals = data.second;

  // Update the stored data
  chars[index]->totalWins = totals.first;
  chars[index]->totalLosses = totals.second;
  chars[index]->longestWinStreak = lengths.first;
  chars[index]->longestLossStreak = lengths.second;
}


// streaklengths (wins, losses) totals (wins, losses)
QPair< QPair<int, int>, QPair<int, int> > Streaks::computeOverall(QStringList saveFilePaths) {
    if (saveFilePaths.empty())
      return QPair< QPair<int, int>, QPair<int, int> > (QPair<int, int>(0,0), QPair<int, int>(0,0));


    int totalWins = 0;
    int totalLosses = 0;

    int longestWin = 0;
    int longestLoss = 0;

    bool winning = true;
    int currentStreak = 0;

    QStringList::const_reverse_iterator itr = saveFilePaths.rbegin();
    for (; itr != saveFilePaths.rend(); ++itr) {
        QString run = *itr;
        bool runWon = isWin(*itr);
        //qDebug() << "run: " << run << runWon;
        //int trackerChange = runWon ? 1 : -1;

        // Update totals
        totalWins += runWon ? 1 : 0;
        totalLosses += !runWon ? 1 : 0;

        // Update streak
        if (winning == runWon) {
            //qDebug() << "current streak at " << currentStreak;
            ++currentStreak;
        }
        else {
            //qDebug() << "ended streak at " << currentStreak << "now wl" << !winning;

            if (winning && currentStreak > longestWin) {
                longestWin = currentStreak;
            }
            else if (!winning && currentStreak > longestLoss) {
                longestLoss = currentStreak;
            }

            currentStreak = 1;
            winning = !winning;
        }
    }

    // need to update the last step too
    if (winning && currentStreak > longestWin)
      longestWin = currentStreak;
    else if (!winning && currentStreak > longestLoss)
      longestLoss = currentStreak;

    qDebug() << "totalWins" << totalWins;
    qDebug() << "totalLosses" << totalLosses;
    qDebug() << "longestWin" << longestWin;
    qDebug() << "longestLoss" << longestLoss;
    qDebug() << "---";

    QPair<int, int> lengths(longestWin, longestLoss);
    QPair<int, int> totals(totalWins, totalLosses);

    return QPair< QPair<int, int>, QPair<int, int> > (lengths, totals);
}


////////////////////////////////////////////////////////////////////////////////

bool Streaks::refresh() {
  checkForNewFiles();

  if (requiresUpdate) {
    bool anythingChanged = false;
    anythingChanged |= updateCurrStreak();
    anythingChanged |= updateOverall();
    //anythingChanged |= updateWinsAndLosses();


    clearUpdates();
    return anythingChanged;
  }
  else {
    return false;
  }
}

void Streaks::clearUpdates() {
  requiresUpdate = false;
  for (int i = 0; i < NUM_CHARACTERS; ++i)
    chars[i]->requiresUpdate = false;
}

////////////////////////////////////////////////////////////////////////////////

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

QString Streaks::formattedWinRate(int wins, int losses) {
    int total = wins + losses;

    double percent = 0.0;
    if (total != 0) {
        percent = (double)wins / (double)total * 100.0;
    }

    return QString("%1\%").arg(QString::number(percent, 'f', 2));
}














