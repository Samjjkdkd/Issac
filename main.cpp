#include "mainscene.h"
#include "startscene.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    StartScene s;

    auto _input_type = s.exec();
    s.ui->start_music->stop();
    MainScene w;
    if(_input_type == StartScene::Accepted){
        w.set_input_type(AD_DIR);
        w.show();                      // 如果被按下，显示主窗口

    }
    else if(_input_type == StartScene::Rejected) {
        w.set_input_type(DIR_only);
        w.show();                      // 如果被按下，显示主窗口

    }

    return a.exec();              // 程序一直执行，直到主窗口关闭

}
