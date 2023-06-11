#include "virtualbutton.h"

VirtualButton::VirtualButton()
{

}

VirtualButton::VirtualButton(int _x,int _y,int _w,int _h,QString _t)
{
    x = _x;
    y = _y;
    width = _w;
    height = _h;
    text = _t;
    rect.setWidth(width);
    rect.setHeight(height);
    rect.moveTo(x,y);

}

void VirtualButton::drawButton(QPainter &painter)
{
    painter.setOpacity(1.0);
    painter.setPen(QPen(Qt::white, 1));
    if(isHanged)
        painter.setBrush(QBrush(QColor(0xfaffa9)));
    painter.drawRect(rect);
    if(isHanged)
        painter.setPen(QPen(Qt::black,1));
    painter.setFont(QFont("黑体",30,QFont::Bold));
    painter.drawText(x+width/2-text.length()*30/2,y+height/2+30/2,text);
    painter.setBrush(Qt::NoBrush);
}
