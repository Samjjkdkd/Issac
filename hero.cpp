#include "hero.h"
#include <QPainter>
#include <QPainterPath>
#include <QRect>
HeroPlane::HeroPlane()
{

    //初始化加载飞机图片资源
    m_Plane_original.load(HERO_PATH);
    //m_Plane_original = PixmapToRound(m_Plane_original, RESIZE_RADIUS);

    m_Plane_original = m_Plane_original.scaled(RESIZE_RADIUS, RESIZE_RADIUS, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    QTransform transform;
    transform.translate(RESIZE_RADIUS/2,RESIZE_RADIUS/2);  //先将矩阵移到图片中心
    transform.rotate(90);        //旋转矩阵
    transform.translate(-RESIZE_RADIUS/2, -RESIZE_RADIUS/2); //将矩阵移回
    m_Plane_original = m_Plane_original.transformed(transform, Qt::SmoothTransformation);

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
    m_charge = 0;
    m_speed = I_SHOW_SPEED;

    m_stamina = MAX_STAMINA;

    //初始化发射间隔记录
    m_recorder = 0;

    m_skill_recorder = SKILL_INTERVAL;
    m_burst_recorder = BURST_INTERVAL;
    m_sprint_recorder = SPRINT_INTERVAL;

    m_sprint_timer = 0;
    m_burst_timer = 0;

    for(int i = 0 ;i<BULLET_NUM;++i){
        m_bullets[i] = Bullet(i);
    }
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
    for(int i = 0 ; i < BULLET_NUM-SKILL_BULLET_NUM;i++)
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
            m_bullets[i].m_Rect.moveTo(m_bullets[i].m_X, m_bullets[i].m_Y);

            break;
        }
    }
}

void HeroPlane::i_got_charge(int n){
    if(this->m_charge<CHARGE_MAX)   this->m_charge+=n;
    if(this->m_charge>CHARGE_MAX)   this->m_charge=CHARGE_MAX;
}


void HeroPlane::skill(bool s)
{

    //累加时间间隔记录变量
    m_skill_recorder++;
    //判断如果记录数字 未达到发射间隔，直接return
    if(m_skill_recorder < SKILL_INTERVAL||!s)
    {
        return;
    }
    //到达发射时间处理
    //重置发射时间间隔记录
    m_skill_recorder = 0;

    //发射子弹
    float d_degree = (float)(360.0/(float)SKILL_BULLET_NUM);
    QTransform transform;
    transform.rotate(180.0);
    for(float j = 0; j <= 360.0;j+=d_degree){

        for(int i = BULLET_NUM-SKILL_BULLET_NUM ; i < BULLET_NUM;i++)
        {
            //如果是空闲的子弹进行发射
            if(m_bullets[i].m_Free)
            {
                //初始化子弹角度
                m_bullets[i].m_direction = j;

                m_bullets[i].m_Bullet = m_bullets[i].m_Bullet_original.transformed(transform,Qt::SmoothTransformation);
                transform.rotate(-d_degree);

                //将该子弹空闲状态改为假
                m_bullets[i].m_Free = false;
                //设置发射的子弹坐标
                m_bullets[i].m_rX = (double)m_X + (double)m_Rect.width()*0.5 - 10.0;
                m_bullets[i].m_rY = (double)m_Y + 20.0 ;
                m_bullets[i].xyTrans();
                m_bullets[i].m_Rect.moveTo(m_bullets[i].m_X, m_bullets[i].m_Y);

                break;
            }
        }
    }
}
void HeroPlane::burst(bool s)
{

    //累加时间间隔记录变量
    m_burst_recorder++;
    //判断如果记录数字 未达到发射间隔，直接return

    if(m_burst_timer)   {
        m_burst_timer--;
    }

    if(m_burst_recorder < SKILL_INTERVAL||!s||m_charge!=CHARGE_MAX)
    {
        return;
    }
    //到达发射时间处理
    //重置发射时间间隔记录
    m_burst_recorder = 0;
    m_charge = 0;

    //大招效果
    m_burst_timer = BURST_TIME;
}


void HeroPlane::sprint(bool s)
{

    //累加时间间隔记录变量
    m_sprint_recorder++;

    m_stamina++;
    m_stamina=m_stamina>=MAX_STAMINA?MAX_STAMINA:m_stamina;

    if(m_sprint_timer){
        m_sprint_timer--;
        if(SPRINT_TIME+BOOST_TIME<m_sprint_timer){
            m_speed = I_SHOW_SPEED + I_GOT_SPRINT*((float)(SPRINT_TIME+2*BOOST_TIME-m_sprint_timer)/(float)BOOST_TIME);
        }else if(BOOST_TIME<m_sprint_timer){
            m_speed = I_SHOW_SPEED + I_GOT_SPRINT;
        }else{
            m_speed = I_SHOW_SPEED + I_GOT_SPRINT*((float)(m_sprint_timer)/(float)BOOST_TIME);
        }
    }
    //判断如果记录数字 未达到发射间隔，直接return
    if(m_sprint_recorder < SPRINT_INTERVAL||!s||m_stamina<SPRINT_COST)
    {
        return;
    }
    m_sprint_timer = SPRINT_TIME+2*BOOST_TIME;
    m_stamina-=SPRINT_COST;
    m_sprint_recorder = 0;

}

QPixmap HeroPlane::PixmapToRound(const QPixmap &src, int radius)
{
    if (src.isNull())
    {
        return QPixmap();
    }

    //按比例缩放
    QPixmap pixmapa;
    if(src.width() != radius || src.height() != radius)
    {
        pixmapa = src.scaled(radius, radius, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    }
    else
    {
        pixmapa = src;
    }

    QPixmap pixmap(radius,radius);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform |  QPainter::Antialiasing);

    QPainterPath path;
    path.addEllipse(0, 0, radius, radius);
    painter.setClipPath(path);
    painter.drawPixmap(0, 0, radius, radius, pixmapa);

    return pixmap;
}

