#include "src/h/run.h"
#include "src/h/utils.h"

#include <QFile>
#include <QDebug>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValueRef>

#include "src/h/resourcemanager.h"

Run* Run::build(QString filename) {
    Run* run = new Run(filename);

    if (run->refresh()) {
      return run;
    }
    else {
      qDebug() << "ERROR: failed to init" << filename;
      delete run;
      return nullptr;
    }
}

Run::Run(QString filename) :
  filename(filename),
  date(0),
  char_name("Unknown Character"),
  post_combat(false),
  hasSozu(false),
  hasWBS(false),
  hasQC(false),
  hasBC(false),
  hasPW(false),
  hasNL(false)
{

}

// Updates this object with the file details; returns true if an update is made
bool Run::refresh() {
  QFile file(filename);

  bool anythingChanged = false;

  if (file.exists()) {
    if (file.open(QFile::ReadOnly)) {
      QByteArray data = file.readAll();
      QJsonDocument doc = QJsonDocument::fromJson(data);
      QJsonObject obj = doc.object();

      // Load useful information from the JSON (with backup default values)
      long new_date = Utils::docGetLongOr(obj, "save_date", -1);

      // TODO: do better than this. maybe look at relics/cards?
      // lazily look at file path to see if it is something like
      // runs/THE_SILENT/15848484294.run
      QString new_name = "Unknown Character";
      if (filename.contains("IRONCLAD"))
          new_name = "Ironclad";
      if (filename.contains("DEFECT"))
          new_name = "Defect";
      if (filename.contains("THE_SILENT"))
          new_name = "Silent";
      if (filename.contains("WATCHER"))
          new_name = "Watcher";
      // don't think this will work (it's not saved in the savefile anywhere)
      // (it definitely doesn't work on the autosaveBETA file)
      //QString new_name = Utils::docGetQStringOr(obj, "char", "Unknown Character");

      bool new_is_asc = Utils::docGetBoolOr(obj, "is_ascension_mode", false);
      bool new_post_combat = Utils::docGetBoolOr(obj, "post_combat", false);

      int new_asc_level = Utils::docGetIntOr(obj, "ascension_level", -1);
      int new_floor_reached = Utils::docGetIntOr(obj, "floor_num", 0);
      int new_potion_chance = Utils::docGetIntOr(obj, "potion_chance", 0);
      int new_cbr = Utils::docGetIntOr(obj, "card_random_seed_randomizer", 5);


      // Check if anything has changed & update it
      if (date != new_date) { date = new_date; anythingChanged = true; }
      if (char_name != new_name) { char_name = new_name; anythingChanged = true; }
      if (new_is_asc != is_asc) { is_asc = new_is_asc; anythingChanged = true; }
      if (new_post_combat != post_combat) { post_combat = new_post_combat; anythingChanged = true; }

      if (new_asc_level != asc_level) { asc_level = new_asc_level; anythingChanged = true; }
      if (new_floor_reached != floor_reached) { floor_reached = new_floor_reached; anythingChanged = true; }
      if (new_potion_chance != potion_chance) { potion_chance = new_potion_chance; anythingChanged = true; }
      if (new_cbr != cbr) { cbr = new_cbr; anythingChanged = true; }

      // Relics list (saves unlikely to only update this)
      if (obj.contains("relics")) {
          current_relics.clear();
          QJsonArray relics = obj["relics"].toArray();
          QJsonArray::iterator itr = relics.begin();

          for (; itr != relics.end(); ++itr) {
              QString r = itr->toString();
              current_relics.push_back(r);
              if (r == "White Beast Statue") hasWBS = true;
              else if (r == "Sozu") hasSozu = true;
              else if (r == "Question Card") hasQC = true;
              else if (r == "Busted Crown") hasBC = true;
              else if (r == "Prayer Wheel") hasPW = true;
              else if (r == "Nloth's Gift") hasNL = true;
          }
      }

//      qDebug() << "run: changes" << anythingChanged;
    }
  }
  else {
      qDebug() << "ERROR: run file not exist" << filename;
  }

  return anythingChanged;
}

// debug
void Run::print() {
    qDebug() << "RUN: [char:"
             << char_name << ", date:"
             << date << ", floor_reached:"
             << floor_reached << ", asc_level:"
             << asc_level << ", potion_chance:"
             << potion_chance << ", cbr:"
             << cbr
             << "]";
}


QString Run::getCharNameAndFloor() {
    QString formatted = char_name;
    formatted.append(QString(" (floor %1)").arg(floor_reached));
    return formatted;
}

QPixmap Run::getPixmap() {
    if (char_name == "Ironclad")
      return ResourceManager::getInstance().getIroncladPixmap();
    else if (char_name == "Silent")
      return ResourceManager::getInstance().getSilentPixmap();
    else if (char_name == "Defect")
      return ResourceManager::getInstance().getDefectPixmap();
    else if (char_name == "Watcher")
      return ResourceManager::getInstance().getWatcherPixmap();
    else
      return ResourceManager::getInstance().getUnknownPixmap();
}































