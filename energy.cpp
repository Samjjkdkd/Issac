#include "energy.h"

Energy::Energy()
{
    //子弹资源

    m_energys[0].load(ENERGY_PATH);
    m_energys[1].load(ENERGY_PATH_1);

    m_energy = m_energys[0];
    //子弹坐标 初始坐标可随意设置，后期会重置
    m_X = GAME_WIDTH*0.5 - m_energy.width()*0.5;
    m_Y = GAME_HEIGHT;

    //状态
    m_Free = true;

    m_recorder = 0;

    m_recorder_1 = 0;


    //矩形框
    m_Rect.setWidth(m_energy.width());
    m_Rect.setHeight(m_energy.height());
    m_Rect.moveTo(m_X,m_Y);
}
void Energy::updateInfo()
{
    //如果子弹是空闲状态，不需要坐标计算
    //玩家飞机可以控制子弹的空闲状态为false
    if(m_Free)
    {
        return;
    }

    if(--m_recorder<=0){
        m_Free = true;
        return;
    }

    m_recorder_1++;
    if(m_recorder_1>ENERGY_BLING){
        m_recorder_1 = 0;
        index=index?0:1;
        m_energy = m_energys[index];

    }


}
