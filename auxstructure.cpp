#include "auxstructure.h"

VariableData::VariableData(float a):base(a){
    for(int i = 0; i<BONUS_INDEX_MAX;++i){
        bonus_flat[i] = 0.0f;
        bonus_ratio[i] = 0.0f;
    }
    bonus_ratio_sum = 0.0f;
    bonus_flat_sum = 0.0f;
}

VariableData::VariableData():VariableData::VariableData(0.0f){}

void VariableData::operator=(float _base){
    base = _base;
}

void VariableData::setFlat(float a, int i){
    bonus_flat[i]=a;
}
void VariableData::setRatio(float a, int i){
    bonus_ratio[i]=a;
}
void VariableData::setMax(float a){
    max=a;
}

float VariableData::operator()(){
    bonus_ratio_sum = 0.0f;
    bonus_flat_sum = 0.0f;
    for(int i = 0; i<BONUS_INDEX_MAX;++i){
        bonus_flat_sum+=bonus_flat[i];
        bonus_ratio_sum+=bonus_ratio[i];
    }
    return_val = base*(1.0f+bonus_ratio_sum)+bonus_flat_sum;
    return return_val > max? max:return_val;
}

TubeLikeData::TubeLikeData(float _v, float _m):val(_v),val_max(_m){}

TubeLikeData::TubeLikeData(float _v):TubeLikeData::TubeLikeData(_v,_v){}

TubeLikeData::TubeLikeData():TubeLikeData::TubeLikeData(0.0,0.0){}

void TubeLikeData::addMax(float a){
    val_max.setFlat(a,0);
    valCheck();
}


void TubeLikeData::addVal(float a){
    val+=a;
    valCheck();
}

void TubeLikeData::operator+=(float a){
    addVal(a);
}

void TubeLikeData::operator-=(float a){
    addVal(-a);
}

void TubeLikeData::operator=(float a){
    val = a;
    valCheck();
}

void TubeLikeData::operator+=(int a){
    addVal((float)a);
}

void TubeLikeData::operator-=(int a){
    addVal(-(float)a);
}

void TubeLikeData::operator=(int a){
    val = (float)a;
    valCheck();
}

float TubeLikeData::operator()(){
    return val;
}

float TubeLikeData::max(){
    return val_max();
}

bool TubeLikeData::full(){
    return val == val_max();
}

void TubeLikeData::fill(){
    val = val_max();
}

void TubeLikeData::valCheck(){
    val = val>val_max()?val_max():val;
}

float TubeLikeData::progress(){
    return val / val_max();
}

float TubeLikeData::operator++(int){
    float tmp = val;
    addVal(1.0f);
    return tmp;
}

EventManager::EventManager(int i,int t, bool init_avail)
{
    freq = 100.0f / (float)i;
    time = t;
    if(init_avail)  recorder = interval();
    timer = 0;

}

EventManager::EventManager(int i):EventManager::EventManager(i,0)
{
    without_timer = true;
}

EventManager::EventManager():EventManager::EventManager(0,0)
{
}

bool EventManager::tick(bool timerOnly)
{
    if(!timerOnly){
        recorder++;
        recorder = recorder>interval()?interval():recorder;
    }
    if(timer)  {
        timer--;
        return true;
    }
    return false;

}

bool EventManager::avail()
{
    return recorder >= interval();
}

int EventManager::interval()
{
    return (int)(100.0f/freq());
}

bool EventManager::holding()
{
    return timer?true:false;
}

void EventManager::release()
{
    timer = (int)time();
    recorder = 0;
}

float EventManager::getCD()
{
    return interval() - (float)recorder;
}

void EventManager::ready()
{
    recorder = interval();
    //timer = 0;
}

float EventManager::progress()
{
    return (float)recorder/interval();
}

