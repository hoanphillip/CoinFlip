#ifndef MYPUSHBUTTON_H
#define MYPUSHBUTTON_H

#include <QPushButton>
#include <QString>
#include <QDebug>
#include <QPropertyAnimation>
class MyPushButton : public QPushButton
{
    Q_OBJECT
public:
    //explicit MyPushButton(QWidget *parent = 0); //不用它的构造函数


    MyPushButton(QString normalImg, QString pressImg="");
    QString normalImgPath;
    QString pressImgPath;

    //向下跳跃
    void zoomdown();
    //向上跳跃
    void zoomup();

    //鼠标按下事件
    void mousePressEvent(QMouseEvent *);
    //鼠标释放事件
    void mouseReleaseEvent(QMouseEvent *);

signals:


public slots:
};

#endif // MYPUSHBUTTON_H
