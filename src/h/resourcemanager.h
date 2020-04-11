#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <QPixmap>

class ResourceManager
{
public:
    static ResourceManager& getInstance() {
        static ResourceManager instance;
        return instance;
    }

    QPixmap getUnknownPixmap() { return pixUnknown; }
    QPixmap getIroncladPixmap() { return pixIronclad; }
    QPixmap getSilentPixmap() { return pixSilent; }
    QPixmap getDefectPixmap() { return pixDefect; }
    QPixmap getWatcherPixmap() { return pixWatcher; }

    QPixmap getIconOptions() { return iconOptions; }
    QPixmap getIconReference() { return iconReference; }
    QPixmap getIconStats() { return iconStats; }
    QPixmap getIconHelp() { return iconHelp; }

    ResourceManager(ResourceManager const&) = delete;
    void operator=(ResourceManager const&) = delete;

private:
    ResourceManager();

    QPixmap pixUnknown, pixIronclad, pixSilent, pixDefect, pixWatcher;
    QPixmap iconOptions, iconReference, iconStats, iconHelp;
};

#endif // RESOURCEMANAGER_H
