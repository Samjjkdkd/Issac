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
    //e技能
    void skill(bool s);
    //q技能
    void burst(bool s);
    //z技能
    void ashwab(bool s);
    //冲刺
    void sprint(bool s);
    //设置飞机位置
    void setPosition(int x, int y);

    //充能
    void i_got_charge(int n);
    void i_got_charge2(int n);

    QPixmap PixmapToRound(const QPixmap &src, int radius);

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

    //飞机速度
    float m_speed;

    //飞机体力
    int m_stamina;

    //飞机充能
    int m_charge;

    int m_charge2;

    //飞机的矩形边框
    QRect m_Rect;

    //弹匣
    Bullet m_bullets[BULLET_NUM];

    //英雄朝向
    int b_direction;

    //发射间隔记录
    int m_recorder;

    //冲刺冷却计时器
    int m_sprint_recorder;

    //冲刺持续计时器
    int m_sprint_timer;

    //爆发持续计时器
    int m_burst_timer;

    //终结技持续计时器
    int m_ashwab_timer;

    //e计时器
    int m_skill_recorder;

    //q计时器
    int m_burst_recorder;

    //z计时器
    int m_ashwab_recorder;

    int shiftx, shifty;

    float m_skill_degree;

    int m_bullet_interval;
};

#endif // HERO_H
