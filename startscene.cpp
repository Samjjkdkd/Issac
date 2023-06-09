#include "startscene.h"
#include "startscene_ui.h"
#include <QMediaPlayer>


StartScene::StartScene(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StartScene)
{
    ui->setupUi(this);
}

StartScene::~StartScene()
{
    delete ui;
}
