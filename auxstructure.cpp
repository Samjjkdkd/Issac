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

float VariableData::operator()(){
    bonus_ratio_sum = 0.0f;
    bonus_flat_sum = 0.0f;
    for(int i = 0; i<BONUS_INDEX_MAX;++i){
        bonus_flat_sum+=bonus_flat[i];
        bonus_ratio_sum+=bonus_ratio[i];
    }

    return base*(1.0f+bonus_ratio_sum)+bonus_flat_sum;
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

EventManager::EventManager()
{

}
