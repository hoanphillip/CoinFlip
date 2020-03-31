#include "chooselevelscene.h"
#include <QMenuBar>
#include <QPainter>
#include "mypushbutton.h"
#include <QPushButton>
#include <QTimer>
#include <QLabel>
#include <QSound>

//hoanc20200331
ChooseLevelScene::ChooseLevelScene(QWidget *parent) : QMainWindow(parent)
{
    int i;
    //设置大小
    this->setFixedSize(390,570);
    //设置标题
    this->setWindowTitle("选择关卡");
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
    //创建返回按钮音效
    QSound *backSound = new QSound (":/res/BackButtonSound.wav",this);
    //返回按钮创建
    MyPushButton *backBtn = new MyPushButton(":/res/BackButton.png",":/res/BackButtonSelected.png");
    backBtn->setParent(this);
    backBtn->move(this->width() - backBtn->width(), this->height() - backBtn->height());
    connect(backBtn,&QPushButton::clicked,[=](){
        //播放返回音效
        backSound->play();

        //延时发送返回按钮信号
         QTimer::singleShot(100,this,[=](){
             emit this->chooseSceneBack();
         });

    });
    QSound *chooseSound = new QSound (":/res/TapButtonSound.wav",this);

    for(i = 0;i<20;i++)
    {
        //进入到具体游戏场景
        playscene[i] = new playScene(i+1);
        //监听第三个游戏场景中的返回信号
         connect(playscene[i],&playScene::playSceneBack,[=](){
             qDebug()<<"收到返回信号";
             //自身的位置同步为选择关卡场景的位置
             this->setGeometry(playscene[i]->geometry());
            //游玩关卡场景隐藏
             playscene[i]->close();
             //playscene[i] = NULL;
             //自身显示（选择关卡）
             this->show();
         });

    }

    //选择关卡按钮
    for(i = 0;i<20;i++)
    {
        MyPushButton *levelBtn = new MyPushButton(":/res/LevelIcon.png");
        levelBtn->setParent(this);
        levelBtn->move(60+ (i%4)*70, 130+ (i/4)*80);

        connect(levelBtn,&MyPushButton::clicked,[=](){
            QString str = QString("您选择的是 %1 关").arg(i+1);
            qDebug()<<str;
            //播放选择按钮音效
            chooseSound->play();
            //隐藏自身
            this->hide();

            //进入到具体游戏场景
            //让下一个场景的位置与当前场景相同
            playscene[i]->setGeometry(this->geometry());
            playscene[i]->show();
            //qDebug()<<playscene[i];


        });
        //创建关卡号Label
        QLabel *label = new QLabel(this);
        //设置标签大小
        label->setFixedSize(levelBtn->width(),levelBtn->height());
        QFont font;
        font.setFamily("华文新魏");
        font.setPointSize(16);
        label->setFont(font);
        //设置显示内容
        label->setText(QString::number(i+1));
        label->move(60+ (i%4)*70, 130+ (i/4)*80);
        label->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter);//居中
        //设置鼠标穿透
        label->setAttribute(Qt::WA_TransparentForMouseEvents);//51号属性，鼠标穿透


    }

    for(int i = 0;i<20;i++){
        // 监听上一关与下一关信号
        connect(playscene[i],&playScene::nextLevelSignal,[=](){
            qDebug()<<"点击下一关";
            qDebug()<<QString("显示第%1关").arg(i+1);

            playscene[i]->close();
            playscene[i+1]->setGeometry(this->geometry());
            playscene[i+1]->show();
            });


        connect(playscene[i],&playScene::previousLevelSignal,[=](){
            qDebug()<<"点击上一关";
            playscene[i]->close();
            playscene[i-1]->setGeometry(this->geometry());
            playscene[i-1]->show();
        });
        //监听鼠标点击关卡按钮事件end
    }

}

//重写绘图事件
void ChooseLevelScene::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    //画背景图片
    QPixmap pix;
    pix.load(":/res/OtherSceneBg.png");
    painter.drawPixmap(0,0,pix);

    //画标题图片
    pix.load(":/res/Title.png");
    //pix = pix.scaled(pix.width()*0.5,pix.height()*0.5);//太大需要缩放，缩小
    painter.drawPixmap( this->width()/2 - pix.width()/2,30,pix);
}
