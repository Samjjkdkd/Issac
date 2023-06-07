#include "bloodtrail.h"

bloodtrail::bloodtrail()
{
    //初始化爆炸图片数组

    //初始化坐标
    m_X = 0;
    m_Y = 0;

    //初始化空闲状态
    m_Free = true;

    //当前播放图片下标
    m_index = 0;

    //爆炸间隔记录
    m_Recoder = BLOOD_INTERVAL;
}
void bloodtrail::updateInfo()
{
    //空闲状态
    if(m_Free)
    {
        return;
    }

    m_Recoder++;
    if(m_Recoder < BLOOD_INTERVAL)
    {
        //记录爆炸间隔未到，直接return，不需要切图
        return;
    }
    //重置记录
    m_Recoder = 0;

    //切换爆炸播放图片
    m_index++;

    m_transparentrate = 0.8-((float)(m_index)*0.75/BLOOD_MAX);
    //注：数组中的下标从0开始，最大是4
    //如果计算的下标大于4，重置为0
    if(m_index > BLOOD_MAX - 1)
    {
        m_index = 0;
        m_Recoder = BLOOD_INTERVAL;
        m_Free = true;
    }
}
