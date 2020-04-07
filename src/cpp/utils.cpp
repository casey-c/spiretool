#include "src/h/utils.h"
#include <QFile>
#include <QDebug>
#include <QJsonDocument>
#include <QDir>


QJsonObject Utils::readSaveFileToObject(QString file) {
    QFile loadFile(file);

    if (!loadFile.open(QIODevice::ReadOnly)) {
      qWarning() << "ERROR: could not read save file";
      return QJsonObject();
    }
    else {
      QByteArray data = loadFile.readAll();
      QJsonDocument loadDoc(QJsonDocument::fromJson(data));

      return loadDoc.object();
    }
}

QString Utils::getMostRecentlyUpdatedFile(QString directory) {
  QDir dir(directory);
  if (dir.exists()) {
    QStringList filters;
    filters << "*.autosaveBETA";

    dir.setSorting(QDir::Time);
    dir.setNameFilters(filters);

    QStringList l = dir.entryList();
    if (l.size() >= 1) {
      return l.first();
    }
  }
  return QString();
}

QString Utils::extractCharacterName(QString file) {
  QStringList list = file.split(".");
  if (list.size() > 1) {
    return list[0];
  }
  else {
    return QString("Unknown Character");
  }
}

QString Utils::formatPotion(int chance, QString format) {
  QString chance_string = QString("%1").arg(chance);

  QStringList list = format.split("$");
  QStringList* final = new QStringList();

  for (QString s : list) {
    final->push_back(s);
    final->push_back(chance_string);
  }
  if (!final->empty())
    final->pop_back();

  QString combined = QString();

  for (QString s : *final) {
    combined = QString("%1%2").arg(combined).arg(s);
  }

  return combined;
}

QString Utils::formatStrings(QString chance, QString format) {
  QStringList list = format.split("$");
  QStringList* final = new QStringList();

  for (QString s : list) {
    final->push_back(s);
    final->push_back(chance);
  }

  if (!final->empty())
    final->pop_back();

  QString combined = QString();

  for (QString s : *final) {
    combined = QString("%1%2").arg(combined).arg(s);
  }

  return combined;
}

QStringList Utils::getFilesSortedByNameEarliestFirst(QString directory) {
  QDir dir(directory);

  if (dir.exists()) {
    QStringList filters;
    filters << "*.run";

    //dir.setSorting(QDir::Name | QDir::Reversed);
    dir.setSorting(QDir::Name);
    dir.setNameFilters(filters);

    return dir.entryList();
  }

  return QStringList();
}

QString Utils::docGetQStringOr(QJsonObject obj, QString key, QString def) {
  if (obj.contains(key) && obj[key].isString())
    return obj[key].toString();
  else {
    //qDebug() << "ERROR: \n" << obj << "\ndoes not contain " << key;
    qDebug() << "ERROR: " << "does not contain " << key;
    return def;
  }
}

double Utils::docGetDoubleOr(QJsonObject obj, QString key, double def) {
  if (obj.contains(key) && obj[key].isDouble())
    return obj[key].toDouble();
  else
    return def;
}

long Utils::docGetLongOr(QJsonObject obj, QString key, long def) {
  if (obj.contains(key) && obj[key].isDouble())
    return (long)obj[key].toDouble();
  else
    return def;
}

int Utils::docGetIntOr(QJsonObject obj, QString key, int def) {
  if (obj.contains(key))
    return obj[key].toInt();
  else
    return def;
}

bool Utils::docGetBoolOr(QJsonObject obj, QString key, bool def) {
  if (obj.contains(key) && obj[key].isBool())
    return obj[key].toBool();
  else
    return def;
}


////////////////////////////////////////////////////////////////////////////////

double calcProbSequence(QVector<Utils::RARITY> sequence, int cbr, int rare_chance) {
  double final = 1.0;

  int uncommon_chance = 37;
  int combined_chance = rare_chance + uncommon_chance;

  for (Utils::RARITY r : sequence) {
    double pr_rare = ((double)rare_chance - (double)cbr) / 100.0;
    if (pr_rare < 0.0) pr_rare = 0.0;

    double pr_uncommon = (((double)combined_chance - (double)cbr) / 100.0) - pr_rare;
    double pr_common = 1.0 - (pr_rare + pr_uncommon);

    switch(r) {
    case Utils::COMMON:
      final *= pr_common;
      cbr -= 1;
      if (cbr < -40)
        cbr = -40;
      break;
    case Utils::UNCOMMON:
      final *= pr_uncommon;
      break;
    case Utils::RARE:
      final *= pr_rare;
      cbr = 5;
      break;
    }
  }

  return final;
}

// Too difficult to explain well, but it generates a vector of rarities
// e.g. 000,001,010,011, etc.
// with strides of length len (the number of cards in the reward set)
// basically, this is used as a way to enumerate all possible sequences of
// rewards ==> something like listing: COMMON, COMMON, RARE or
//   RARE, UNCOMMON, RARE  or any of the 3^len number of possibilities
// these subsequences e.g. 001 (common, common, uncommon) are contained in the
// final output vector all mushed next to each other. we'll need to stride
// them out again. I did it this way because it was the easiest recursion I
// could think of. It's an exponentially growing algo but it's okay cause len
// is 8 at absolute max (see 8 cards with both prayerwheel & qcard)
QVector<Utils::RARITY> recursiveTripleGen(int len) {
    if (len <= 1)
        return QVector<Utils::RARITY>({Utils::COMMON, Utils::UNCOMMON, Utils::RARE});
    else {
        QVector<Utils::RARITY> lower = recursiveTripleGen(len - 1);
        QVector<Utils::RARITY> final;

        int stride = len - 1;
        for (int i = 0; i < lower.length(); i += stride) {
            QVector<Utils::RARITY> strided;
            for (int j = 0; j < stride; ++j)
                strided.push_back(lower[i+j]);

            final.push_back(Utils::COMMON);
            final.append(strided);

            final.push_back(Utils::UNCOMMON);
            final.append(strided);

            final.push_back(Utils::RARE);
            final.append(strided);
        }

        return final;
    }
}

double Utils::calculateProb(int cbr, int numCards, int rareChance, RARITY rarity) {
    double final_prob = 0.0;

    QVector<RARITY> all = recursiveTripleGen(numCards);
    for (int i = 0; i < all.length(); i += numCards) {
        // Get the current sequence using a stride of length numCards
        QVector<RARITY> sequence;
        for (int j = 0; j < numCards; ++j)
            sequence.push_back(all[i+j]);

        // Verify that this sequence contains the target rarity type
        if (!sequence.contains(rarity))
            continue;

        // Calculate the probability of this sequence occuring
        final_prob += calcProbSequence(sequence, cbr, rareChance);
    }

    return final_prob;
}

int Utils::determineNumRewardCards(bool hasQuestionCard, bool hasBustedCrown, bool hasPrayerWheel) {
    int numCards = 3;
    if (hasBustedCrown) numCards -= 2;
    if (hasQuestionCard) numCards += 1;
    if (hasPrayerWheel) numCards *= 2;

    return numCards;
}

int Utils::determineRareChance(bool hasNloths) {
    int rareChance = 3;
    if (hasNloths) rareChance *= 3;

    return rareChance;
}

#if 0
//enum RARITY {
//  COMMON,
//  UNCOMMON,
//  RARE
//};

double calcProbSequence(QVector<RARITY> sequence, int cbr) {
  double final = 1.0;

  int rare_chance = 3;
  int uncommon_chance = 37;
  int combined_chance = rare_chance + uncommon_chance;

  for (RARITY r : sequence) {
    double pr_rare = ((double)rare_chance - (double)cbr) / 100.0;
    if (pr_rare < 0.0) pr_rare = 0.0;

    double pr_uncommon = (((double)combined_chance - (double)cbr) / 100.0) - pr_rare;
    double pr_common = 1.0 - (pr_rare + pr_uncommon);

    switch(r) {
    case COMMON:
      final *= pr_common;
      cbr -= 1;
      if (cbr < -40)
        cbr = -40;
      break;
    case UNCOMMON:
      final *= pr_uncommon;
      break;
    case RARE:
      final *= pr_rare;
      cbr = 5;
      break;
    }
  }

  return final;
}

RARITY rarityFromInt(int i) {
  if (i == 0)
    return COMMON;
  else if (i == 1)
    return UNCOMMON;
  else
    return RARE;
}

QVector<RARITY> rarity3Vec(int i, int j, int k) {
  QVector<RARITY> result;
  result.push_back(rarityFromInt(i));
  result.push_back(rarityFromInt(j));
  result.push_back(rarityFromInt(k));
  return result;
}

QVector<RARITY> recursiveTripleGen(int len) {
    if (len <= 1)
        return QVector<RARITY>({COMMON, UNCOMMON, RARE});
    else {
        QVector<RARITY> lower = recursiveTripleGen(len - 1);
        QVector<RARITY> final;

        int stride = len - 1;
        for (int i = 0; i < lower.length(); i += stride) {
            QVector<RARITY> strided;

            for (int j = 0; j < stride; ++j) {
                strided.push_back(lower[i+j]);
            }

//            qDebug() << "stride len" << strided.length();
//            for (RARITY r : strided)
//                qDebug() << "strided r: " << r;
//            qDebug() << "---";

            final.push_back(COMMON);
            //for (RARITY r : strided) final.push_back(r);
            final.append(strided);

            final.push_back(UNCOMMON);
            //for (RARITY r : strided) final.push_front(r);
            final.append(strided);

            final.push_back(RARE);
            //for (RARITY r : strided) final.push_front(r);
            final.append(strided);
        }


        return final;
    }
}

double calcAtLeast1Of3(int cbr, RARITY r) {
  double final_prob = 0.0;

  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
      for (int k = 0; k < 3; ++k) {
        QVector<RARITY> sequence = rarity3Vec(i, j, k);
        if (!sequence.contains(r))
          continue;
        else
          final_prob += calcProbSequence(sequence, cbr);
      }
    }
  }

  return final_prob;
}



// TODO : implement relic interactions
double Utils::calculateAtLeast1Common(int cbr, bool hasQuestionCard, bool hasBustedCrown, bool hasPrayerWheel, bool hasNloths) {
    int numCards = 3;
    if (hasBustedCrown) numCards -= 2;
    if (hasQuestionCard) numCards += 1;
    if (hasPrayerWheel) numCards *= 2;

    int rareChance = 3;
    if (hasNloths) rareChance *= 3;

//    QVector<RARITY> test = recursiveTripleGen(4);

//    qDebug() << "recursion test has len" << test.length();
//    for (int i  = 0; i < test.length(); i+=4) {
//        qDebug() << test[i] << test[i+1] << test[i+2] << test[i+3];
//    }
//    qDebug() << "----";

  return calcAtLeast1Of3(cbr, COMMON);
}

double Utils::calculateAtLeast1Uncommon(int cbr, bool hasQuestionCard, bool hasBustedCrown, bool hasNloths) {
  return calcAtLeast1Of3(cbr, UNCOMMON);
}

double Utils::calculateAtLeast1Rare(int cbr, bool hasQuestionCard, bool hasBustedCrown, bool hasNloths) {
  return calcAtLeast1Of3(cbr, RARE);
}



#endif

































