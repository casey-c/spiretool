#include "src/h/card.h"
#include <QDebug>

Card::Card(QString character, QString name, QString id, QString type, QString rarity, QString cost, QString desc) :
    character(character),
    name(name),
    id(id),
    type(type),
    rarity(rarity),
    cost(cost),
    desc(desc)
{

}


void Card::print() {
    qDebug() << "Class:" << character
             << "| Card:" << name
             << "| Type:" << type
             << "| Cost:" << cost
             << "| Rarity:" << rarity;
}

Card::operator QString() {
    return QString("Card[%1, %2]").arg(character).arg(name);
}
