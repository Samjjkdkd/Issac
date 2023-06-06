#ifndef ENEMY_3_H
#define ENEMY_3_H
#include "enemy.h"
#include <QLabel>


class enemy_3:public EnemyPlane
{
public:
    //敌机资源对象
    QPixmap m_enemy[2];
    void drawEnemy(QPainter &t);
    void updateInfo();
    enemy_3();
    int m_Recorder=0;
    int m_index = 0;
};

#endif // ENEMY_3_H
