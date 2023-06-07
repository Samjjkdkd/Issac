#ifndef BLOODTRAIL_H
#define BLOODTRAIL_H
#include "config.h"
#include <QPixmap>
#include <QVector>

class bloodtrail
{
public:
    bloodtrail();

    //更新信息（播放图片下标、播放间隔）
    void updateInfo();

    //放爆炸资源数组


    //爆炸位置
    int m_X;
    int m_Y;

    //爆炸状态
    bool m_Free;

    //爆炸切图的时间间隔
    int m_Recoder;

    //爆炸时加载的图片下标
    int m_index;

    //画笔透明度
    float m_transparentrate=1;

    int type = 0;

};

#endif // BLOODTRAIL_H
