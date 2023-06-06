#include "bullet.h"
#include <math.h>
Bullet::Bullet()
{
    //加载子弹资源
    m_Bullet_original.load(BULLET_PATH);

    m_Bullet = m_Bullet_original;

    //子弹坐标 初始坐标可随意设置，后期会重置
    m_X = GAME_WIDTH*0.5 - m_Bullet.width()*0.5;
    m_Y = GAME_HEIGHT;

    //子弹状态
    m_Free = true;

    //子弹速度
    m_Speed = BULLET_SPEED;

    //子弹矩形框
    m_Rect.setWidth(m_Bullet.width());
    m_Rect.setHeight(m_Bullet.height());
    m_Rect.moveTo(m_X,m_Y);
}
void Bullet::updatePosition()
{
    //如果子弹是空闲状态，不需要坐标计算
    //玩家飞机可以控制子弹的空闲状态为false
    if(m_Free)
    {
        return;
    }

    //子弹瞎jb移动
    m_rX+=(double)m_Speed*sin(Pi/180.0*(double)(m_direction));
    m_rY+=(double)m_Speed*cos(Pi/180.0*(double)(m_direction));
    this->xyTrans();
    m_Rect.moveTo(m_X,m_Y);

    if(m_X>=GAME_WIDTH + m_Rect.width()||m_X<=-m_Rect.width()
        ||m_Y>=GAME_HEIGHT + m_Rect.height()||m_Y<=-m_Rect.height()) m_Free = true;
}
