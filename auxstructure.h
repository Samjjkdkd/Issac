#ifndef AUXSTRUCTURE_H
#define AUXSTRUCTURE_H

#define BONUS_INDEX_MAX 20

//一种很新的，可以变的量
//如nesh的攻击力，由基础攻击力(base)，攻击力加成百分比(bonus_ratio)，和小攻击(bonus_flat)构成
//通过operator()来获取当前值
class VariableData{
public:
    float base;
    float bonus_flat[BONUS_INDEX_MAX];
    float bonus_ratio[BONUS_INDEX_MAX];
    float bonus_flat_sum;
    float bonus_ratio_sum;
    VariableData(float a);
    VariableData();
    float operator()();
    void setFlat(float a, int i);
    void setRatio(float a, int i);

    void operator=(float _base);
};


//类条变量，如血条，充能条，体力条
class TubeLikeData{
public:
    float val;
    VariableData val_max;
    TubeLikeData(float _v, float _m);
    TubeLikeData(float _v);
    TubeLikeData();
    float max();
    bool full();
    float progress();

    void addMax(float a);
    void addVal(float a);

    void operator+=(float a);
    void operator-=(float a);
    void operator=(float a);
    void operator+=(int a);
    void operator-=(int a);
    void operator=(int a);
    float operator++(int);

    float operator()();
    void valCheck();

};


//用于管理某些事件，包含计时、间隔、持续时间等
class EventManager
{
public:
    int recorder;
    int interval;
    int time;
    EventManager();
};

#endif // AUXSTRUCTURE_H
