QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    carddisplay.cpp \
    custombutton.cpp \
    eventdisplay.cpp \
    eventoverview.cpp \
    mainwindow2.cpp \
    mainwindow3.cpp \
    potiondisplay.cpp \
    src/cpp/resourcemanager.cpp \
    src/cpp/run.cpp \
    src/cpp/cardstatistics.cpp \
    src/cpp/about.cpp \
    src/cpp/card.cpp \
    src/cpp/carddatabase.cpp \
    src/cpp/config.cpp \
    src/cpp/main.cpp \
    src/cpp/mainwindow.cpp \
    src/cpp/options.cpp \
    src/cpp/tutorial.cpp \
    src/cpp/streaks.cpp \
    src/cpp/utils.cpp \
    src/cpp/statisticswindow.cpp \
    src/cpp/referencewindow.cpp

HEADERS += \
    carddisplay.h \
    custombutton.h \
    eventdisplay.h \
    eventoverview.h \
    mainwindow2.h \
    mainwindow3.h \
    potiondisplay.h \
    src/h/resourcemanager.h \
    src/h/run.h \
    src/h/cardstatistics.h \
    src/h/about.h \
    src/h/card.h \
    src/h/carddatabase.h \
    src/h/config.h \
    src/h/mainwindow.h \
    src/h/options.h \
    src/h/tutorial.h \
    src/h/streaks.h \
    src/h/utils.h \
    src/h/statisticswindow.h \
    src/h/referencewindow.h

FORMS += \
    carddisplay.ui \
    custombutton.ui \
    eventdisplay.ui \
    eventoverview.ui \
    mainwindow2.ui \
    mainwindow3.ui \
    potiondisplay.ui \
    src/ui/about.ui \
    src/ui/mainwindow.ui \
    src/ui/options.ui \
    src/ui/tutorial.ui \
    src/ui/statisticswindow.ui \
    src/ui/referencewindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources/resources.qrc

RC_ICONS = resources/icon.ico
