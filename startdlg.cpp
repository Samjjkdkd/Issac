#include "startdlg.h"
#include "ui_startdlg.h"
#include <QMediaPlayer>


StartDlg::StartDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StartDlg)
{
    ui->setupUi(this);
}

StartDlg::~StartDlg()
{
    delete ui;
}
