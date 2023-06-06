#include "hero.h"

HeroPlane::HeroPlane()
{
    //初始化加载飞机图片资源
    m_Plane_original.load(HERO_PATH);
    QTransform transform;
    transform.rotate(90);
    m_Plane_original = m_Plane_original.transformed(transform, Qt::SmoothTransformation);
    m_Plane_original = m_Plane_original.scaled(RESIZE_WIDTH_1, RESIZE_HEIGHT_1, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    //
    m_Plane = m_Plane_original;

    //初始化坐标
    m_X = GAME_WIDTH * 0.5 - m_Plane.width()*0.5;
    m_Y = GAME_HEIGHT*0.5 - m_Plane.height()*0.5;

    //初始化矩形框
    m_Rect.setWidth(m_Plane.width());
    m_Rect.setHeight(m_Plane.height());
    m_Rect.moveTo(m_X,m_Y);

    m_hp = MAX_HEALTH;

    //初始化发射间隔记录
    m_recorder = 0;

}

void HeroPlane::setPosition(int x, int y)
{
    if(x<0||y<0||x+m_Rect.width()>GAME_WIDTH||y+m_Rect.height()>GAME_HEIGHT)    return;
    m_X = x;
    m_Y = y;
    m_Rect.moveTo(m_X,m_Y);
}

void HeroPlane::shoot()
{
    QTransform transform;
    //累加时间间隔记录变量
    m_recorder++;
    //判断如果记录数字 未达到发射间隔，直接return
    if(m_recorder < BULLET_INTERVAL)
    {
        return;
    }
    //到达发射时间处理
    //重置发射时间间隔记录
    m_recorder = 0;

    //发射子弹
    for(int i = 0 ; i < BULLET_NUM;i++)
    {
        //如果是空闲的子弹进行发射
        if(m_bullets[i].m_Free)
        {
            //初始化子弹角度
            m_bullets[i].m_direction = b_direction;
            transform.rotate(-b_direction+180);
            m_bullets[i].m_Bullet = m_bullets[i].m_Bullet_original.transformed(transform,Qt::SmoothTransformation);

            //将该子弹空闲状态改为假
            m_bullets[i].m_Free = false;
            //设置发射的子弹坐标
            m_bullets[i].m_rX = (double)m_X + (double)m_Rect.width()*0.5 - 10.0;
            m_bullets[i].m_rY = (double)m_Y + 20.0 ;
            m_bullets[i].xyTrans();

            break;
        }
    }
}
