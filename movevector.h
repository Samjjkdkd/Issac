#ifndef MOVEVECTOR_H
#define MOVEVECTOR_H
#include <QtMath>
#include <QString>
#include "config.h"



class MoveVector
{
public:
    MoveVector();
    void toZeroVector();
    void GenerateVector();
    void AddVx(qreal deltax);
    void AddVy(qreal deltay);
    void changeTheta(qreal dtheta);
    QString StateofMoveKeys[10];//记录键盘相关键的状态
    qreal Vx;
    qreal Vy;
    qreal theta;
    bool skill;//技能是否释放
    bool burst;//爆发是否释放
    bool sprint;//是否冲刺

    //由于各种技能按键均为点击触发式，故需要保证抬起之后才能再按
    bool skill_up;
    bool burst_up;
    bool sprint_up;

};

#endif // MOVEVECTOR_H
