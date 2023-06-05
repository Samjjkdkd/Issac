#include "enemy_1.h"



enemy_1::enemy_1():EnemyPlane()
{
    //敌机资源加载
    m_enemy.load(ENEMY_PATH_1);
    m_enemy = m_enemy.scaled(RESIZE_WIDTH_1, RESIZE_HEIGHT_1, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    //敌机速度
    m_Speed = ENEMY_SPEED_1;
    //绘制碰撞箱
    m_Rect.setWidth(RESIZE_WIDTH_1);
    m_Rect.setHeight(RESIZE_HEIGHT_1);
    //hp
    hp = ENEMY_HP_1;
    preset_hp = ENEMY_HP_1;
}
