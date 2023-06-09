#include "startscene.h"
#include "qicon.h"
#include "qpainter.h"

StartScene::StartScene(QWidget *parent)
    : QWidget{parent}
{
    initScene();
}
StartScene::~StartScene()
{
}
void StartScene::initScene()
{
    //加载资源。

    startsound = new QSoundEffect(this);
    startsound->setSource(QUrl::fromLocalFile(SOUND_BGM_PATH));
    startsound->setLoopCount(QSoundEffect::Infinite);
    startsound->setVolume(0.25f);


    //初始化窗口大小
    setFixedSize(START_WIDTH,START_HEIGHT);

    //设置窗口标题
    setWindowTitle(GAME_TITLE);

    setWindowIcon(QIcon( GAME_ICON));

    QPainter painter;
    m_startscene.load(START_MAP_PATH);
    m_startscene.scaled(START_WIDTH, START_HEIGHT, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    painter.drawPixmap(0,0, m_startscene);

    play_game_button = new QPushButton(this);
    play_game_button->move(100,200);
    play_game_button->setObjectName(QString::fromUtf8("123"));
    play_game_button->setText("开始游戏");
}
