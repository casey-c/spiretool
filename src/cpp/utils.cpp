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

enum RARITY {
  COMMON,
  UNCOMMON,
  RARE
};

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
double Utils::calculateAtLeast1Common(int cbr, bool hasQuestionCard, bool hasBustedCrown, bool hasNloths) {
  return calcAtLeast1Of3(cbr, COMMON);
}

double Utils::calculateAtLeast1Uncommon(int cbr, bool hasQuestionCard, bool hasBustedCrown, bool hasNloths) {
  return calcAtLeast1Of3(cbr, UNCOMMON);
}

double Utils::calculateAtLeast1Rare(int cbr, bool hasQuestionCard, bool hasBustedCrown, bool hasNloths) {
  return calcAtLeast1Of3(cbr, RARE);
}




































