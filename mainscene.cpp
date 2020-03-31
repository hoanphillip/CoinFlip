#include "mainscene.h"
#include "ui_mainscene.h"
#include <QPainter>
#include "mypushbutton.h"
#include <QTimer>
#include "chooselevelscene.h"
#include <QSound>

MainScene::MainScene(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainScene)
{
    ui->setupUi(this);

    //点击开始 - 退出： 退出游戏
    connect(ui->actionQuit,&QAction::triggered,[=](){
        this->close();
    });
    //设置大小
    this->setFixedSize(390,570);
    //设置标题
    this->setWindowTitle("翻金币游戏");
    //设置标题图片
    this->setWindowIcon(QIcon(":/res/Coin0001.png"));

    //设置背景图片

    MyPushButton *startBtn = new MyPushButton(":/res/MenuSceneStartButton.png");
    //设置父类
    startBtn->setParent(this);
    startBtn->move(this->width()*0.5 - startBtn->width()*0.5 , this->height() *0.7 );

    //准备开始按钮的音效
    QSound *startSound = new QSound (":/res/TapButtonSound.wav",this);
    //startSound-> setLoops(-1); //-1 是无限

    //创建出第二个选择关卡场景
    chooseScene = new ChooseLevelScene;
    //监听第二个选择关卡场景中的返回信号
    connect(chooseScene,&ChooseLevelScene::chooseSceneBack,[=](){
       //选择关卡场景隐藏
        chooseScene->hide();

        //自身的位置同步为选择关卡场景的位置
        this->setGeometry(chooseScene->geometry());
        //自身显示
        this->show();
    });

    connect(startBtn,&QPushButton::clicked,[=](){
        //qDebug()<<"start";
        //播放开始音效
        startSound->play();


        startBtn->zoomdown();
        startBtn->zoomup();

        //延时进入第二个场景（为了把动画显示出来）
        QTimer::singleShot(150,this,[=](){ //在150ms内执行以下动作
            //让下一个场景的位置与当前场景相同
            chooseScene->setGeometry(this->geometry());
            //将自身主场景隐藏
            this->hide();
            //进入到第二个关卡中
            chooseScene->show();
        });

    });

}

//使用绘图事件来绘制背景图
void MainScene::paintEvent(QPaintEvent *){
    QPainter painter(this);
    //画背景图片
    QPixmap pix;
    pix.load(":/res/PlayLevelSceneBg.png");
    painter.drawPixmap(0,0,pix);

    //画标题图片
    pix.load(":/res/Title.png");
    pix = pix.scaled(pix.width()*0.5,pix.height()*0.5);//太大需要缩放，缩小
    painter.drawPixmap(10,30,pix);


}


MainScene::~MainScene()
{
    delete ui;
}
