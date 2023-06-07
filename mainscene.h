#ifndef MAINSCENE_H
#define MAINSCENE_H
#include<QTimer>
#include <QWidget>
#include <QSound>
#include <QSoundEffect>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include "map.h"
#include "hero.h"
#include "enemy.h"
#include "bomb.h"
#include "movevector.h"
#include "bloodtrail.h"
#include <vector>

class MainScene : public QWidget
{
    Q_OBJECT

public:
    MainScene(QWidget *parent = nullptr);

    QSoundEffect *bgsound;
    QSoundEffect *bombSound;

    //启动游戏  用于启动定时器对象
    void playGame();
    //更新坐标
    void updatePosition();
    //绘图事件
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);//按键事件
    void keyReleaseEvent(QKeyEvent *event);//松键事件

    //地图对象
    Map m_map;

    QTimer m_Timer;

    HeroPlane m_hero;

    MoveVector my_vector;//自机移动向量

    //敌机出场
    void enemyToScene();

    //敌机数组
    std::vector <EnemyPlane*> m_enemys;

    //敌机出场间隔记录
    int m_recorder;

    //得分
    int score = 0;

    void collisionDetection();

    //爆炸数组
    Bomb m_bombs[BOMB_NUM];

    //残留血液
    bloodtrail m_bloodtrail[BLOOD_NUM];
    QPixmap m_blood[2];

    // Bullet temp_bullet;
    ~MainScene();
    void  initScene();

};

#endif // MAINSCENE_H
