#ifndef HERO_H
#define HERO_H
#include<QRect>
#include<QPixmap>
#include "bullet.h"

class HeroPlane
{
public:
    HeroPlane();

    //发射子弹
    void shoot();
    //设置飞机位置
    void setPosition(int x, int y);

public:
    //飞机资源 对象
    QPixmap m_Plane;
    QPixmap m_Plane_original;
    QPixmap m_Plane_right;
    QPixmap m_Plane_down;
    QPixmap m_Plane_left;
    QPixmap m_Plane_up;

    //飞机坐标
    int m_X;
    int m_Y;

    //飞机生命
    int m_hp;

    //飞机的矩形边框
    QRect m_Rect;

    //弹匣
    Bullet m_bullets[BULLET_NUM];

    //子弹方向
    int b_direction = 180;

    //发射间隔记录
    int m_recorder;
};

#endif // HERO_H
