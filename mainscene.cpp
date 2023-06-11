#include "mainscene.h"
MainScene::MainScene(QWidget *parent)
    : QWidget(parent)
{
    //初始化场景
    initScene();
    mainLogic();
    //playGame();

}

void MainScene::mainLogic(){
    connect(this,SIGNAL(toWelcome()),this,SLOT(welCome()));
    connect(this,SIGNAL(toInGame()),this,SLOT(playGame()));
    emit toWelcome();

}

void MainScene::set_input_type(int t){
    this->input_type = t;
}

MainScene::~MainScene()
{
}
void MainScene::initScene()
{
    welcome_buttons[0] = VirtualButton(1024-520,460,300,200,"Start");
    welcome_buttons[1] = VirtualButton(1024+220,460,300,200,"Quit");

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

    //初始化窗口大小
    setFixedSize(GAME_WIDTH,GAME_HEIGHT);

    //设置窗口标题
    setWindowTitle(GAME_TITLE);

    setWindowIcon(QIcon( GAME_ICON));

    m_Timer.setInterval(GAME_RATE);

    m_enemySpawn = EventManager(ENEMY_INTERVAL);

    scene_stage = Welcome;

    m_enemy_num = ENEMY_NUM;
    m_enemy_num.setMax(ENEMY_MAX);

    //随机数种子
    srand((unsigned int)time(NULL));  //头文件  #include <ctime>

    //初始化按钮事件
    icon_skill = new InfoIcon(m_hero.m_skill,m_hero.m_charge3,SKILL_ICON_MARGIN_X,SKILL_ICON_MARGIN_Y,SKILL_ICON_SIZE,0xffffff,"E");
    icon_burst = new InfoIcon(m_hero.m_burst,m_hero.m_charge,BURST_ICON_MARGIN_X,BURST_ICON_MARGIN_Y,BURST_ICON_SIZE,0x2edcff,"Q");
    icon_ashwab = new InfoIcon(m_hero.m_ashwab,m_hero.m_charge2,ASHWAB_ICON_MARGIN_X,ASHWAB_ICON_MARGIN_Y,ASHWAB_ICON_SIZE,0xfff700,"Z");

    //初始化终结播放器
    ashwab_player = new AnimatePlayer(74,25,Z_PATH_PREFFIX,1.500f);

    //开数组
    if(m_enemys.size()<m_enemy_num.max){
        for(int i=m_enemys.size();i<m_enemy_num.max;i++){
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
    m_enemySpawn.tick(m_hero.m_ashwab.holding());
    if(!m_enemySpawn.avail())
    {
        return;
    }

    m_enemySpawn.release();

    for(int i = 0 ; i< m_enemy_num();i++)
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
    //大前提：没开终结寄
    if(!m_hero.m_ashwab.holding()){
        //发射子弹
        m_hero.shoot();

        //计算子弹
        for(int i = 0 ;i < m_hero.m_bullet_num();i++)
        {
            //如果子弹状态为非空闲，计算发射位置
            if(!m_hero.m_bullets[i].m_Free)
            {
            m_hero.m_bullets[i].updatePosition();
            }
        }

        for(int i = 0 ;i < m_hero.m_bullet_num.max;i++)
        {
            //如果子弹状态为非空闲，计算发射位置
            if(!m_hero.m_bullets2[i].m_Free)
            {
            m_hero.m_bullets2[i].updatePosition();
            }
        }

        //爆发时，不更新敌机位置
        if(!m_hero.m_burst.holding()){
            for(int i = 0 ; i< m_enemy_num.max;i++)
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

        //计算血迹更新
        for(int i=0;i<BLOOD_NUM;i++){
            if(m_bloodtrail[i].m_Free==false){
            m_bloodtrail[i].updateInfo();
            }
        }

        //计算能量图片
        for(int i=0;i<ENERGY_MAX;i++){
            if(m_energies[i].m_Free==false){
            m_energies[i].updateInfo();
            }
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

    //与终结技释放有关
    if(m_hero.m_ashwab.timer&&this->my_vector.ashwab){
        bgsound->setVolume(0.15f);
        z_sound->play();
        ashwab_player->start();
    }

    if(ashwab_player->ended){
        ashwab_player->ended = false;
        killAll();
        bgsound->setVolume(0.25f);
    }

    //应用坐标、角度变化
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

void MainScene::updatePosition4welcome()
{

    //以下内容与玩家输入动作有关
    QTransform transform;
    this->my_vector.GenerateVector();

    m_hero.sprint(my_vector.sprint);

    if(my_vector.confirm){
        switch(welcome_selected){
        case Enter_Game:
            m_Timer.stop();
            m_Timer.disconnect();
            m_hero.toInitPosotion();
            emit toInGame();
            break;
        case Quit_Game:
            m_Timer.stop();
            m_Timer.disconnect();
            qApp->quit();
            break;
        }
    }

    //应用坐标、角度变化
    int deltax = 0;
    int deltay = 0;

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

void MainScene::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    switch(scene_stage){
    case Welcome:
        paintWelcomeScene(painter);
        break;
    case InGame:
        paintInGameScene(painter);
        break;
    }

    //painter.setRenderHint(QPainter::Antialiasing);

}

void MainScene::paintWelcomeScene(QPainter &painter){
    //绘制地图
    painter.drawPixmap(0,0 , m_map.m_map_1);
    painter.setFont(QFont("黑体",40,QFont::Bold));
    painter.setPen(QPen(Qt::white, 1));
    //绘制标题
    QString a = QString("Welcome to ") + GAME_TITLE;
    painter.drawText(1024-a.length()*41/2,200,a);

    painter.setFont(QFont("黑体",30,QFont::Bold));


    QString spaceHint("Press [SPACE] to confirm your choice");
    QString adHint("Press A/D to adjust your facing direction");
    QString dirHint("Press ←→/↑↓ to move horizontally/vertically");
    QString wsHint("Press W/S to move forward/backward");
    QString sprintHint("Press [Shift] to speed up for a while");
    painter.drawText(1024-sprintHint.length()*31/2,860,sprintHint);
    painter.drawText(1024-adHint.length()*31/2,930,adHint);
    switch(input_type){
        case WASD:
            painter.drawText(1024-wsHint.length()*31/2,1000,wsHint);
            break;
        case AD_DIR:
            painter.drawText(1024-dirHint.length()*31/2,1000,dirHint);
            break;
    }
    if(welcome_selected!=-1)
        painter.drawText(1024-spaceHint.length()*31/2,1070,spaceHint);

    painter.setPen(QPen(Qt::white, 1));



    welcome_buttons[0].drawButton(painter);
    welcome_buttons[1].drawButton(painter);

    painter.drawPixmap(m_hero.m_X - m_hero.shiftx,m_hero.m_Y - m_hero.shifty,m_hero.m_Plane);

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



    painter.setOpacity(1.0);
    painter.setPen(QPen(Qt::white, 1));
}

void MainScene::paintInGameScene(QPainter &painter){
    //绘制地图
    painter.drawPixmap(0,0 , m_map.m_map_1);


    paintHostileObject(painter);

    if(m_hero.m_burst.holding()||m_hero.m_ashwab.holding()){
        if(ashwab_player->playing){
        if(ashwab_player->playing_time>0.795f)
            paintMask(painter, 0.45f);
        }
        paintMask(painter,0.35f);
    }


    paintFriendlyObject(painter);

    painter.setOpacity(1.0);
    painter.setPen(QPen(Qt::white, 1));



    //绘制大招动画
    if(ashwab_player->playing){

        ashwab_player->play(painter);
    }else{
        paintInfoComponent(painter);
    }


    paintDebug(painter);


}



void MainScene::playGame()
{
    scene_stage = InGame;

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

void MainScene::welCome()
{
    scene_stage = Welcome;
    //启动定时器
    m_Timer.start();



    //监听定时器
    connect(&m_Timer,&QTimer::timeout,[=](){
        //更新游戏中元素的坐标

        //敌机出场
        updatePosition4welcome();
        collisionDetection4welcome();

        //temp_bullet.m_Free = false;
        //temp_bullet.updatePosition();
        //重新绘制图片
        update();

    });
}

void MainScene::mouseMoveEvent(QMouseEvent *event)
{
//    int x = event->x() - m_hero.m_Rect.width()*0.5; //鼠标位置 - 飞机矩形的一半
//    int y = event->y() - m_hero.m_Rect.height()*0.5;

//    //边界检测
//    if(x <= 0 )
//    {
//        x = 0;
//    }
//    if(x >= GAME_WIDTH - m_hero.m_Rect.width())
//    {
//        x = GAME_WIDTH - m_hero.m_Rect.width();
//    }
//    if(y <= 0)
//    {
//        y = 0;
//    }
//    if(y >= GAME_HEIGHT - m_hero.m_Rect.height())
//    {
//        y = GAME_HEIGHT - m_hero.m_Rect.height();
//    }
//    m_hero.setPosition(x,y);
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
    if(event->key()==Qt::Key_Space)
    {
        this->my_vector.StateofMoveKeys[13]=QString("pressed");
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
    if(event->key()==Qt::Key_Space)
    {
        this->my_vector.StateofMoveKeys[13]=QString("unpressed");
    }
}

void MainScene::killAll(){
    QTransform blood_trans;
    int tmp_type;
    for(int i = 0 ;i < m_enemy_num.max;i++){
        if(!m_enemys[i]->m_Free){
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

                tmp_type = rand()%2;

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
        }
    }

}

void MainScene::collisionDetection()
{

    QTransform blood_trans;
    int tmp_type;


    //遍历所有非空闲的敌机
    for(int i = 0 ;i < m_enemy_num.max;i++)
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

                tmp_type = rand()%2;

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
        for(int j = 0 ; j < m_hero.m_bullet_num();j++)
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

                        tmp_type = rand()%2;

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

        for(int j = 0 ; j < m_hero.m_bullet_num.max;j++)
        {
            if(m_hero.m_bullets2[j].m_Free||m_enemys[i]->m_Free)
            {
                //空闲子弹 跳转下一次循环
                continue;
            }

            //如果子弹矩形框和敌机矩形框相交，发生碰撞，同时变为空闲状态即可
            if(m_enemys[i]->m_Rect.intersects(m_hero.m_bullets2[j].m_Rect)&&!m_enemys[i]->m_Free)
            {
                m_enemys[i]->hp--;
                m_hero.m_bullets2[j].m_Free = true;
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

                        tmp_type = rand()%2;

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

void MainScene::collisionDetection4welcome()
{

    for(int i = 0; i<2;++i){
        if(welcome_buttons[i].rect.intersects(m_hero.m_Rect)){
            welcome_buttons[i].isHanged = true;
            welcome_selected = i;
        }else{
            welcome_buttons[i].isHanged = false;
            welcome_selected = welcome_selected == i?-1:welcome_selected;
        }
    }
}

MainScene::InfoIcon::InfoIcon(EventManager &_e,TubeLikeData &_charge,int _x, int _y, int _size,unsigned _c,QString k):e(_e),d(_charge),c(_c),key_(k){
    margin_x = _x;
    margin_y = _y;
    size = _size;
    font1 = QFont("黑体",15,QFont::Bold);
    font2 = QFont("黑体",30,QFont::Bold);

}

void MainScene::InfoIcon::paint(QPainter &p){
    str_ = e.avail()?"":(QString::number(e.getCD()*(float)GAME_RATE/1000.0f,'f',1)+"s");
    path.clear();
    path.addRect(GAME_WIDTH-margin_x+1,GAME_HEIGHT-margin_y+1+(float)(size-2)*(
                                                                                                    1.0f-d.progress()),SKILL_ICON_SIZE-1,(float)(SKILL_ICON_SIZE-2)*d.progress()+1);
    p.setPen(QPen(Qt::white, 1));
    p.drawRect(GAME_WIDTH-margin_x,GAME_HEIGHT-margin_y,size,size);
    p.setOpacity(e.avail()?0.6:0.3);
    p.fillPath(path, c);
    p.setOpacity(1);
    p.setFont(QFont("黑体",15,QFont::Bold));
    p.drawText(GAME_WIDTH-margin_x+10,GAME_HEIGHT-margin_y+size-10,str_);
    p.setFont(QFont("黑体",30,QFont::Bold));
    p.drawText(GAME_WIDTH-margin_x+10,GAME_HEIGHT-margin_y+size-48,key_);

}

void MainScene::paintInfoComponent(QPainter &painter){
    //绘制分数
    QString a = "Score:" + QString::number(score);
    painter.setFont(QFont("黑体",20,QFont::Bold));
    painter.drawText(750,100,a);

    //绘制生命
    QString b = "HP:" + QString::number(m_hero.m_hp());
    painter.setFont(QFont("黑体",20,QFont::Bold));
    painter.drawText(1050,100,b);

    //绘制右下角图标
    icon_skill->paint(painter);
    icon_burst->paint(painter);
    icon_ashwab->paint(painter);

    //绘制FPS
    painter.setFont(QFont("黑体",18,QFont::Bold));
    ftime(&cTime);
    currTime = cTime.time*1000+cTime.millitm;
    painter.drawText(GAME_WIDTH-180,50,"FPS " + QString("%1").arg((int)(1000.0f/(float)(currTime-lastTime)), 3, 10, QLatin1Char(' ')));
    lastTime = currTime;
}

void MainScene::paintHostileObject(QPainter &painter){

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
    for(int i = 0 ; i< m_enemy_num.max;i++)
    {
        if(m_enemys[i]->m_Free == false)
        {
            m_enemys[i]->drawEnemy(painter);
            //painter.drawRect(m_enemys[i]->m_Rect);
        }
    }


}
void MainScene::paintFriendlyObject(QPainter &painter){


    //painter.drawPixmap(temp_bullet.m_X,temp_bullet.m_Y,temp_bullet.m_Bullet);
    //绘制子弹
    for(int i = 0 ;i < m_hero.m_bullet_num();i++)
    {
        //如果子弹状态为非空闲，计算发射位置
        if(!m_hero.m_bullets[i].m_Free)
        {
            painter.drawPixmap(m_hero.m_bullets[i].m_X,m_hero.m_bullets[i].m_Y,m_hero.m_bullets[i].m_Bullet);
            //painter.drawRect(m_hero.m_bullets[i].m_Rect);
        }
    }

    for(int i = 0 ;i < m_hero.m_bullet_num.max;i++)
    {
        //如果子弹状态为非空闲，计算发射位置
        if(!m_hero.m_bullets2[i].m_Free)
        {
            painter.drawPixmap(m_hero.m_bullets2[i].m_X,m_hero.m_bullets2[i].m_Y,m_hero.m_bullets2[i].m_Bullet);
            //painter.drawRect(m_hero.m_bullets2[i].m_Rect);
        }
    }


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
    //画Hero
    painter.drawPixmap(m_hero.m_X - m_hero.shiftx,m_hero.m_Y - m_hero.shifty,m_hero.m_Plane);
    //painter.drawRect(m_hero.m_Rect);

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


}

MainScene::AnimatePlayer::AnimatePlayer(int _num, int _rate, QString _pre, float _start, float _end) {
    frame_num = _num;
    frame_rate = _rate;
    path_preffix = _pre;
    start_time = _start;
    frame_time = 1.0f / (float)frame_rate;
    end_time = _end==100.0f?start_time+(float)frame_num / (float)frame_rate:_end;
    for(int i = 0; i<frame_num;++i){
        frames[i].load(path_preffix + QString("%1").arg(i, 2, 10, QLatin1Char('0'))+".png");
    }
}

void MainScene::AnimatePlayer::start(){
    ftime(&timeObject);
    play_time = timeObject.time*1000 + timeObject.millitm;
    playing = true;
    qDebug()<<"play_time:"<<play_time<<'\n';
}
bool MainScene::AnimatePlayer::isPlaying(){
    ftime(&timeObject);
    curr_time = timeObject.time*1000 + timeObject.millitm;
    playing_time = (float)(curr_time - play_time)/1000.0f;
    if(playing_time < end_time){
        playing = true;
    }else{
        if(playing == true) ended = true;
        playing = false;

    }
    return playing;
}

void MainScene::AnimatePlayer::play(QPainter &painter){
    if(!isPlaying())    return;

    if(playing_time>=start_time&&playing_time<end_time){
        animate_time = playing_time - start_time;
        painter.drawPixmap(0,0,frames[(int)(animate_time*(float)frame_rate)]);
    }
}

void MainScene::paintDebug(QPainter &painter){
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

}

void MainScene::paintMask(QPainter &painter, float trans){
    painter.setBrush(QBrush(QColor(0,0,0,(int)(255.0f*trans))));
    painter.drawRect(-1,-1,GAME_WIDTH+2,GAME_HEIGHT+2);
    painter.setBrush(QBrush(Qt::NoBrush));

}


