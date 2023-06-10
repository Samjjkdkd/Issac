#ifndef ENERGY_H
#define ENERGY_H

#include "config.h"
#include <QPixmap>

class Energy
{
public:
    Energy();



    //更新状态
    void updateInfo();

    //资源对象
    QPixmap m_energys[2];
    QPixmap m_energy;

    int index = 0;

    //坐标
    int m_X;
    int m_Y;

    //记录消失时间
    int m_recorder;

    //能量值
    float m_energy_amount = 3;

    //记录状态时间
    int m_recorder_1;

    //是否闲置
    bool m_Free;
    //矩形边框（用于碰撞检测）
    QRect m_Rect;

};
#endif // ENERGY_H
