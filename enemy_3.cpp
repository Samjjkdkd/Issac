#include "enemy_3.h"
#include <QMovie>

enemy_3::enemy_3():EnemyPlane()
{
    //敌机资源加载
    m_enemy[0].load(ENEMY_PATH_3_1);
    m_enemy[1].load(ENEMY_PATH_3_2);
    m_enemy[0] = m_enemy[0].scaled(RESIZE_WIDTH_3, RESIZE_HEIGHT_3, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    m_enemy[1] = m_enemy[1].scaled(RESIZE_WIDTH_3, RESIZE_HEIGHT_3, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    //敌机速度
    m_Speed = ENEMY_SPEED_3;
    //绘制碰撞箱
    m_Rect.setWidth(RESIZE_WIDTH_3);
    m_Rect.setHeight(RESIZE_HEIGHT_3);
    //hp
    hp = ENEMY_HP_3;
    preset_hp = ENEMY_HP_3;
    type = 3;
}
void enemy_3::updateInfo(){
    m_Recorder++;
    if(m_Recorder < ENEMY_3_INTERVAL)
    {
        //记录爆炸间隔未到，直接return，不需要切图
        return;
    }
    //重置记录
    m_Recorder = 0;

    //切换爆炸播放图片
    m_index++;
    //注：数组中的下标从0开始，最大是6
    //如果计算的下标大于6，重置为0
    if(m_index > ENEMY_3_MAX-1)
    {
        m_index = 0;
    }
}

void enemy_3::drawEnemy(QPainter &t){
    t.drawPixmap(m_X,m_Y,m_enemy[m_index]);
}
