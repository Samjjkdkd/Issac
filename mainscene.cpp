#include "mainscene.h"
MainScene::MainScene(QWidget *parent)
    : QWidget(parent)
{
    //初始化场景
    initScene();

    playGame();

}

void MainScene::set_input_type(int t){
    this->input_type = t;
}

MainScene::~MainScene()
{
}
void MainScene::initScene()
{
    //加载资源。
    z_sound = new QSoundEffect(this);
    z_sound->setSource(QUrl::fromLocalFile(Z_SOUND_PATH));
    z_sound->setLoopCount(1);
    z_sound->setVolume(1.0f);
    bgsound = new QSoundEffect(this);
    bgsound->setSource(QUrl::fromLocalFile(SOUND_BGM_PATH));
    bgsound->setLoopCount(QSoundEffect::Infinite);
    bgsound->setVolume(0.5f);

    m_blood[0].load(BLOOD_PATH_1);
    m_blood[0] = m_blood[0].scaled(RESIZE_BLOOD_WIDTH,RESIZE_BLOOD_HEIGHT, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    m_blood[1].load(BLOOD_PATH_2);
    m_blood[1] = m_blood[1].scaled(RESIZE_BLOOD_WIDTH,RESIZE_BLOOD_HEIGHT, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    for(int i = 0; i<76;i++){
        m_ashwab[i].load(Z_PATH_PREFFIX + QString("%1").arg(i, 2, 10, QLatin1Char('0'))+".png");
        //m_ashwab[i] = m_ashwab[i].scaled(GAME_WIDTH,GAME_HEIGHT,Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    }

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

    ftime(&cTime);
    lastTime = cTime.time*1000+cTime.millitm;
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
    if(!m_hero.m_ashwab.holding()){
        for(int i = 0 ;i < BULLET_NUM;i++)
        {
            //如果子弹状态为非空闲，计算发射位置
            if(!m_hero.m_bullets[i].m_Free)
            {
            m_hero.m_bullets[i].updatePosition();
            }
        }
    }
    //敌机坐标计算
    if(!(m_hero.m_burst.holding()||m_hero.m_ashwab.holding())){
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
    for(int i=0;i<ENERGY_MAX;i++){
        if(m_energies[i].m_Free==false){
            m_energies[i].updateInfo();
        }
    }

    //以下内容与玩家输入动作有关
    QTransform transform;
    this->my_vector.GenerateVector();

    //战技
    m_hero.skill(this->my_vector.skill);

    //爆发
    m_hero.burst(this->my_vector.burst);

    //冲刺
    m_hero.sprint(this->my_vector.sprint);

    //终结
    m_hero.ashwab(this->my_vector.ashwab);

    //作弊
    if(this->my_vector.cheat)   m_hero.cheat();

    if(m_hero.m_ashwab.timer&&this->my_vector.ashwab){
        bgsound->setVolume(0.15f);
        z_sound->play();
    }

    if(m_hero.m_ashwab.timer == 20){
        bgsound->setVolume(0.25f);
    }
    int deltax = 0;
    int deltay = 0;
    if(!m_hero.m_ashwab.holding()){
        if(input_type == WASD){
            if(m_hero.m_sprint.holding()&&!this->my_vector.Vf){
                this->my_vector.Vf = 1.0;
            }
            deltax += this->my_vector.Vf*qCos((float)(90-m_hero.b_direction)*Pi/180.0)*this->m_hero.m_speed();
            deltay += this->my_vector.Vf*qSin((float)(90-m_hero.b_direction)*Pi/180.0)*this->m_hero.m_speed();
            if(this->my_vector.Vf<0.0f){
                this->my_vector.theta*=-1;
            }
        }else if(input_type == AD_DIR)
        {
            deltax += (this->my_vector.Vx)*this->m_hero.m_speed();
            deltay += (this->my_vector.Vy)*this->m_hero.m_speed();
        }

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
}

void MainScene::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    //painter.setRenderHint(QPainter::Antialiasing);
    //绘制地图
    painter.drawPixmap(0,0 , m_map.m_map_1);


    //绘制血迹

    for(int i = 0 ; i < BLOOD_NUM;i++)
    {
        if(m_bloodtrail[i].m_Free == false)
        {
            painter.setOpacity(m_bloodtrail[i].m_transparentrate/1.5);

            painter.drawPixmap(m_bloodtrail[i].m_X,m_bloodtrail[i].m_Y,m_bloodtrail[i].m_bloodtrail);

        }
    }
    painter.setOpacity(1);

    //绘制爆炸图片
    for(int i = 0 ; i < BOMB_NUM;i++)
    {
        if(m_bombs[i].m_Free == false)
        {
            painter.drawPixmap(m_bombs[i].m_X,m_bombs[i].m_Y,m_bombs[i].m_pixArr[m_bombs[i].m_index]);
        }
    }

    //画敌人
    for(int i = 0 ; i< ENEMY_NUM;i++)
    {
        if(m_enemys[i]->m_Free == false)
        {
            m_enemys[i]->drawEnemy(painter);
            //painter.drawRect(m_enemys[i]->m_Rect);
        }
    }

    if(m_hero.m_burst.holding()||m_hero.m_ashwab.holding()){
        painter.setBrush(QBrush(QColor(0,0,0,80)));
        painter.drawRect(-1,-1,GAME_WIDTH+2,GAME_HEIGHT+2);
        painter.setBrush(QBrush(Qt::NoBrush));
    }


    painter.setOpacity(1);


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

    //画Hero
    painter.drawPixmap(m_hero.m_X - m_hero.shiftx,m_hero.m_Y - m_hero.shifty,m_hero.m_Plane);
    //painter.drawRect(m_hero.m_Rect);

    //绘制能量球
    for(int i = 0 ;i < ENERGY_MAX;i++)
    {
        //如果子弹状态为非空闲，计算发射位置
        if(!m_energies[i].m_Free)
        {
            painter.drawPixmap(m_energies[i].m_X,m_energies[i].m_Y,m_energies[i].m_energy);
            //painter.drawRect(m_energies[i].m_Rect);
        }
    }
    painter.setOpacity(1.0);
    painter.setPen(QPen(Qt::white, 1));

    //绘制分数
    QString a = "Score:" + QString::number(score);
    painter.setFont(QFont("黑体",20,QFont::Bold));
    painter.drawText(750,100,a);

    //绘制生命
    QString b = "HP:" + QString::number(m_hero.m_hp());
    painter.setFont(QFont("黑体",20,QFont::Bold));
    painter.drawText(1050,100,b);

    //画血条
    QPainterPath path1;
    path1.addRect(m_hero.m_Rect.x()-9,m_hero.m_Rect.y()-32,(m_hero.m_Rect.width()+19)*((float)(m_hero.m_hp()>=0?m_hero.m_hp():0)/(float)m_hero.m_hp.max()),19);
    painter.setPen(QPen(Qt::red, 1));
    painter.fillPath(path1, Qt::red);
    painter.setPen(QPen(Qt::white, 1));
    painter.drawRect(m_hero.m_Rect.x()-10,m_hero.m_Rect.y()-33,m_hero.m_Rect.width()+20,20);
    QString h_show1 = QString::number(m_hero.m_hp()) + "/"+ QString::number(m_hero.m_hp.max());
    painter.setFont(QFont("Consolas",10,QFont::Normal));
    painter.drawText(m_hero.m_Rect.x()+m_hero.m_Rect.width()/2-(h_show1.length()/2)*12,m_hero.m_Rect.y()-17,h_show1);


    //画体力
    QPainterPath path2;
    path2.addRect(m_hero.m_Rect.x()-9,m_hero.m_Rect.y()-12,(m_hero.m_Rect.width()+19)*((float)(m_hero.m_stamina())/(float)m_hero.m_stamina.max()),7);
    painter.setPen(QPen(Qt::red, 1));
    painter.fillPath(path2, QColor(0xaaaaaa));
    painter.setPen(QPen(Qt::white, 1));
    painter.drawRect(m_hero.m_Rect.x()-10,m_hero.m_Rect.y()-13,m_hero.m_Rect.width()+20,8);


    //绘制技能图标
    QString e = m_hero.m_skill.avail()?"":(QString::number(m_hero.m_skill.getCD()*(float)GAME_RATE/1000.0f,'f',1)+"s");
    QPainterPath path3;
    path3.addRect(GAME_WIDTH-SKILL_ICON_MARGIN_X+1,GAME_HEIGHT-SKILL_ICON_MARGIN_Y+1+(float)(SKILL_ICON_SIZE-2)*(
                                                                                                    1.0f-m_hero.m_skill.progress()),SKILL_ICON_SIZE-1,(float)(SKILL_ICON_SIZE-2)*m_hero.m_skill.progress()+1);
    painter.setPen(QPen(Qt::white, 1));
    painter.drawRect(GAME_WIDTH-SKILL_ICON_MARGIN_X,GAME_HEIGHT-SKILL_ICON_MARGIN_Y,SKILL_ICON_SIZE,SKILL_ICON_SIZE);
    painter.setOpacity(m_hero.m_skill.avail()?0.6:0.3);
    painter.fillPath(path3, QColor(0xff,0xff,0xff));
    painter.setOpacity(1);
    painter.setFont(QFont("黑体",15,QFont::Bold));
    painter.drawText(GAME_WIDTH-SKILL_ICON_MARGIN_X+10,GAME_HEIGHT-SKILL_ICON_MARGIN_Y+SKILL_ICON_SIZE-10,e);
    painter.setFont(QFont("黑体",30,QFont::Bold));
    painter.drawText(GAME_WIDTH-SKILL_ICON_MARGIN_X+10,GAME_HEIGHT-SKILL_ICON_MARGIN_Y+SKILL_ICON_SIZE-48,QString("E"));

    //绘制元素爆发图标
    QString q = m_hero.m_burst.avail()?"":(QString::number(m_hero.m_burst.getCD()*(float)GAME_RATE/1000.0f,'f',1)+"s");

    QPainterPath path4;
    path4.addRect(GAME_WIDTH-BURST_ICON_MARGIN_X+1,GAME_HEIGHT-BURST_ICON_MARGIN_Y+1+(float)(BURST_ICON_SIZE-2)*(1.0f-m_hero.m_charge.progress()),SKILL_ICON_SIZE-1,(float)(SKILL_ICON_SIZE-2)*(m_hero.m_charge.progress())+1);
    painter.setPen(QPen(Qt::white, 1));
    painter.drawRect(GAME_WIDTH-BURST_ICON_MARGIN_X,GAME_HEIGHT-BURST_ICON_MARGIN_Y,BURST_ICON_SIZE,BURST_ICON_SIZE);
    painter.setOpacity(m_hero.m_charge.full()?0.6:0.4);
    painter.fillPath(path4, QColor(0x2e,0xdc,0xff));
    painter.setOpacity(1);
    painter.setFont(QFont("黑体",15,QFont::Bold));
    painter.drawText(GAME_WIDTH-BURST_ICON_MARGIN_X+10,GAME_HEIGHT-BURST_ICON_MARGIN_Y+BURST_ICON_SIZE-10,q);
    painter.setFont(QFont("黑体",30,QFont::Bold));
    painter.drawText(GAME_WIDTH-BURST_ICON_MARGIN_X+10,GAME_HEIGHT-BURST_ICON_MARGIN_Y+BURST_ICON_SIZE-48,QString("Q"));

    //绘制终结图标
    QString z = m_hero.m_ashwab.avail()?"":(QString::number(m_hero.m_ashwab.getCD()*(float)GAME_RATE/1000.0f,'f',1)+"s");

    QPainterPath path5;
    path5.addRect(GAME_WIDTH-ASHWAB_ICON_MARGIN_X+1,GAME_HEIGHT-ASHWAB_ICON_MARGIN_Y+1+(float)(ASHWAB_ICON_SIZE-2)*(1.0f- m_hero.m_charge2.progress()),ASHWAB_ICON_SIZE-1,(float)(ASHWAB_ICON_SIZE-2)*m_hero.m_charge2.progress()+1);
    painter.setPen(QPen(Qt::white, 1));
    painter.drawRect(GAME_WIDTH-ASHWAB_ICON_MARGIN_X,GAME_HEIGHT-ASHWAB_ICON_MARGIN_Y,ASHWAB_ICON_SIZE,ASHWAB_ICON_SIZE);
    painter.setOpacity(m_hero.m_charge2.full()?0.6:0.4);
    painter.fillPath(path5, QColor(0xfff700));
    painter.setOpacity(1);
    painter.setFont(QFont("黑体",15,QFont::Bold));
    painter.drawText(GAME_WIDTH-ASHWAB_ICON_MARGIN_X+10,GAME_HEIGHT-ASHWAB_ICON_MARGIN_Y+ASHWAB_ICON_SIZE-10,z);
    painter.setFont(QFont("黑体",30,QFont::Bold));
    painter.drawText(GAME_WIDTH-ASHWAB_ICON_MARGIN_X+10,GAME_HEIGHT-ASHWAB_ICON_MARGIN_Y+ASHWAB_ICON_SIZE-48,QString("Z"));


    //绘制大招动画
    if(m_hero.m_ashwab.timer){
        float curr_time = (float)(ASHWAB_TIME-m_hero.m_ashwab.timer)*(float)GAME_RATE/1000.0f-ASHWAB_OFFSET;
        if(curr_time>=1.60f&&curr_time<4.56f){
            painter.drawPixmap(0,0,m_ashwab[(int)((curr_time-1.60f)*25.0f)]);
        }


    }


    //绘制debug信息
    painter.setFont(QFont("黑体",8,QFont::Bold));
//    for(int i = BULLET_NUM-SKILL_BULLET_NUM;i<BULLET_NUM;++i)
//    {
//        painter.drawText(0,12*(SKILL_BULLET_NUM+i-BULLET_NUM),QString(QString::number(i)+":"+(m_hero.m_bullets[i].m_Free?"true":"false")));
//    }
    //    for(int i = 0;i<ENERGY_MAX;++i)
    //    {
    //        painter.drawText(0,12*(i),QString(QString::number(i)+":"+(m_energies[i].m_Free?"true":"false")));
    //    }
//    for(int i = 0;i<ENEMY_NUM;++i)
//    {
//        painter.drawText(0,12*(i),QString(QString::number(i)+":"+(m_enemys[i]->m_Free?"true":"false")));
//    }


    //绘制FPS
    painter.setFont(QFont("黑体",18,QFont::Bold));
    ftime(&cTime);
    currTime = cTime.time*1000+cTime.millitm;
    painter.drawText(GAME_WIDTH-180,50,"FPS " + QString("%1").arg((int)(1000.0f/(float)(currTime-lastTime)), 3, 10, QLatin1Char(' ')));
    lastTime = currTime;

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
    if(event->key()==Qt::Key_Shift)
    {
        this->my_vector.StateofMoveKeys[4]=QString("pressed");
    }
    if(event->key()==Qt::Key_E)
    {
        this->my_vector.StateofMoveKeys[7]=QString("pressed");
    }
    if(event->key()==Qt::Key_Q)
    {
        this->my_vector.StateofMoveKeys[8]=QString("pressed");
    }
    if(event->key()==Qt::Key_Z)
    {
        this->my_vector.StateofMoveKeys[9]=QString("pressed");
    }
    if(event->key()==Qt::Key_W)
    {
        this->my_vector.StateofMoveKeys[10]=QString("pressed");
    }
    if(event->key()==Qt::Key_S)
    {
        this->my_vector.StateofMoveKeys[11]=QString("pressed");
    }
    if(event->key()==Qt::Key_G)
    {
        this->my_vector.StateofMoveKeys[12]=QString("pressed");
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
    if(event->key()==Qt::Key_Shift)
    {
        this->my_vector.StateofMoveKeys[4]=QString("unpressed");
    }
    if(event->key()==Qt::Key_E)
    {
        this->my_vector.StateofMoveKeys[7]=QString("unpressed");
    }
    if(event->key()==Qt::Key_Q)
    {
        this->my_vector.StateofMoveKeys[8]=QString("unpressed");
    }
    if(event->key()==Qt::Key_Z)
    {
        this->my_vector.StateofMoveKeys[9]=QString("unpressed");
    }
    if(event->key()==Qt::Key_W)
    {
        this->my_vector.StateofMoveKeys[10]=QString("unpressed");
    }
    if(event->key()==Qt::Key_S)
    {
        this->my_vector.StateofMoveKeys[11]=QString("unpressed");
    }
    if(event->key()==Qt::Key_G)
    {
        this->my_vector.StateofMoveKeys[12]=QString("unpressed");
    }
}

//古法碰撞检测
bool isIntersect(const QRect& a, const QRect& b){
    return (a.x()+a.width()>b.x()&&a.y()+a.height()>b.y())
           &&(a.x()<b.x()+b.width()&&a.y()<b.y()+b.height());
}

void MainScene::collisionDetection()
{

    QTransform blood_trans;
    //遍历所有非空闲的敌机
    for(int i = 0 ;i < ENEMY_NUM;i++)
    {
        if(m_hero.m_ashwab.timer == (int)((ASHWAB_OFFSET+0.20f)*1000.0f/(float)GAME_RATE) &&!m_enemys[i]->m_Free){
            m_enemys[i]->hp=0;
            m_enemys[i]->m_Free = true;
            //得分增加
            switch(m_enemys[i]->type){
            case 1:
                score+=ENEMY_SCORE_1;
                break;
            case 2:
                score+=ENEMY_SCORE_2;
                break;
            case 3:
                score+=ENEMY_SCORE_3;
                break;
            }
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

                    //给予方向
                    m_bloodtrail[k].m_direction = rand()%360;

                    //更新坐标
                    m_bloodtrail[k].m_X = m_enemys[i]->m_X;
                    m_bloodtrail[k].m_Y = m_enemys[i]->m_Y;
                    break;
                }
            }
            continue;
        }
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

                    int tmp_type = rand()%2;

                    m_bloodtrail[k].m_direction = rand()%360;
                    m_bloodtrail[k].type = tmp_type;
                    blood_trans.reset();
                    blood_trans.rotate(m_bloodtrail[k].m_direction);
                    m_bloodtrail[k].m_bloodtrail = m_blood[m_bloodtrail[k].type].transformed(blood_trans,Qt::SmoothTransformation);

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

                    //得分增加
                    switch(m_enemys[i]->type){
                    case 1:
                        score+=ENEMY_SCORE_1;
                        break;
                    case 2:
                        score+=ENEMY_SCORE_2;
                        break;
                    case 3:
                        score+=ENEMY_SCORE_3;
                        break;
                    }


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

                            int tmp_type = rand()%2;

                            m_bloodtrail[k].m_direction = rand()%360;
                            m_bloodtrail[k].type = tmp_type;
                            blood_trans.reset();
                            blood_trans.rotate(m_bloodtrail[k].m_direction);
                            m_bloodtrail[k].m_bloodtrail = m_blood[m_bloodtrail[k].type].transformed(blood_trans,Qt::SmoothTransformation);
                            //更新坐标

                            m_bloodtrail[k].m_X = m_enemys[i]->m_X;
                            m_bloodtrail[k].m_Y = m_enemys[i]->m_Y;
                            break;
                        }
                    }

                    //生产能量球
                    if(rand()%100<(int)((float)ENERGY_POSSIBILITY*100.0f))
                    {
                        for(int k = 0; k<ENERGY_MAX;k++){
                            if(m_energies[k].m_Free)
                            {
                                m_energies[k].m_Free = false;
                                switch(m_enemys[i]->type){
                                case 1:
                                    m_energies[k].m_energy_amount = ENEMY_ENERGY_1;
                                    break;
                                case 2:
                                    m_energies[k].m_energy_amount = ENEMY_ENERGY_2;
                                    break;
                                case 3:
                                    m_energies[k].m_energy_amount = ENEMY_ENERGY_3;
                                    break;
                                }


                                m_energies[k].m_X = m_enemys[i]->m_X;
                                m_energies[k].m_Y = m_enemys[i]->m_Y;
                                m_energies[k].m_Rect.moveTo(m_energies[k].m_X,m_energies[k].m_Y);
                                m_energies[k].m_recorder = ENERGY_LASTING;
                                break;
                            }
                        }
                    }
                }
            }


        }

    }

    //遍历所有能量
    for(int i = 0; i< ENERGY_MAX;++i)
    {
        if((!m_energies[i].m_Free)&&m_energies[i].m_Rect.intersects(m_hero.m_Rect)){
            m_hero.m_charge+=m_energies[i].m_energy_amount;
            m_energies[i].m_Free = true;
        }
    }
}

