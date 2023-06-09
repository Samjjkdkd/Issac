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
    void GenerateVector(int input_type);
    void AddVx(qreal deltax);
    void AddVy(qreal deltay);
    void AddVf(qreal deltay);//前进
    void changeTheta(qreal dtheta);
    QString StateofMoveKeys[20];//记录键盘相关键的状态
    qreal Vx;
    qreal Vy;
    qreal Vf;
    qreal theta;
    bool skill;//技能是否释放
    bool burst;//爆发是否释放
    bool sprint;//是否冲刺
    bool ashwab;//是否z

    //由于各种技能按键均为点击触发式，故需要保证抬起之后才能再按
    bool skill_up;
    bool burst_up;
    bool sprint_up;
    bool ashwab_up;


    bool cheat;

    bool confirm;

};

#endif // MOVEVECTOR_H
