#ifndef STARTDLG_H
#define STARTDLG_H
#include <QDialog>

namespace Ui {
class StartDlg;
}

class StartDlg : public QDialog
{
    Q_OBJECT

public:
    explicit StartDlg(QWidget *parent = nullptr);
    ~StartDlg();

private:
    Ui::StartDlg *ui;
};

#endif // STARTDLG_H
