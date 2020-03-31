#include "mycoin.h"
#include <QPixmap>
#include <QMessageBox>
#include <QTimer>
#include "playscene.h"
MyCoin::MyCoin(QString CoinImg)
{
    QPixmap pix;
    bool ret = pix.load(CoinImg);
    if(!ret)
    {
        QString str = QString ("图片加载失败 %1").arg(CoinImg);
        QMessageBox::critical(this,"加载失败",str);
        return;
    }
    this->setFixedSize( pix.width(),pix.height() );
    this->setStyleSheet("QPushButton{border:0px}");
    this->setIcon(pix);
    this->setIconSize(QSize(pix.width(),pix.height()));

    timer1= new QTimer(this);
    timer2= new QTimer(this);

    //初始化是否执行动画的标志
    isAnimation = false;

    //监听定时器1 金币翻银币
    connect(timer1,&QTimer::timeout,[=](){
        QPixmap pix;
        QString str = QString(":/res/Coin000%1").arg(this->min++); //从1到8不断增加（金币翻到银币）
        pix.load(str);

        this->setFixedSize( pix.width(),pix.height() );
        this->setStyleSheet("QPushButton{border:0px}");
        this->setIcon(pix);
        this->setIconSize(QSize(pix.width(),pix.height()));
        //如果现实到最后一张，停止定时器
        if(this->min>this->max)
        {
            this->min = 1;
            this->isAnimation =false;//做完动画后，将标志设置为false
            timer1->stop();
        }
    });

    //监听定时器2,银币翻金币
    connect(timer2,&QTimer::timeout,[=](){
        QPixmap pix;
        QString str = QString(":/res/Coin000%1").arg(this->max--); //从8到1不断减小（银币翻到金币）
        pix.load(str);

        this->setFixedSize( pix.width(),pix.height() );
        this->setStyleSheet("QPushButton{border:0px}");
        this->setIcon(pix);
        this->setIconSize(QSize(pix.width(),pix.height()));
        //如果现实到最后一张，停止定时器
        if(this->max < this->min)
        {
            this->max = 8;
            this->isAnimation =false;//做完动画后，将标志设置为false
            timer2->stop();
        }
    });
}

void MyCoin::changeFlag(){

    if(this->flag)
    {
        this->flag = false;
        //同步执行动画标志
        isAnimation = true;
        //开启正面翻反面的定时器
        timer1->start(30);
        //同步标志

    }
    else //当前是反面
    {
        this->flag =true;
        //同步执行动画标志
        isAnimation = true;
        //开启反面翻正面的定时器
        timer2->start(30);

    }

}

void MyCoin::mousePressEvent(QMouseEvent *e)
{
    if(this->isAnimation || this->isWin)
    {
        //正在执行动画，不响应鼠标事件
        return;
    }
    else{
        //交给父类 做默认处理
        QPushButton::mousePressEvent(e);
    }
}
