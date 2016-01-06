QT += qml quick
TARGET = EOSAIUserInterface
!no_desktop: QT += widgets

#include(src.pri)
#include(src/src.pri)
#include(../shared/shared.pri)
# C:\Programming\boost_1_59_0

#INCLUDEPATH += "C:\Programming\boost_1_59_0\boost"
#INCLUDEPATH += "C:/Programming/boost_1_59_0/"
INCLUDEPATH += C:/Programming/boost_1_59_0/

OTHER_FILES += \
    main.qml \
    ScrollBar.qml

RESOURCES += \
    resources.qrc

SOURCES += \
    main.cpp \
    InterprocLoop.cpp \
    GameMapImage.cpp \
    GameData.cpp \
    PoiListModel.cpp \
    SharedMemory.cpp \
    StatusListModel.cpp

HEADERS += \
    InterprocLoop.h \
    GameMapImage.h \
    GameData.h \
    StatusLine.h \
    PoiListModel.h \
    ../EOSAI/SharedMemory.h \
    StatusListModel.h

