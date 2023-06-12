#ifndef AUXSTRUCTURE_H
#define AUXSTRUCTURE_H

#define BONUS_INDEX_MAX 20

//一种很新的，可以变的量
//如nesh的攻击力，由基础攻击力(base)，攻击力加成百分比(bonus_ratio)，和小攻击(bonus_flat)构成
//通过operator()来获取当前值
class VariableData{
public:
    float base;

    //不同加成来源的加成分开存储
    //最大索引先给了20 感觉没必要放进config里
    float bonus_flat[BONUS_INDEX_MAX];
    float bonus_ratio[BONUS_INDEX_MAX];
    float bonus_flat_sum;
    float bonus_ratio_sum;
    float max = 16777216.0f;
    float return_val;
    VariableData(float a);
    VariableData();

    float operator()();

    //设置第i个加成
    void setFlat(float a, int i);
    void setRatio(float a, int i);

    //设置最大值
    void setMax(float a);

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
    float max();//获得当前上限
    bool full();//是否为满
    void fill();//填满
    float progress();//获得当前进度

    void addMax(float a);
    void addVal(float a);


    //支持正常赋值运算符
    void operator+=(float a);
    void operator-=(float a);
    void operator=(float a);
    void operator+=(int a);
    void operator-=(int a);
    void operator=(int a);
    float operator++(int);

    float operator()();

    void valCheck();//数值合法性校验

};


//用于管理某些事件，包含计时、间隔、持续时间等
class EventManager
{
public:
    int recorder;
    int interval();
    VariableData freq;
    VariableData time;
    int timer;
    bool without_timer = false;

    bool tick(bool timerOnly = false);//计时一次
    bool avail();//是否可用
    void release();//释放
    bool holding();//是否进行中
    float getCD();//获得冷却
    void ready();//使可用化
    float progress();//获得进度

    EventManager(int,int,bool init_avail = true);
    EventManager(int);
    EventManager();
};

#endif // AUXSTRUCTURE_H
