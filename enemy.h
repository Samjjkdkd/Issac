#ifndef ENEMYPLANE_H
#define ENEMYPLANE_H
#include <QPixmap>
#include "config.h"
#include "qpainter.h"
class EnemyPlane
{
public:
    EnemyPlane();

    //更新坐标
    void updatePosition(int hero_X,int hero_Y);

    //位置
    int m_X;
    int m_Y;

    //敌机的矩形边框（碰撞检测）
    QRect m_Rect;


    //状态
    bool m_Free;

    //速度
    double m_Speed;
    //从哪里进入地图
    int startplace;
    //生命值
    int hp;
    //初始生命
    int preset_hp;

    //敌人种类
    int type;
    //画敌人
    QPixmap m_enemy;
    virtual void drawEnemy(QPainter &t){
        t.drawPixmap(this->m_X, this->m_Y, this->m_enemy);
    }
    virtual void updateInfo(){}
};

#endif // ENEMYPLANE_H
