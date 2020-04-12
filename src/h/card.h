#ifndef CARD_H
#define CARD_H

#include <QString>

class Card
{
public:
  Card(QString character, QString name,
       QString id, QString type,
       QString rarity, QString cost,
       QString desc);

  operator QString();

  void print();

  bool isStarter() {return rarity == "Starter"; }

  QString getCardName() { return name; }
  QString getClassName() { return character; }

private:
  QString character, name, id, type, rarity, cost, desc;

};

#endif // CARD_H
