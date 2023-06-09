#ifndef STARTSCENE_H
#define STARTSCENE_H

#include "startscene_ui.h"

#include <QSoundEffect>
#include <QDialog>
#include <QWidget>
#include <QPushButton>

namespace Ui {
    class StartScene;
}

class StartScene : public QDialog
{
    Q_OBJECT

public:
    explicit StartScene(QWidget *parent = nullptr);
    ~StartScene();
    Ui::StartScene *ui;
};

#endif // STARTSCENE_H
