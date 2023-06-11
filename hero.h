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

enum BulletBonusIndex{
    b_Keep,
    b_Burst
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

    //开了
    void cheat();

    //设置飞机位置
    void setPosition(int x, int y);

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
    VariableData sprint_cost;

    //飞机充能
    TubeLikeData m_charge3;//战技充能

    TubeLikeData m_charge;//爆发充能

    TubeLikeData m_charge2;//终结充能

    //飞机的矩形边框
    QRect m_Rect;

    //弹匣
    Bullet m_bullets[BULLET_MAX];
    Bullet m_bullets2[BULLET_MAX];

    //英雄朝向
    int b_direction;

    //行为相关
    //recorder为正计时，与间隔相关
    //timer为倒计时，与持续时间相关
    //interval为间隔

    //发射事件
    EventManager m_shoot;

    //冲刺事件
    EventManager m_sprint;

    //战技事件
    EventManager m_skill;

    //爆发事件
    EventManager m_burst;

    //终结事件
    EventManager m_ashwab;


    //弹匣数量
    VariableData m_bullet_num;

    //战技子弹扇形角度
    VariableData m_skill_degree;
    //战技子弹数量
    VariableData m_skill_bullet_num;

    //q计时器

    //z计时器

    int shiftx, shifty;


};

#endif // HERO_H
