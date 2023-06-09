QT       += core gui
QT       += multimedia
CONFIG += resources_big
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    audiothread.cpp \
    auxstructure.cpp \
    bloodtrail.cpp \
    bomb.cpp \
    bullet.cpp \
    enemy.cpp \
    enemy_1.cpp \
    enemy_2.cpp \
    enemy_3.cpp \
    enemy_4.cpp \
    energy.cpp \
    hero.cpp \
    main.cpp \
    mainscene.cpp \
    map.cpp \
    movevector.cpp \
    startscene.cpp \
    virtualbutton.cpp

HEADERS += \
    audiothread.h \
    auxstructure.h \
    bloodtrail.h \
    bomb.h \
    bullet.h \
    config.h \
    enemy.h \
    enemy_1.h \
    enemy_2.h \
    enemy_3.h \
    enemy_4.h \
    energy.h \
    hero.h \
    mainscene.h \
    map.h \
    movevector.h \
    startscene.h \
    startscene_ui.h \
    virtualbutton.h
    QSoundEffect

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
RC_ICONS = res\app.ico

QT  += core gui multimedia

FORMS +=
