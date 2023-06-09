#include "audiothread.h"

#include <QUrl>

AudioThread::AudioThread(QObject *parent,int type) :
    QObject(parent)
{
    effect.moveToThread(&thread);
    connect(this, SIGNAL(doPlay()), &effect, SLOT(play()));
    thread.start();
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
    case SOUND_Z:
    {
        fileName = Z_SOUND_PATH;
    }
    break;

    default:
    {
        return;
    }
    }

    effect.setSource(QUrl::fromLocalFile(fileName));
    effect.setLoopCount(1);
    effect.setVolume(1.0f);

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
    case SOUND_Z:
    {
        fileName = Z_SOUND_PATH;
    }
    break;

    default:
    {
        return;
    }
    }

    effect.setSource(QUrl::fromLocalFile(fileName));
    effect.setLoopCount(1);
    effect.setVolume(1.0f);

    emit this->doPlay();
}
