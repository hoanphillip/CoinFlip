#ifndef PLAYSCENE_H
#define PLAYSCENE_H

#include <QMainWindow>
#include <QDebug>
#include "mycoin.h"
#include <QTimer>
class playScene : public QMainWindow
{
    Q_OBJECT
public:
    explicit playScene(QWidget *parent = 0);

    playScene(int index);
    //记录本关的关卡号
    int levelIndex;

    void paintEvent(QPaintEvent *);

    //游戏二维数组维护里面的金币或银币的标志
    int gameArray[4][4];

    //金币按钮数组
    MyCoin * coinBtn[4][4];

    //胜利标志
    bool isWin;

signals:
    void playSceneBack();
    void nextLevelSignal();
    void previousLevelSignal();

public slots:
};

#endif // PLAYSCENE_H
