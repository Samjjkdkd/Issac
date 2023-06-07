#include "audiothread.h"

#include <QUrl>

AudioThread::AudioThread(QObject *parent) :
    QObject(parent)
{
    effect.moveToThread(&thread);
    connect(this, SIGNAL(doPlay()), &effect, SLOT(play()));
    thread.start();
}

AudioThread::~AudioThread()
{
    if(thread.isRunning())
    {
        thread.terminate();
    }
}

void AudioThread::play()
{

    if(!effect.isPlaying())
    {
        effect.setSource(QUrl::fromLocalFile(SOUND_BGM_PATH));
        effect.setLoopCount(1);

        emit this->doPlay();
    }
}

void AudioThread::play(int type)
{
    if(effect.isPlaying())
    {
        return;
    }

    voice_type = type;
    QString fileName;
    switch (voice_type)
    {
    case SOUND_BGM:
    {
        fileName = SOUND_BGM_PATH;
    }
    break;
    case SOUND_BOOM:
    {
        fileName = SOUND_BOOM_PATH;
    }
    break;

    default:
    {
        return;
    }
    }

    effect.setSource(QUrl::fromLocalFile("./voice/"+fileName));
    effect.setLoopCount(1);
    effect.setVolume(1.0f);

    emit this->doPlay();
}
