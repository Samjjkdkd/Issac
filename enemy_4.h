#ifndef ENEMY_4_H
#define ENEMY_4_H
#include "enemy.h"
#include "config.h"


class enemy_4:public EnemyPlane
{
public:
    enemy_4();
    int direction = 0;
    void updatePosition();
    std::string state = "a";
    int recorder_4 = 0;
    void avail_enemy_4(int,int);
    void decide_dash(int,int);
    bool dash_end();
    void drawSquare();
    int square_X_1,square_Y_1,square_X_2,square_Y_2;


};

#endif // ENEMY_4_H
