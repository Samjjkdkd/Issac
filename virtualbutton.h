#ifndef VIRTUALBUTTON_H
#define VIRTUALBUTTON_H

#include "qpainter.h"

class VirtualButton
{
public:
    VirtualButton();
    VirtualButton(int _x,int _y,int _w,int _h,QString _t);

    int x;
    int y;

    QRect rect;

    bool isHanged = false;
    QString text;
    int width;
    int height;


    void drawButton(QPainter& painter);
    ~VirtualButton(){};
};

#endif // VIRTUALBUTTON_H
