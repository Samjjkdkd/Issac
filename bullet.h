#ifndef BULLET_H
#define BULLET_H
#include <QPixmap>

class Bullet
{
public:
    Bullet(int i);
    Bullet();

    void xyTrans(){
        this->m_X = (int)this->m_rX;
        this->m_Y = (int)this->m_rY;
    }

    //更新子弹坐标
    void updatePosition();

public:
    //子弹资源对象
    QPixmap m_Bullet_original;
    QPixmap m_Bullet;
    QPixmap m_sBullet_original;
    QPixmap m_sBullet;
    //子弹坐标
    int m_X;
    int m_Y;
    double m_rX;
    double m_rY;
    //子弹移动速度
    int m_Speed;
    //子弹是否闲置
    bool m_Free;
    //子弹的矩形边框（用于碰撞检测）
    QRect m_Rect;
    //子弹方向
    int m_direction = 0;
};

#endif // BULLET_H
