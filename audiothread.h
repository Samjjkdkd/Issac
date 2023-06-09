#ifndef AUDIOTHREAD_H
#define AUDIOTHREAD_H

#include "config.h"

#include <QtCore>
#include <QObject>

#include <QSoundEffect>
#include <QThread>

enum SOUND_TYPE{
    SOUND_BGM,
    SOUND_BOOM,
    SOUND_Z
};

class AudioThread : public QObject
{
    Q_OBJECT
public:
    AudioThread(QObject *parent = nullptr,int type= SOUND_Z);
    ~AudioThread();

signals:
    void doPlay();

public slots:
    void play();
    void play(int type);

private:
    int voice_type;

    QSoundEffect effect;

    QThread thread;

};

#endif // AUDIOTHREAD_H
