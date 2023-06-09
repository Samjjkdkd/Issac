#include "movevector.h"

MoveVector::MoveVector()
{
    for(int i=0;i<20;i++)
    {
        this->StateofMoveKeys[i]=QString("unpressed");
    }
    this->toZeroVector();
    this->skill_up = true;
    this->burst_up = true;
    this->sprint_up = true;
    this->ashwab_up = true;
}

void MoveVector::toZeroVector()//归零向量
{
    this->Vx=0;
    this->Vy=0;
    this->Vf=0;
    this->theta=0;
    this->skill = false;
    this->burst = false;
    this->sprint = false;
    this->ashwab = false;
    this->cheat = false;
    this->confirm = false;
}



void MoveVector::GenerateVector(int input_type)//根据按键状态生成单位向量
{
    this->toZeroVector();
    if(this->StateofMoveKeys[0]==QString("pressed"))//左
    {
        if(input_type)
            this->AddVx(-1.0);
        else
            this->changeTheta(ROTATE_SENSITIVITY);
    }
    if(this->StateofMoveKeys[1]==QString("pressed"))//上
    {
        this->AddVf(1.0);
        this->AddVy(-1.0);
    }
    if(this->StateofMoveKeys[2]==QString("pressed"))//下
    {
        this->AddVf(-1.0);
        this->AddVy(1.0);
    }
    if(this->StateofMoveKeys[3]==QString("pressed"))//右
    {
        if(input_type)
            this->AddVx(1.0);
        else
            this->changeTheta(-ROTATE_SENSITIVITY);
    }
    if(this->StateofMoveKeys[5]==QString("pressed"))//A
    {
        if(input_type)
            this->changeTheta(ROTATE_SENSITIVITY);
    }
    if(this->StateofMoveKeys[6]==QString("pressed"))//D
    {
        if(input_type)
            this->changeTheta(-ROTATE_SENSITIVITY);
    }
    if(this->StateofMoveKeys[7]==QString("pressed"))//E
    {
        if(this->skill_up){
            this->skill= true;
            this->skill_up = false;
        }else{
            this->skill = false;
        }
    }
    if(this->StateofMoveKeys[8]==QString("pressed"))//Q
    {
        if(this->burst_up){
            this->burst = true;
            this->burst_up = false;
        }else{
            this->burst = false;
        }
    }
    if(this->StateofMoveKeys[4]==QString("pressed"))//Shift
    {
        if(this->sprint_up){
            this->sprint = true;
            this->sprint_up = false;
        }else{
            this->sprint = false;
        }
    }
    if(this->StateofMoveKeys[11]==QString("pressed"))//R
    {
        if(this->ashwab_up){
            this->ashwab = true;
            this->ashwab_up = false;
        }else{
            this->ashwab = false;
        }
    }

    if(this->StateofMoveKeys[7]==QString("unpressed"))//E
    {
        this->skill_up=true;
    }
    if(this->StateofMoveKeys[8]==QString("unpressed"))//Q
    {
        this->burst_up=true;
    }
    if(this->StateofMoveKeys[4]==QString("unpressed"))//Shift
    {
        this->sprint_up=true;
    }
    if(this->StateofMoveKeys[11]==QString("unpressed"))//R
    {
        this->ashwab_up=true;
    }
    if(this->StateofMoveKeys[10]==QString("pressed"))//S
    {
    }
    if(this->StateofMoveKeys[8]==QString("pressed"))//Z
    {
    }
    if(this->StateofMoveKeys[12]==QString("pressed"))//G
    {
        this->cheat = true;
    }
    if(this->StateofMoveKeys[13]==QString("pressed"))//Space
    {
        this->confirm = true;
    }
    qreal length=qSqrt(this->Vx*this->Vx+this->Vy*this->Vy);
    if(length!=qreal(0.0))//向量归一化
    {
        this->Vx=this->Vx/length;
        this->Vy=this->Vy/length;
    }
}

void MoveVector::AddVx(qreal deltax)
{
    this->Vx+=deltax;
}

void MoveVector::AddVy(qreal deltay)
{
    this->Vy+=deltay;
}

void MoveVector::AddVf(qreal deltaf)
{
    this->Vf+=deltaf;
}


void MoveVector::changeTheta(qreal dtheta){
    this->theta+=dtheta;
}
