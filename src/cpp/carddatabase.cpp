#include "src/h/carddatabase.h"
#include "src/h/utils.h"
#include <QFile>
#include <QDebug>

#include <QByteArray>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValueRef>

CardDatabase::CardDatabase() : cards()
{
  //cards = new QMap<QString, Card*>();

  //loadCardsFromFile("ironclad_cards.json");
  //loadCardsFromFile("silent_cards.json");
  //loadCardsFromFile("defect_cards.json");
  //loadCardsFromFile("watcher_cards.json");

  //loadCardsFromFile("ironclad_fixed.json");
  //loadCardsFromFile("silent_fixed.json");
  //loadCardsFromFile("defect_fixed.json");
  //loadCardsFromFile("watcher_fixed.json");

    loadCardsFromFile("databases/cards/ironclad.json");
    loadCardsFromFile("databases/cards/silent.json");
    loadCardsFromFile("databases/cards/defect.json");
    loadCardsFromFile("databases/cards/watcher.json");
    loadCardsFromFile("databases/cards/colorless.json");

  // e.g. can check for cards
  //    if (cards.contains("Feed")) {
  //        qDebug() << "Cards list does indeed contain feed";
  //        Card* feedcard = cards["Feed"];
  //        feedcard->print();
  //    }

  //checkRun("/home/casey/Documents/qt/stats/test.run");
  QMap<QString, bool> map;

//  for (int i = 1; i <= 11; ++i) {
//      QString run = QString("/home/casey/Documents/qt/tests/test%1.run").arg(i);
//      //qDebug() << "run" << run;
//      checkRun(run, map);
//  }

  QString dir = QString("/home/casey/Documents/qt/stats/terrence/IRONCLAD/");
  QStringList runs = Utils::getFilesSortedByNameEarliestFirst(dir);
  for (QString run : runs) { checkRun(QString("%1%2").arg(dir).arg(run)); }

  dir = QString("/home/casey/Documents/qt/stats/terrence/THE_SILENT/");
  runs = Utils::getFilesSortedByNameEarliestFirst(dir);
  for (QString run : runs) { checkRun(QString("%1%2").arg(dir).arg(run)); }

  dir = QString("/home/casey/Documents/qt/stats/terrence/DEFECT/");
  runs = Utils::getFilesSortedByNameEarliestFirst(dir);
  for (QString run : runs) { checkRun(QString("%1%2").arg(dir).arg(run)); }

  dir = QString("/home/casey/Documents/qt/stats/terrence/WATCHER/");
  runs = Utils::getFilesSortedByNameEarliestFirst(dir);
  for (QString run : runs) { checkRun(QString("%1%2").arg(dir).arg(run)); }


  for (QString key : cardStats.keys()) {
      qDebug() << key <<  "\n\t" << *cardStats[key];
  }

//  qDebug() << "EXISTING CARDS:";
//  for (QString key : cards.keys()) {
//    if (cards[key]->isStarter())
//      continue;
//    if (map.contains(key)) {
//      qDebug() << *cards[key];
//    }
//    //    else {
//    //      qDebug() << key << "NOT FOUND";
//    //    }
//  }
//  qDebug() << "--------\nUNKNOWN CARDS:";
//  for (QString key : cards.keys()) {
//    if (cards[key]->isStarter())
//      continue;
//    if (!map.contains(key)) {
//      qDebug() << key;
//    }
//    //    else {
//    //      qDebug() << key << "NOT FOUND";
//    //    }
//  }
//  qDebug() << "--------";
//  for (QString key : map.keys()) {
//      if (!cards.contains(key)) {
//          qDebug() << key;
//      }
//  }
}


void CardDatabase::print() {
  //      for (Card* c : cards) {
  //          c->print();
  //      }

  qDebug() << "Managing" << cards.size() << "cards";
}

void CardDatabase::loadCardsFromFile(QString filename) {
  QFile file(filename);

  int created = 0;

  if (file.exists()) {
    if (file.open(QFile::ReadOnly)) {
      QByteArray data = file.readAll();
      QJsonDocument doc = QJsonDocument::fromJson(data);
      QJsonObject obj = doc.object();

      if (!(obj.contains("char") && obj["char"].isString())) {
        qDebug() << "ERROR: character name not found in json";
        return;
      }
      QString character = obj["char"].toString();

      if (obj.contains("cards") && obj["cards"].isArray()) {
        QJsonArray cardArr = obj["cards"].toArray();

        for (QJsonValueRef cardRef : cardArr) {
          if (cardRef.isObject()) {
            QJsonObject card = cardRef.toObject();

            // Ensure well formated and insert into the database
            if (
                card.contains("name") && card["name"].isString() &&
                card.contains("id") && card["id"].isString() &&
                card.contains("type") && card["type"].isString() &&
                card.contains("rarity") && card["rarity"].isString() && card["rarity"].toString() != "Special" &&
                card.contains("cost") && card["cost"].isString() &&
                card.contains("desc") && card["desc"].isString()
                ) {

              Card* c = new Card(character,
                                 card["name"].toString(),
                  card["id"].toString(),
                  card["type"].toString(),
                  card["rarity"].toString(),
                  card["cost"].toString(),
                  card["desc"].toString());

              cards.insert(card["id"].toString(), c);
              ++created;
            }
            else {
              qDebug() << "ERROR: invalid card";
            }

          }
        }
      }

      qDebug() << "Found" << created << "cards for" << character;

      file.close();
    }
  }
  else {
    qDebug() << "ERROR:" << filename << "does not exist";
  }

}

QString stripUpgrade(QString input) {
  QStringList list = input.split("+");
  return list.first();
}

void CardDatabase::checkRun(QString filename) {
  QFile file(filename);
  qDebug() << "reading file" << filename;

  if (file.exists()) {
    if (file.open(QFile::ReadOnly)) {
      QByteArray data = file.readAll();
      QJsonDocument doc = QJsonDocument::fromJson(data);
      QJsonObject obj = doc.object();

      QJsonArray choices = obj["card_choices"].toArray();

      for (QJsonValueRef choiceRef : choices) {
        QJsonObject choice = choiceRef.toObject();
        QJsonArray notPicked = choice["not_picked"].toArray();
        QString picked = stripUpgrade(choice["picked"].toString());

        if (cards.contains(picked)) {
            Card* pickedCard = cards[picked];
            if (!cardStats.contains(picked))
                cardStats.insert(picked, new CardStatistics(pickedCard));
            cardStats[picked]->pick();
        }
        //map.insert(picked, true);

        //qDebug() << "PICKED:" << picked;
        for (QJsonValueRef skippedRef : notPicked) {
          QString skipped = stripUpgrade(skippedRef.toString());
          //qDebug() << "SKIPPED:" << skipped;
          if (cards.contains(skipped)) {
            Card* skippedCard = cards[skipped];
            if (!cardStats.contains(skipped))
              cardStats.insert(skipped, new CardStatistics(skippedCard));
            cardStats[skipped]->skip();
          }
          //map.insert(skipped, true);

          if (!cards.contains(skipped)) {
            qDebug() << "card" << skipped << "not found";
          }
        }
        //qDebug() << "--------";

        if (picked != "SKIP") {
          if (!cards.contains(picked)) {
            qDebug() << "card" << picked << "not found";
          }
        }

      }

      //        if (!(obj.contains("char") && obj["char"].isString())) {
      //          qDebug() << "ERROR: character name not found in json";
      //          return;
      //        }
    }
  }


  //    for (QString key : map.keys()) {
  //        qDebug() << "Found " << key;
  //    }
}




















