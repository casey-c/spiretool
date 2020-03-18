#ifndef DATA_H
#define DATA_H

#include <QString>

class Data
{
public:
    Data();

    QString getCharacterName() { return character; }
    int getPotionChance() { return potionChance; }

    void setCharacter(QString character) {this->character = character;}
    void setPotionChance(int chance) {this->potionChance = chance;}
private:
    QString character;
    int potionChance;
};

#endif // DATA_H
