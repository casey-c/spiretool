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
