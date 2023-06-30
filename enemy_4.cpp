    #include "enemy_4.h"
    enemy_4::enemy_4()
    {
    //敌机资源加载
    m_enemy.load(ENEMY_PATH_4);
    m_enemy = m_enemy.scaled(RESIZE_WIDTH_4, RESIZE_HEIGHT_4, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    //敌机速度
    m_Speed = ENEMY_SPEED_4;
    //绘制碰撞箱
    m_Rect.setWidth(RESIZE_WIDTH_4);
    m_Rect.setHeight(RESIZE_HEIGHT_4);
    //hp
    hp = ENEMY_HP_4;
    preset_hp = ENEMY_HP_4;
    type = -1;
    m_Free = 1;
    state="wait";
    recorder_4 = 0;


    }
    void enemy_4:: updatePosition(){


    if(direction==0){

        if(m_Y>=GAME_HEIGHT){
            m_Free= 1;
            state = "wait";

        }
        else{
        m_Y+=ENEMY_SPEED_4;



        }
     m_Rect.moveTo(m_X,m_Y);
    }
    else if(direction==1){
     if(m_Y<=-RESIZE_HEIGHT_4){
        m_Free= 1;
        state = "wait";

     }
     else{
        m_Y-=ENEMY_SPEED_4;



     }
     m_Rect.moveTo(m_X,m_Y);
    }
    else if(direction==2){
     if(m_X>=GAME_WIDTH){
        m_Free= 1;
        state = "wait";

     }
     else{
        m_X+=ENEMY_SPEED_4;



     }
     m_Rect.moveTo(m_X,m_Y);
    }
    else{
     if(m_X<=-RESIZE_WIDTH_4){
        m_Free= 1;
        state = "wait";

     }
     else{
        m_X-=ENEMY_SPEED_4;



     }
     m_Rect.moveTo(m_X,m_Y);
    }
    }

    void enemy_4::decide_dash(int x,int y){




        int choice = rand()%4;
        direction = choice;
        if(choice ==0){
            direction = 0;
            m_X = x;
            m_Y =  -m_Rect.height();
            m_Rect.moveTo(m_X,m_Y);
            hp = preset_hp;

        }
        if(choice==1){
            direction = 1;
            m_X = x;
            m_Y =  GAME_HEIGHT+m_Rect.height();
            m_Rect.moveTo(m_X,m_Y);
            hp = preset_hp;

        }
        if(choice==2){
            direction = 2;
            m_Y = y;
            m_X =  -m_Rect.width();
            m_Rect.moveTo(m_X,m_Y);
            hp = preset_hp;

        }
        if(choice==3){
            direction = 3;
            m_Y = y;
            m_X = GAME_WIDTH +m_Rect.width();
            m_Rect.moveTo(m_X,m_Y);
            hp = preset_hp;

        }



    }
    bool enemy_4::dash_end(){
        if(direction==0){
            return m_Y>=GAME_HEIGHT;
        }
        else if(direction==1){
            return m_Y<=RESIZE_HEIGHT_4;
        }
        else if(direction==2){
            return m_X>=RESIZE_WIDTH_4;
        }
        else{
            return m_X<=-RESIZE_WIDTH_4;
        }
    }

    void enemy_4::avail_enemy_4(int x,int y)
    {
     if(state=="wait"){
        recorder_4++;
        if(recorder_4>=1000){
            if(rand()%2==0){
            recorder_4=0;
            state = "show";
            decide_dash(x,y);

            }


        }

     }
     else if(state=="show"){
        recorder_4++;
        if(recorder_4>=60){
            recorder_4=0;
            state = "dash";

        }
     }
     else if(state=="dash"){
        if(dash_end()){
            if(rand()%4!=0){
            decide_dash(x,y);
            state="show";

            }
            else{
            state="wait";

            }

        }

     }




    }

