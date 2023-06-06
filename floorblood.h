#ifndef FLOORBLOOD_H
#define FLOORBLOOD_H
#include "config.h"
#include <QPixmap>

class FloorBlood
{
public:
    FloorBlood();
    //放爆炸资源数组
    QPixmap blood;

    //爆炸位置
    int m_X;
    int m_Y;

    //爆炸状态
    bool m_Free;
};

#endif // FLOORBLOOD_H
