#include "enemy_2.h"

enemy_2::enemy_2()
{
    //敌机资源加载
    m_enemy.load(ENEMY_PATH_2);
    m_enemy = m_enemy.scaled(RESIZE_WIDTH_2, RESIZE_HEIGHT_2, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    //敌机速度
    m_Speed = ENEMY_SPEED_2;
    //绘制碰撞箱
    m_Rect.setWidth(RESIZE_WIDTH_2);
    m_Rect.setHeight(RESIZE_HEIGHT_2);
    //hp
    hp = ENEMY_HP_2;
    preset_hp = ENEMY_HP_2;
}
