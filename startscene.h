#ifndef STARTSCENE_H
#define STARTSCENE_H

#include <QSoundEffect>
#include "config.h"
#include <QWidget>
#include <QPushButton>
class StartScene : public QWidget
{
    Q_OBJECT
public:
    explicit StartScene(QWidget *parent = nullptr);
    QPixmap m_startscene;
    QSoundEffect *startsound;

    void  initScene();
    QPushButton* play_game_button;
    ~StartScene();
signals:

};

#endif // STARTSCENE_H
