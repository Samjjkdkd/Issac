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
    toInitPosotion();
    //初始化矩形框
    m_Rect.setWidth(m_Plane.width());
    m_Rect.setHeight(m_Plane.height());
    m_Rect.moveTo(m_X,m_Y);


    m_hp = TubeLikeData(MAX_HEALTH);

    m_charge = TubeLikeData(0.0f,CHARGE_MAX);
    m_charge2 = TubeLikeData(0.0f,CHARGE2_MAX);
    m_charge3 = TubeLikeData(SKILL_INTERVAL);

    m_speed = I_SHOW_SPEED;

    m_stamina = TubeLikeData(MAX_STAMINA);

    //事件初始化
    m_shoot = EventManager(BULLET_INTERVAL);
    m_sprint = EventManager(SPRINT_INTERVAL, SPRINT_TIME);
    m_skill = EventManager(SKILL_INTERVAL);
    m_burst = EventManager(BURST_INTERVAL, BURST_TIME);
    m_ashwab = EventManager(ASHWAB_INTERVAL,ASHWAB_TIME);
    sprint_cost = SPRINT_COST;


    m_bullet_num = BULLET_NUM;
    m_bullet_num.setMax(BULLET_MAX);


    for(int i = 0 ;i<m_bullet_num.max;++i){
        m_bullets[i] = Bullet(0);
        m_bullets2[i] = Bullet(1);
    }

    m_skill_degree = SKILL_DEGREE;
    m_skill_bullet_num = SKILL_BULLET_NUM;

}
\
void HeroPlane::toInitPosotion(){
    b_direction = 180;
    m_X = GAME_WIDTH * 0.5 - m_Plane.width()*0.5;
    m_Y = GAME_HEIGHT*0.5 - m_Plane.height()*0.5;

}

void HeroPlane::cheat(){
    m_charge.fill();
    m_charge2.fill();
    m_charge3.fill();
    m_skill.ready();
    m_burst.ready();
    m_ashwab.ready();
    m_sprint.ready();
    m_stamina.fill();
}

qreal HeroPlane::getInfo(int type){
    qreal return_val;
    switch(type){
    case Speed:
        return_val = this->m_speed();
        break;
    default:
        return_val = 0;
    }
    return return_val;
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

    m_shoot.tick(m_ashwab.holding());
    if(!m_shoot.avail())
    {
        return;
    }

    //释放
    m_shoot.release();

    //发射子弹
    for(int i = 0 ; i < m_bullet_num();i++)
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


void HeroPlane::skill(bool s)
{

    m_charge3++;
    //累加时间间隔记录变量
    m_skill.tick(m_ashwab.holding());
    //判断如果记录数字 未达到发射间隔，直接return
    if(!(m_skill.avail()&&s&&!m_ashwab.holding()))
    {
        return;
    }
    //到达发射时间处理
    //重置发射时间间隔记录
    m_skill.release();
    m_charge3 = 0;
    //发射子弹
    float d_degree = (m_skill_degree()/m_skill_bullet_num());
    QTransform transform;
    transform.rotate(180.0);
    for(float j = b_direction - m_skill_degree()/2; j <= b_direction + m_skill_degree()/2;j+=d_degree){
        for(int i = 0 ; i < m_bullet_num.max;i++)
        {
            //如果是空闲的子弹进行发射
            if(m_bullets2[i].m_Free)
            {
                //初始化子弹角度
                m_bullets2[i].m_direction = j;

                m_bullets2[i].m_Bullet = m_bullets2[i].m_Bullet_original.transformed(transform,Qt::SmoothTransformation);
                transform.rotate(-d_degree);

                //将该子弹空闲状态改为假
                m_bullets2[i].m_Free = false;
                //设置发射的子弹坐标
                m_bullets2[i].m_rX = (double)m_X + (double)m_Rect.width()*0.5 - 10.0;
                m_bullets2[i].m_rY = (double)m_Y + 20.0 ;
                m_bullets2[i].xyTrans();
                m_bullets2[i].m_Rect.moveTo(m_bullets2[i].m_X, m_bullets2[i].m_Y);

                break;
            }
        }
    }
}
void HeroPlane::burst(bool s)
{

    //累加时间间隔记录变量
    m_burst.tick(m_ashwab.holding());
    //判断如果记录数字 未达到发射间隔，直接return

    if(m_burst.holding())   {//大招 速度 射速 提升
        m_speed.setRatio(I_SPEED_BURST,s_Burst);
        m_shoot.interval.setRatio(0.5f,b_Burst);
    }else{
        m_speed.setRatio(0.0f,s_Burst);
        m_shoot.interval.setRatio(0.0f,b_Burst);
    }

    if(!(m_burst.avail()&&s&&m_charge.full()&&!m_ashwab.holding()))
    {
        return;
    }
    //到达发射时间处理
    //重置发射时间间隔记录
    m_burst.release();
    m_charge = 0;
    m_charge2+=10;
}

void HeroPlane::ashwab(bool s)
{

    //累加时间间隔记录变量
    m_ashwab.tick(m_ashwab.holding());

    if(!(m_ashwab.avail()&&s&&m_charge2.full()&&!m_ashwab.holding()))
    {
        return;
    }
    //到达发射时间处理
    //重置发射时间间隔记录
    m_ashwab.release();
    m_charge2 = 0;


}


void HeroPlane::sprint(bool s)
{

    //累加时间间隔记录变量
    m_sprint.tick();

    m_stamina++;

    if(m_sprint.holding()){
        if(m_sprint.time()+BOOST_TIME<m_sprint.timer){
            m_speed.setFlat(I_GOT_SPRINT*((float)(m_sprint.time()+2*BOOST_TIME-m_sprint.timer)/(float)m_sprint.time()),s_Sprint);
        }else if(BOOST_TIME<m_sprint.timer){
            m_speed.setFlat(I_GOT_SPRINT,s_Sprint);
        }else{
            m_speed.setFlat(I_GOT_SPRINT*(m_sprint.timer/(float)BOOST_TIME),s_Sprint);
        }
    }
    //判断如果记录数字 未达到发射间隔，直接return
    if(!m_sprint.avail()||!s||m_stamina()<sprint_cost()||m_ashwab.holding())
    {
        return;
    }
    m_sprint.timer = m_sprint.time()+2*BOOST_TIME;
    m_stamina-=sprint_cost();

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

