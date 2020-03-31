#ifndef MYCOIN_H
#define MYCOIN_H

#include <QPushButton>

class MyCoin : public QPushButton
{
    Q_OBJECT
public:
    //explicit MyCoin(QWidget *parent = 0);

    //参数，显示默认是金币还是银币
    MyCoin(QString CoinImg);

    //单个金币的属性
    int posX;//X坐标
    int posY;//Y坐标
    bool flag;//正反标志

    void changeFlag();//改编标志，执行翻转效果
    QTimer *timer1;//正面翻反面定时器
    QTimer *timer2;//反面翻正面定时器
    int min=1;//最小图片
    int max=8;//最大图片

    //创建是否正在执行动画的标志
    bool isAnimation;

    //鼠标按下的事件重写
    void mousePressEvent(QMouseEvent *);

    bool isWin = false;
    //创建按钮的数组


signals:

public slots:
};

#endif // MYCOIN_H
