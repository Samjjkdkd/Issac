#include "mainscene.h"
#include "config.h"
#include "enemy_1.h"
#include "enemy_2.h"
#include <QIcon>
#include<QPainter>
#include<QMouseEvent>
#include <ctime>
#include <QSoundEffect>
#include <QSound>
#include <QLabel>
#include <math.h>

MainScene::MainScene(QWidget *parent)
    : QWidget(parent)
{

        //初始化场景
        initScene();
        playGame();

}

MainScene::~MainScene()
{
}
void MainScene::initScene()
{
    //初始化窗口大小
    setFixedSize(GAME_WIDTH,GAME_HEIGHT);

    //设置窗口标题
    setWindowTitle(GAME_TITLE);

    setWindowIcon(QIcon( GAME_ICON));

    m_Timer.setInterval(GAME_RATE);

    m_recorder = 0;

    //随机数种子
    srand((unsigned int)time(NULL));  //头文件  #include <ctime>

    //开数组
    if(m_enemys.size()<ENEMY_NUM){
        for(int i=m_enemys.size();i<ENEMY_NUM;i++){
            int type = rand()%4;
            EnemyPlane* pointer;
            if(type == 0){
                pointer = new enemy_1();
            }
            else if(type == 1 || type == 2 || type == 3){
                pointer = new enemy_2();
            }
            m_enemys.push_back(pointer);
        }
    }
}

void MainScene::enemyToScene()
{
    m_recorder++;
    if(m_recorder < ENEMY_INTERVAL)
    {
        return;
    }

    m_recorder = 0;

    for(int i = 0 ; i< ENEMY_NUM;i++)
    {
        if(m_enemys[i]->m_Free)
        {
            m_enemys[i]->m_Free = false;
            int choice = rand()%2;
            if(choice==0){            //敌机空闲状态改为false

            //设置坐标
            m_enemys[i]->startplace=0;
            m_enemys[i]->m_X = rand() % (GAME_WIDTH - m_enemys[i]->m_Rect.width());
            m_enemys[i]->m_Y = -m_enemys[i]->m_Rect.height();
            m_enemys[i]->m_Rect.moveTo(m_enemys[i]->m_X, m_enemys[i]->m_Y);
            m_enemys[i]->hp = m_enemys[i]->preset_hp;


            }
            if(choice==1){
            m_enemys[i]->startplace=1;
            m_enemys[i]->m_X = rand() % (GAME_WIDTH - m_enemys[i]->m_Rect.width());
            m_enemys[i]->m_Y = MAP_RESIZE_HEIGHT+m_enemys[i]->m_Rect.height();
            m_enemys[i]->m_Rect.moveTo(m_enemys[i]->m_X, m_enemys[i]->m_Y);
            m_enemys[i]->hp = m_enemys[i]->preset_hp;

            }
            break;
        }
    }
}
void MainScene::updatePosition()
{
    //更新地图坐标

    //发射子弹
    m_hero.shoot();
    //计算子弹坐标
    for(int i = 0 ;i < BULLET_NUM;i++)
    {
        //如果子弹状态为非空闲，计算发射位置
        if(!m_hero.m_bullets[i].m_Free)
        {
            m_hero.m_bullets[i].updatePosition();
        }
    }
    //敌机坐标计算
    for(int i = 0 ; i< ENEMY_NUM;i++)
    {
        //非空闲敌机 更新坐标
        if(m_enemys[i]->m_Free == false)
        {
            m_enemys[i]->updatePosition(m_hero.m_X,m_hero.m_Y);
        }
    }
    //计算爆炸播放的图片
    for(int i = 0 ; i < BOMB_NUM;i++)
    {
        if(m_bombs[i].m_Free == false)
        {
            m_bombs[i].updateInfo();
        }
    }

    QTransform transform;
    this->my_vector.GenerateVector();
    int isShiftPressed=this->my_vector.StateofMoveKeys[4]==QString("pressed")?1:0;
    int deltax=qFloor(this->my_vector.Vx*10000.0*I_SHOW_SPEED)/(10000+isShiftPressed*3*10000);
    int deltay=qFloor(this->my_vector.Vy*10000.0*I_SHOW_SPEED)/(10000+isShiftPressed*3*10000);

    this->m_hero.setPosition(this->m_hero.m_X+deltax,this->m_hero.m_Y+deltay);
    this->m_hero.b_direction+=this->my_vector.theta;
    this->m_hero.b_direction%=360;
    transform.rotate(-this->m_hero.b_direction+180);
    QPixmap ibashPlane = m_hero.m_Plane_original.transformed(transform, Qt::SmoothTransformation);
    m_hero.m_Plane = ibashPlane;

}

void MainScene::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    //绘制地图
    painter.drawPixmap(0,0 , m_map.m_map_1);

    painter.drawPixmap(m_hero.m_X,m_hero.m_Y,m_hero.m_Plane);
    painter.drawRect(m_hero.m_Rect);
    //painter.drawPixmap(temp_bullet.m_X,temp_bullet.m_Y,temp_bullet.m_Bullet);
    //绘制子弹
    for(int i = 0 ;i < BULLET_NUM;i++)
    {
        //如果子弹状态为非空闲，计算发射位置
        if(!m_hero.m_bullets[i].m_Free)
        {
            painter.drawPixmap(m_hero.m_bullets[i].m_X,m_hero.m_bullets[i].m_Y,m_hero.m_bullets[i].m_Bullet);
            painter.drawRect(m_hero.m_bullets[i].m_Rect);
        }
    }
    for(int i = 0 ; i< ENEMY_NUM;i++)
    {
        if(m_enemys[i]->m_Free == false)
        {
            painter.drawPixmap(m_enemys[i]->m_X,m_enemys[i]->m_Y,m_enemys[i]->m_enemy);
            painter.drawRect(m_enemys[i]->m_Rect);
        }
    }
    //绘制爆炸图片
    for(int i = 0 ; i < BOMB_NUM;i++)
    {
        if(m_bombs[i].m_Free == false)
        {
            painter.drawPixmap(m_bombs[i].m_X,m_bombs[i].m_Y,m_bombs[i].m_pixArr[m_bombs[i].m_index]);
        }
    }
    //绘制分数
    QString a = "Killed:" + QString::number(score);
    painter.setFont(QFont("黑体",20,QFont::Bold));
    painter.drawText(750,100,a);

    //绘制生命
    QString b = "HP:" + QString::number(m_hero.m_hp);
    painter.setFont(QFont("黑体",20,QFont::Bold));
    painter.drawText(1050,100,b);
}

void MainScene::playGame()
{
    //启动定时器
    m_Timer.start();
    QSoundEffect *soundEffect = new QSoundEffect();
    // 设置声音源文件的路径
    soundEffect->setSource(QUrl::fromLocalFile(SOUND_BACKGROUND));
    // 音频循环的次数
    soundEffect->setLoopCount(114514);
    // 音量
    soundEffect->setVolume(1);
    soundEffect->play();


    //监听定时器
    connect(&m_Timer,&QTimer::timeout,[=](){
        //更新游戏中元素的坐标

        //敌机出场
        enemyToScene();
        updatePosition();
        collisionDetection();

        //temp_bullet.m_Free = false;
        //temp_bullet.updatePosition();
        //重新绘制图片
        update();

    });
}
void MainScene::mouseMoveEvent(QMouseEvent *event)
{
    int x = event->x() - m_hero.m_Rect.width()*0.5; //鼠标位置 - 飞机矩形的一半
    int y = event->y() - m_hero.m_Rect.height()*0.5;

    //边界检测
    if(x <= 0 )
    {
        x = 0;
    }
    if(x >= GAME_WIDTH - m_hero.m_Rect.width())
    {
        x = GAME_WIDTH - m_hero.m_Rect.width();
    }
    if(y <= 0)
    {
        y = 0;
    }
    if(y >= GAME_HEIGHT - m_hero.m_Rect.height())
    {
        y = GAME_HEIGHT - m_hero.m_Rect.height();
    }
    m_hero.setPosition(x,y);
}


//按键事件
void MainScene::keyPressEvent(QKeyEvent *event)
{

    //人物移动
    if(event->key()==Qt::Key_Left)
    {
        this->my_vector.StateofMoveKeys[0]=QString("pressed");
    }
    if(event->key()==Qt::Key_Right)
    {
        this->my_vector.StateofMoveKeys[3]=QString("pressed");
    }
    if(event->key()==Qt::Key_Up)
    {
        this->my_vector.StateofMoveKeys[1]=QString("pressed");
    }
    if(event->key()==Qt::Key_Down)
    {
        this->my_vector.StateofMoveKeys[2]=QString("pressed");
    }
    if(event->key()==Qt::Key_A)
    {
        this->my_vector.StateofMoveKeys[5]=QString("pressed");
    }
    if(event->key()==Qt::Key_D)
    {
        this->my_vector.StateofMoveKeys[6]=QString("pressed");
    }
    if(event->key()==Qt::Key_Shift)//低速移动键，参照東方project机制
    {
        this->my_vector.StateofMoveKeys[4]=QString("pressed");
    }
}

//松键事件
void MainScene::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key()==Qt::Key_Left)
    {
        this->my_vector.StateofMoveKeys[0]=QString("unpressed");
    }
    if(event->key()==Qt::Key_Right)
    {
        this->my_vector.StateofMoveKeys[3]=QString("unpressed");
    }
    if(event->key()==Qt::Key_Up)
    {
        this->my_vector.StateofMoveKeys[1]=QString("unpressed");
    }
    if(event->key()==Qt::Key_Down)
    {
        this->my_vector.StateofMoveKeys[2]=QString("unpressed");
    }
    if(event->key()==Qt::Key_A)
    {
        this->my_vector.StateofMoveKeys[5]=QString("unpressed");
    }
    if(event->key()==Qt::Key_D)
    {
        this->my_vector.StateofMoveKeys[6]=QString("unpressed");
    }
    if(event->key()==Qt::Key_Shift)//低速移动键，参照東方project机制
    {
        this->my_vector.StateofMoveKeys[4]=QString("unpressed");
    }
}

//古法碰撞检测
bool isIntersect(const QRect& a, const QRect& b){
    return (a.x()+a.width()>b.x()&&a.y()+a.height()>b.y())
           &&(a.x()<b.x()+b.width()&&a.y()<b.y()+b.height());
}

void MainScene::collisionDetection()
{
    QSoundEffect * bombSound = new QSoundEffect(this);
    bombSound->setSource(QUrl::fromLocalFile(SOUND_BOMB));

    //遍历所有非空闲的敌机
    for(int i = 0 ;i < ENEMY_NUM;i++)
    {
        if(m_enemys[i]->m_Free)
        {
            //空闲飞机 跳转下一次循环
            continue;
        }

        //遍历所有 非空闲的子弹
        for(int j = 0 ; j < BULLET_NUM;j++)
        {
            if(m_hero.m_bullets[j].m_Free||m_enemys[i]->m_Free)
            {
                //空闲子弹 跳转下一次循环
                continue;
            }

            //如果子弹矩形框和敌机矩形框相交，发生碰撞，同时变为空闲状态即可
            if(m_enemys[i]->m_Rect.intersects(m_hero.m_bullets[j].m_Rect))
            {
                m_enemys[i]->hp--;
                m_hero.m_bullets[j].m_Free = true;
                if(m_enemys[i]->hp==0){
                    score++;//得分增加

                    m_enemys[i]->m_Free = true;

                    //播放爆炸效果
                    for(int k = 0 ; k < BOMB_NUM;k++)
                    {
                        if(m_bombs[k].m_Free)
                        {
                            //爆炸状态设置为非空闲
                            m_bombs[k].m_Free = false;
                            //更新坐标
                            bombSound->play();
                            m_bombs[k].m_X = m_enemys[i]->m_X;
                            m_bombs[k].m_Y = m_enemys[i]->m_Y;
                            k = BOMB_NUM;
                            break;
                        }
                    }
                }
            }
            if(m_enemys[i]->m_Rect.intersects(m_hero.m_Rect)){
                m_hero.m_hp-= m_enemys[i]->hp;
                m_enemys[i]->m_Free = true;
                for(int k = 0 ; k < BOMB_NUM;k++)
                {
                    if(m_bombs[k].m_Free)
                    {
                        //爆炸状态设置为非空闲
                        m_bombs[k].m_Free = false;
                        //更新坐标
                        bombSound->play();
                        m_bombs[k].m_X = m_enemys[i]->m_X;
                        m_bombs[k].m_Y = m_enemys[i]->m_Y;
                        k = BOMB_NUM;
                        break;
                    }
                }
            }

        }

    }
}

