#ifndef ENEMYPLANE_H
#define ENEMYPLANE_H
#include <QPixmap>

class EnemyPlane
{
public:
    EnemyPlane();

    //更新坐标
    void updatePosition(int hero_X,int hero_Y);
public:
    //敌机资源对象
    QPixmap m_enemy;

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
};

#endif // ENEMYPLANE_H
