#ifndef MAINSCENE_H
#define MAINSCENE_H
#include <QTimer>
#include <QWidget>
#include <QSound>
#include <QSoundEffect>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include "map.h"
#include "hero.h"
#include "enemy.h"
#include "bomb.h"
#include "energy.h"
#include "movevector.h"
#include "bloodtrail.h"
#include "audiothread.h"
#include <vector>
#include "config.h"
#include "enemy_1.h"
#include "enemy_2.h"
#include "enemy_3.h"
#include <QIcon>
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>
#include <QPen>
#include <ctime>
#include <QSoundEffect>
#include <QSound>
#include <QLabel>
#include <QColor>
#include <math.h>
#include <complex>
#include <sys/timeb.h>

enum INPUT_TYPE{
    WASD,
    AD_DIR
};

class MainScene : public QWidget
{
    Q_OBJECT

public:
    MainScene(QWidget *parent = nullptr);
    void set_input_type(int t);

    class InfoIcon;

    InfoIcon *icon_skill;
    InfoIcon *icon_burst;
    InfoIcon *icon_ashwab;

    class AnimatePlayer;

    AnimatePlayer *ashwab_player;

    QSoundEffect *bgsound;
    QSoundEffect *bombSound;
    QSoundEffect *z_sound;

    int input_type;

    //启动游戏  用于启动定时器对象
    void playGame();
    //更新坐标
    void updatePosition();

    void killAll();

    //绘图事件
    void paintEvent(QPaintEvent *event);
    void paintHostileObject(QPainter &painter);
    void paintFriendlyObject(QPainter &painter);
    void paintInfoComponent(QPainter &painter);
    void paintDebug(QPainter &painter);
    void paintMask(QPainter &painter, float trans);


    void mouseMoveEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);//按键事件
    void keyReleaseEvent(QKeyEvent *event);//松键事件

    //用于寄算fps
    int lastTime;
    int currTime;
    timeb cTime;

    //地图对象
    Map m_map;

    QTimer m_Timer;

    HeroPlane m_hero;

    MoveVector my_vector;//自机移动向量

    //敌机出场
    void enemyToScene();

    //敌机数组
    std::vector <EnemyPlane*> m_enemys;

    //敌机生成事件
    EventManager m_enemySpawn;

    //敌机数量
    VariableData m_enemy_num;

    //得分
    int score = 0;

    void collisionDetection();

    //爆炸数组
    Bomb m_bombs[BOMB_NUM];

    //能量数组
    Energy m_energies[ENERGY_MAX];

    //残留血液
    bloodtrail m_bloodtrail[BLOOD_NUM];
    QPixmap m_blood[2];

    // Bullet temp_bullet;
    ~MainScene();
    void  initScene();

};

class MainScene::InfoIcon{
public:
    EventManager &e;
    TubeLikeData &d;
    InfoIcon(EventManager &_e,TubeLikeData &_charge,int _x, int _y, int _size, unsigned _c,QString k);
    int margin_x, margin_y;
    int size;
    QFont font1;
    QFont font2;
    float opacity1 = 0.6f;
    float opacity2 = 0.4f;
    QColor c;
    QPainterPath path;
    QString str_;
    QString key_;

    void paint(QPainter &p);
};

class MainScene::AnimatePlayer{
public:
    timeb timeObject;
    QPixmap frames[100];
    int frame_num;
    int frame_rate;
    float frame_time;
    QString path_preffix;
    bool playing = false;
    bool ended = false;
    float start_time;
    float end_time;
    float playing_time;
    float animate_time;
    long long play_time;
    long long curr_time;
    bool isPlaying();
    AnimatePlayer(int _num, int _rate, QString pre, float _start, float _end = 100.0f);
    void start();
    void play(QPainter &painter);

};

#endif // MAINSCENE_H
