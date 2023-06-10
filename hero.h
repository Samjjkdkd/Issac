#ifndef HERO_H
#define HERO_H
#include<QRect>
#include<QPixmap>
#include "bullet.h"
#include "config.h"
#include "auxstructure.h"

enum HeroInfo{
    Speed,
    Skill_Interval,
    Sprint_Interval,
    Ashwab_Interval,
    Burst_Interval,
    Health,
    Stamina
};

enum SpeedBonusIndex{
    s_Keep,
    s_Sprint,
    s_Burst
};


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

    //获取速度
    qreal getInfo(int type);

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
    TubeLikeData m_hp;

    //飞机速度
    VariableData m_speed;

    //飞机体力
    TubeLikeData m_stamina;

    //飞机充能
    TubeLikeData m_charge;

    TubeLikeData m_charge2;

    //飞机的矩形边框
    QRect m_Rect;

    //弹匣
    Bullet m_bullets[BULLET_MAX];

    //英雄朝向
    int b_direction;

    //行为相关
    //recorder为正计时，与间隔相关
    //timer为倒计时，与持续时间相关
    //interval为间隔

    //发射间隔记录
    int m_bullet_recorder;
    int m_bullet_interval;

    //元素战技
    int m_skill_recorder;
    int m_skill_interval;

    //冲刺
    int m_sprint_recorder;
    int m_sprint_timer;
    int m_sprint_interval;

    //元素爆发
    int m_burst_timer;
    int m_burst_recorder;
    int m_burst_interval;

    //终结技
    int m_ashwab_timer;
    int m_ashwab_recorder;
    int m_ashwab_interval;

    float m_skill_degree;

    //q计时器

    //z计时器

    int shiftx, shifty;


};

#endif // HERO_H
