/********************************************************************************
** Form generated from reading UI file 'StartScene.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef STARTSCENE_UI_H
#define STARTSCENE_UI_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QSoundEffect>
#include "config.h"


QT_BEGIN_NAMESPACE

class Ui_StartScene{
public:
    QPushButton *pushButton;
    QLabel *label;
    QSoundEffect* start_music;
    void setupUi(QDialog *StartScene)
    {
        if (StartScene->objectName().isEmpty())
            StartScene->setObjectName(QString::fromUtf8("StartScene"));
        StartScene->resize(1024, 479);
        pushButton = new QPushButton(StartScene);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(330, 180, 361, 140));
        QFont font;
        font.setFamily(QString::fromUtf8("Microsoft YaHei"));
        font.setPointSize(20);
        pushButton->setFont(font);
        label = new QLabel(StartScene);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(0, 0, 1024, 479));
        label->setPixmap(QPixmap(QString::fromUtf8(":/res/startbg.jpg")));
        label->raise();
        pushButton->raise();

        start_music = new QSoundEffect(StartScene);
        start_music->setSource(QUrl::fromLocalFile(START_BGM_PATH));
        start_music->setLoopCount(QSoundEffect::Infinite);
        start_music->setVolume(0.9f);
        start_music->play();


        retranslateUi(StartScene);
        QObject::connect(pushButton, SIGNAL(clicked()), StartScene, SLOT(accept()));

        QMetaObject::connectSlotsByName(StartScene);
    } // setupUi

    void retranslateUi(QDialog *StartScene)
    {
        StartScene->setWindowTitle(QCoreApplication::translate("StartScene", "Start Issac", nullptr));
        pushButton->setText(QCoreApplication::translate("StartScene", "\345\274\200\345\247\213\346\270\270\346\210\217", nullptr));
        label->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class StartScene: public Ui_StartScene {};
} // namespace Ui

QT_END_NAMESPACE

#endif // STARTSCENE_UI_H
