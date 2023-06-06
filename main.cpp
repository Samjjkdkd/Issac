#include "mainscene.h"
#include "startdlg.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    StartDlg s;
    if(s.exec() == QDialog::Accepted) // 利用Accepted返回值判断按钮是否被按下
    {
        MainScene w;
        w.show();                      // 如果被按下，显示主窗口
        return a.exec();              // 程序一直执行，直到主窗口关闭
    }
    else{
        return 0;
    }
}
