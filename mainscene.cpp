#include "mainscene.h"
#include "config.h"
#include "enemy_1.h"
#include "enemy_2.h"
#include "enemy_3.h"
#include <QIcon>
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>
#include <QPen>
#include <ctime>
#include <QSoundEffect>
#include <QSound>
#include <QLabel>
#include <QColor>
#include <math.h>
#include <complex>

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
    //加载资源。
    bgsound = new QSoundEffect(this);
    bgsound->setSource(QUrl::fromLocalFile(SOUND_BGM_PATH));
    bgsound->setLoopCount(-1);
    bgsound->setVolume(0.25f);

    m_blood[0].load(BLOOD_PATH_1);
    m_blood[0] = m_blood[0].scaled(RESIZE_BLOOD_WIDTH,RESIZE_BLOOD_HEIGHT, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    m_blood[1].load(BLOOD_PATH_2);
    m_blood[1] = m_blood[1].scaled(RESIZE_BLOOD_WIDTH,RESIZE_BLOOD_HEIGHT, Qt::KeepAspectRatio, Qt::SmoothTransformation);

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
            int type_ = rand()%4;
            EnemyPlane* pointer;
            if(type_ == 0){
                pointer = new enemy_1();
            }
            else if(type_ == 1 || type_ == 2){
                pointer = new enemy_2();
            }
            else if(type_ == 3){
                pointer = new enemy_3();
            }
            m_enemys.push_back(pointer);
        }
    }
    for(int i=0;i<BLOOD_NUM;i++){
        int type_ = rand()%2;
        if(type_ == 0){
            m_bloodtrail[i].type = 0;
        }
        else if(type_ == 1){
            m_bloodtrail[i].type = 1;
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
            if(m_enemys[i]->type == 3){
                m_enemys[i]->updateInfo();
            }
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
    for(int i=0;i<BLOOD_NUM;i++){
        if(m_bloodtrail[i].m_Free==false){
            m_bloodtrail[i].updateInfo();
        }
    }

    QTransform transform;
    this->my_vector.GenerateVector();

    m_hero.skill(this->my_vector.skill);

    int isShiftPressed=this->my_vector.StateofMoveKeys[4]==QString("pressed")?1:0;
    int deltax=qFloor(this->my_vector.Vx*10000.0*I_SHOW_SPEED)/(10000+isShiftPressed*3*10000);
    int deltay=qFloor(this->my_vector.Vy*10000.0*I_SHOW_SPEED)/(10000+isShiftPressed*3*10000);

    this->m_hero.setPosition(this->m_hero.m_X+deltax,this->m_hero.m_Y+deltay);
    this->m_hero.b_direction+=this->my_vector.theta;
    this->m_hero.b_direction%=360;
    transform.translate(RESIZE_RADIUS/2,RESIZE_RADIUS/2);
    int alpha = -this->m_hero.b_direction+180;
    transform.rotate(alpha);
    transform.translate(-RESIZE_RADIUS/2,-RESIZE_RADIUS/2);
    QPixmap ibashPlane = m_hero.m_Plane_original.transformed(transform, Qt::SmoothTransformation);
    m_hero.m_Plane = ibashPlane;

    alpha %= 90;
    alpha = abs(alpha);
    m_hero.shiftx = qCos((45-alpha)*Pi/180) * sqrt(2) / 2 * RESIZE_RADIUS - RESIZE_RADIUS/2;
    m_hero.shifty = qSin((45+alpha)*Pi/180) * sqrt(2) / 2 * RESIZE_RADIUS - RESIZE_RADIUS/2;
}

void MainScene::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing);
    //绘制地图
    painter.drawPixmap(0,0 , m_map.m_map_1);

    //绘制血迹
    for(int i = 0 ; i < BLOOD_NUM;i++)
    {
        if(m_bloodtrail[i].m_Free == false)
        {
            painter.setOpacity(m_bloodtrail[i].m_transparentrate/1.5);
            if(m_bloodtrail[i].type == 0){
                painter.drawPixmap(m_bloodtrail[i].m_X,m_bloodtrail[i].m_Y,m_blood[0]);
            }
            if(m_bloodtrail[i].type == 1){
                painter.drawPixmap(m_bloodtrail[i].m_X,m_bloodtrail[i].m_Y,m_blood[1]);
            }
        }
    }
    painter.setOpacity(1);
    //画Hero
    painter.drawPixmap(m_hero.m_X - m_hero.shiftx,m_hero.m_Y - m_hero.shifty,m_hero.m_Plane);
    //painter.drawRect(m_hero.m_Rect);

    //画血条
    QPainterPath path1;
    path1.addRect(m_hero.m_Rect.x()-9,m_hero.m_Rect.y()-32,(m_hero.m_Rect.width()+18)*((double)(m_hero.m_hp>=0?m_hero.m_hp:0)/MAX_HEALTH),18);
    painter.setPen(QPen(Qt::red, 1));
    painter.fillPath(path1, Qt::red);
    painter.setPen(QPen(Qt::white, 1));
    painter.drawRect(m_hero.m_Rect.x()-10,m_hero.m_Rect.y()-33,m_hero.m_Rect.width()+20,20);
    QString h_show1 = QString::number(m_hero.m_hp) + "/"+ QString::number(MAX_HEALTH);
    painter.setFont(QFont("Consolas",10,QFont::Normal));
    painter.drawText(m_hero.m_Rect.x()+m_hero.m_Rect.width()/2-(h_show1.length()/2)*12,m_hero.m_Rect.y()-17,h_show1);
    //画蓝条
    QPainterPath path2;
    path2.addRect(m_hero.m_Rect.x()-9,m_hero.m_Rect.y()-12,(m_hero.m_Rect.width()+18)*((double)(m_hero.m_charge>=0?m_hero.m_charge:0)/CHARGE_MAX),6);
    painter.setPen(QPen(Qt::red, 1));
    painter.fillPath(path2, QColor(0x2e,0xdc,0xff));
    painter.setPen(QPen(Qt::white, 1));
    painter.drawRect(m_hero.m_Rect.x()-10,m_hero.m_Rect.y()-13,m_hero.m_Rect.width()+20,8);

    //painter.drawPixmap(temp_bullet.m_X,temp_bullet.m_Y,temp_bullet.m_Bullet);
    //绘制子弹
    for(int i = 0 ;i < BULLET_NUM;i++)
    {
        //如果子弹状态为非空闲，计算发射位置
        if(!m_hero.m_bullets[i].m_Free)
        {
            painter.drawPixmap(m_hero.m_bullets[i].m_X,m_hero.m_bullets[i].m_Y,m_hero.m_bullets[i].m_Bullet);
            //painter.drawRect(m_hero.m_bullets[i].m_Rect);
        }
    }
    for(int i = 0 ; i< ENEMY_NUM;i++)
    {
        if(m_enemys[i]->m_Free == false)
        {
            m_enemys[i]->drawEnemy(painter);
            //painter.drawRect(m_enemys[i]->m_Rect);
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
    painter.setOpacity(1);
    //绘制分数
    QString a = "Killed:" + QString::number(score);
    painter.setFont(QFont("黑体",20,QFont::Bold));
    painter.drawText(750,100,a);

    //绘制生命
    QString b = "HP:" + QString::number(m_hero.m_hp);
    painter.setFont(QFont("黑体",20,QFont::Bold));
    painter.drawText(1050,100,b);



    //绘制技能图标
    QString c = m_hero.m_skill_recorder>SKILL_INTERVAL?"":(QString::number((float)((float)SKILL_INTERVAL-(float)m_hero.m_skill_recorder)/(float)GAME_RATE)+"s");
    QPainterPath path3;
    path3.addRect(GAME_WIDTH-100,GAME_HEIGHT-50,20,20)
    path2.addRect(m_hero.m_Rect.x()-9,m_hero.m_Rect.y()-12,(m_hero.m_Rect.width()+18)*((double)(m_hero.m_charge>=0?m_hero.m_charge:0)/CHARGE_MAX),6);
    painter.setFont(QFont("黑体",20,QFont::Bold));
    painter.drawText(1050,100,b);

    //绘制debug信息
    painter.setFont(QFont("黑体",8,QFont::Bold));
    for(int i = BULLET_NUM-SKILL_BULLET_NUM;i<BULLET_NUM;++i)
    {
        painter.drawText(0,12*(SKILL_BULLET_NUM+i-BULLET_NUM),QString(QString::number(i)+":"+(m_hero.m_bullets[i].m_Free?"true":"false")));
    }
}

void MainScene::playGame()
{
    //启动定时器
    m_Timer.start();

    bgsound->play();//


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
    if(event->key()==Qt::Key_E)
    {
        this->my_vector.StateofMoveKeys[7]=QString("pressed");
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
    if(event->key()==Qt::Key_E)
    {
        this->my_vector.StateofMoveKeys[7]=QString("unpressed");
    }
}

//古法碰撞检测
bool isIntersect(const QRect& a, const QRect& b){
    return (a.x()+a.width()>b.x()&&a.y()+a.height()>b.y())
           &&(a.x()<b.x()+b.width()&&a.y()<b.y()+b.height());
}

void MainScene::collisionDetection()
{

    //遍历所有非空闲的敌机
    for(int i = 0 ;i < ENEMY_NUM;i++)
    {

        if(m_enemys[i]->m_Rect.intersects(m_hero.m_Rect)&&!m_enemys[i]->m_Free){
            m_hero.m_hp-= m_enemys[i]->hp;
            m_enemys[i]->m_Free = true;
            for(int k = 0 ; k < BOMB_NUM;k++)
            {
                if(m_bombs[k].m_Free)
                {
                    //爆炸状态设置为非空闲
                    m_bombs[k].m_Free = false;
                    //更新坐标
                    m_bombs[k].m_X = m_enemys[i]->m_X;
                    m_bombs[k].m_Y = m_enemys[i]->m_Y;
                    break;
                }
            }
            for(int k=0;k<BLOOD_NUM;k++){
                if(m_bloodtrail[k].m_Free)
                {

                    m_bloodtrail[k].m_Free = false;
                    //更新坐标

                    m_bloodtrail[k].m_X = m_enemys[i]->m_X;
                    m_bloodtrail[k].m_Y = m_enemys[i]->m_Y;
                    break;
                }
            }
        }
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
            if(m_enemys[i]->m_Rect.intersects(m_hero.m_bullets[j].m_Rect)&&!m_enemys[i]->m_Free)
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
                            m_bombs[k].m_X = m_enemys[i]->m_X;
                            m_bombs[k].m_Y = m_enemys[i]->m_Y;
                            break;
                        }
                    }
                    for(int k = 0 ; k < BLOOD_NUM;k++)
                    {
                        if(m_bloodtrail[k].m_Free)
                        {
                            //爆炸状态设置为非空闲
                            m_bloodtrail[k].m_Free = false;
                            //更新坐标
                            m_bloodtrail[k].m_X = m_enemys[i]->m_X;
                            m_bloodtrail[k].m_Y = m_enemys[i]->m_Y;
                            break;
                        }
                    }
                }
            }


        }

    }
}

