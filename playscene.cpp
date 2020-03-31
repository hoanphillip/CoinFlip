#include "playscene.h"
#include <QMenuBar>
#include <QAction>
#include <QPainter>
#include "mypushbutton.h"
#include <QTimer>
#include <QLabel>
#include "mycoin.h"
#include "dataconfig.h"
#include <QPropertyAnimation>
#include <QSound>
playScene::playScene(int index)
{
    //qDebug()<<"打开了第"<<index<<"关";

    //保存本关 关卡号
    this->levelIndex = index;
    //设置大小
    this->setFixedSize(390,570);
    //设置标题
    QString titleStr = QString("第%1关").arg(index);
    this->setWindowTitle(titleStr);
    //设置标题图片
    this->setWindowIcon(QIcon(":/res/Coin0001.png"));
    //创建菜单栏
    QMenuBar * bar= menuBar();
    this->setMenuBar(bar);
    //创建开始菜单
    QMenu * startMenu = bar->addMenu("开始");
    //创建菜单项
    QAction *quitAction = startMenu->addAction("退出");
    //点击退出按钮，实现退出游戏
    connect(quitAction,&QAction::triggered,[=](){
        this->close();
    });

    //默认未胜利(关卡胜利标志)
    isWin = false;
    //返回音效创建
    QSound *backSound = new QSound (":/res/BackButtonSound.wav",this);
    //返回按钮创建
    MyPushButton *backBtn = new MyPushButton(":/res/BackButton.png",":/res/BackButtonSelected.png");
    backBtn->setParent(this);
    backBtn->move(this->width() - backBtn->width(), this->height() - backBtn->height());

    connect(backBtn,&QPushButton::clicked,[=](){
        backSound->play();
        //qDebug()<<"返回第二关卡场景";
        //延时发送返回按钮信号
         QTimer::singleShot(100,this,[=](){
             emit this->playSceneBack();
         });

    });

    //创建下一关按钮
    //判断当前是否是第一关
    //如果是第一关就只创建下一关
    if(index == 1)
    {
        MyPushButton *nextLevelBtn = new MyPushButton(":/res/nextLevelBtn.jpg");
        nextLevelBtn->setParent(this);
        nextLevelBtn->move(this->width() - 2 * nextLevelBtn->width(),this->height() - 4 * nextLevelBtn->height());

        connect(nextLevelBtn,&QPushButton::clicked,[=](){
            emit this->nextLevelSignal();
        });

    }else if(index == 20){ //如果是第二十关就只创建上一关
        MyPushButton *previousLevelBtn = new MyPushButton(":/res/previousLevelBtn.jpg");
        previousLevelBtn->setParent(this);
        previousLevelBtn->move(1 * previousLevelBtn->width(),this->height() - 4 * previousLevelBtn->height());

        connect(previousLevelBtn,&QPushButton::clicked,[=](){
            emit this->previousLevelSignal();
        });

    }else{//其余关卡上一关下一关都创建
        MyPushButton *nextLevelBtn = new MyPushButton(":/res/nextLevelBtn.jpg");
        nextLevelBtn->setParent(this);
        nextLevelBtn->move(this->width() - 2 * nextLevelBtn->width(),this->height() - 4 * nextLevelBtn->height());
        MyPushButton *previousLevelBtn = new MyPushButton(":/res/previousLevelBtn.jpg");
        previousLevelBtn->setParent(this);
        previousLevelBtn->move(previousLevelBtn->width(),this->height() - 4 * previousLevelBtn->height());

        connect(nextLevelBtn,&QPushButton::clicked,[=](){
            emit this->nextLevelSignal();
        });
        connect(previousLevelBtn,&QPushButton::clicked,[=](){
            emit this->previousLevelSignal();
        });
    }



    //左下角的具体关卡号
    QLabel *label = new QLabel;
    label->setParent(this);
    QFont font;
    font.setFamily("华文新魏");
    font.setPointSize(20);
    label->setFont(font);
    QString str = QString ("level: %1").arg(this->levelIndex);
    label->setText(str);
    label->setGeometry(QRect(30,this->height() - 50,120,50));


    //初始化游戏中的二维数组
    dataConfig data;
     for(int i=0; i<4 ;i++)
     {
         for(int j=0; j<4 ;j++)
         {
             this->gameArray[i][j] = data.mData[this->levelIndex][i][j];
         }
     }

    //创建胜利图片
     QLabel *winLabel = new QLabel;
     QPixmap pix;
     pix.load(":/res/LevelCompletedDialogBg.png");
     winLabel->setGeometry(QRect(0,0,pix.width(),pix.height()));
     winLabel->setParent(this);
     winLabel->setPixmap(pix);
     winLabel->move((this->width() - pix.width())*0.5, -pix.height());

     QSound * coinflipSound = new QSound(":/res/ConFlipSound.wav",this);
     QSound * levelWinSound = new QSound(":/res/LevelWinSound.wav");
    //创建金币背景图片
    int startX;
    startX = this->width() / 2 - 2*50;
    for(int i=0;i<4;i++)
    {
        for(int j = 0;j<4;j++)
        {
            //QLabel显示图片 金币背景
            QLabel *bg = new QLabel(this);
            bg->setGeometry(0,0,50,50);
            bg->setPixmap(QPixmap(":/res/BoardNode(1).png"));
            bg->move(startX+i*50,200+j*50);

            //创建金币
            QString str;
            if(this->gameArray[i][j] == 1)
            {
                //金币
                str = ":/res/Coin0001.png";
            }
            else{
                //银币
                str = ":/res/Coin0008.png";
            }


            MyCoin * coin = new MyCoin(str);
            coin->posX = i;//记录x位置
            coin->posY = j;//记录y位置
            coin->flag = this->gameArray[i][j];//记录硬币正反标志            
            coin->setParent(this);
            coin->move(startX+2+i*50,200+4+j*50);

            //将coin放入到维护的金币的二维数组中
            coinBtn[i][j]= coin;
            //qDebug()<<coinBtn[i][j]->flag;
            //qDebug()<<coinBtn[i][j]->isWin;

            //金币的点击事件
            connect(coin,&MyCoin::clicked,[=](){

                //播放翻转音效
                coinflipSound->play();
                qDebug()<<"点击硬币";

                //coin->changeFlag();
                coinBtn[i][j]->changeFlag();
                //同步内部维护二维数组中的数据
                gameArray[i][j] = gameArray[i][j] == 0 ? 1 : 0;

                //延时
                    /*注意，如果这里延时过短，则后续无法检测状态变化。暂时也加了个延时给判断胜利*/
                //QTimer::singleShot(10,this,[=](){

                    //继续翻动周围的硬币
                    //检测并翻下侧硬币（小于等于3）
                    if(coin->posX +1 <=3)
                    {
                        //qDebug()<<"下侧"<<coin->posX+1<<coin->posY<<coinBtn[coin->posX+1][coin->posY]->flag;
                        coinBtn[coin->posX+1][coin->posY]->changeFlag();
                        //qDebug()<<"下侧"<<coin->posX+1<<coin->posY<<coinBtn[coin->posX+1][coin->posY]->flag;
                        gameArray[coin->posX+1][coin->posY] = gameArray[coin->posX+1][coin->posY] == 0 ?1:0 ;
                    }
                    //检测翻上侧硬币
                    if(coin->posX -1 >=0)
                    {
                       // qDebug()<<"上侧"<<coin->posX-1<<coin->posY<<coinBtn[coin->posX-1][coin->posY]->flag;
                        coinBtn[coin->posX-1][coin->posY]->changeFlag();
                        //qDebug()<<"上侧"<<coin->posX-1<<coin->posY<<coinBtn[coin->posX-1][coin->posY]->flag;
                        gameArray[coin->posX-1][coin->posY] = gameArray[coin->posX-1][coin->posY] == 0 ?1:0 ;
                    }
                    //右侧硬币
                    if(coin->posY +1 <=3)
                    {
                        //qDebug()<<"右侧"<<coin->posX<<coin->posY+1<<coinBtn[coin->posX][coin->posY+1]->flag;
                        coinBtn[coin->posX][coin->posY+1]->changeFlag();
                        //qDebug()<<"右侧"<<coin->posX<<coin->posY+1<<coinBtn[coin->posX][coin->posY+1]->flag;
                        gameArray[coin->posX][coin->posY+1] = gameArray[coin->posX][coin->posY+1] == 0 ?1:0 ;
                    }
                    //左侧硬币
                    if(coin->posY -1 >=0)
                    {
                        //qDebug()<<"左侧"<<coin->posX<<coin->posY-1<<coinBtn[coin->posX][coin->posY-1]->flag;
                        coinBtn[coin->posX][coin->posY-1]->changeFlag();
                        //qDebug()<<"左侧"<<coin->posX<<coin->posY-1<<coinBtn[coin->posX][coin->posY-1]->flag;
                        gameArray[coin->posX][coin->posY-1] = gameArray[coin->posX][coin->posY-1] == 0 ?1:0 ;
                    }
                //});

                /*为了翻转效果更好我把时间设置的比较短，所以这里要给判断胜利也加一个延时*/
                //QTimer::singleShot(90,this,[=](){

                    //判断胜利
                    //在一次翻转动作结束后，判断是否全都是金币
                    this->isWin = true;
                    for(int i=0;i<4 ;i++)
                    {
                        for(int j=0;j<4 ;j++)
                        {
                            //qDebug()<<gameArray[i][j];
                            //qDebug()<<i<<j<<coinBtn[i][j]->flag;
                            //qDebug()<<i<<j<<coinBtn[i][j]->isWin;
                            if(gameArray[i][j] == 0)
                            {

                                this->isWin = false;
                                //break;

                            }
                        }
                    }

                    //胜利动作
                    if(this->isWin)
                    {
                        //将所有硬币的标志设置为true
                        for(int i=0;i<4 ;i++)
                        {
                            for(int j=0;j<4 ;j++)
                            {
                                //qDebug()<<"设置标志";
                                coinBtn[i][j]->isWin = true;

                            }
                        }

                        qDebug()<<"游戏胜利";
                        levelWinSound->play();
                        //显示胜利的图片
                        QPropertyAnimation *animation = new QPropertyAnimation(winLabel,"geometry");
                        animation->setDuration(1000);
                        animation->setStartValue(QRect(winLabel->x(),winLabel->y(),winLabel->width(),winLabel->height()));
                        animation->setEndValue(QRect(winLabel->x(),winLabel->y()+114,winLabel->width(),winLabel->height()));
                        animation->setEasingCurve(QEasingCurve::OutBounce);
                        animation->start();
                    }

                //});

            });

        }
    }

}

void playScene::paintEvent(QPaintEvent *){
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/PlayLevelSceneBg.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);

    pix.load(":/res/Title.png");
    //painter.drawPicture();
    painter.drawPixmap( this->width()/2 - pix.width()/2,30,pix);
}
