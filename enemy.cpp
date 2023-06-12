#include "enemy.h"
#include "config.h"
#include <math.h>
EnemyPlane::EnemyPlane()
{

    //敌机位置
    m_X = 0;
    m_Y = 0;

    //敌机状态
    m_Free = true;
    //敌机矩形

    m_Rect.moveTo(m_X,m_Y);
}

void EnemyPlane::updatePosition(int hero_X,int hero_Y)
{
    //空闲状态，不计算坐标
    if(m_Free)
    {
        return;
    }
//    if(startplace==0){
//    m_Y += m_Speed;
//    }
//    if(startplace==1){
//    m_Y-=m_Speed;
//    }

    int vector_X = hero_X-m_X;
    int vector_Y = hero_Y-m_Y;
    vector_X *= rand()%10>=2?rand()%30:0;
    vector_Y *= rand()%10>=2?rand()%30:0;

//    m_X += m_Speed * vector_X/100;
//    m_Y += m_Speed * vector_Y/100;
    float length = sqrt(vector_X*vector_X+vector_Y*vector_Y);


    if(length>0.0f){
        int dX = (int) (((float)vector_X/length)*m_Speed*10000.0)/10000;
        int dY = (int) (((float)vector_Y/length)*m_Speed*10000.0)/10000;
        m_X +=dX;
        m_Y +=dY;
        m_Rect.moveTo(m_X,m_Y);
    }

    if(m_Y >= GAME_HEIGHT + m_Rect.height()||m_Y <= - m_Rect.height()
        ||m_X >= GAME_WIDTH + m_Rect.width()||m_X <= - m_Rect.width())
    {
        m_Free = true;
    }

}


