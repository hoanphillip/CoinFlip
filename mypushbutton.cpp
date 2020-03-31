#include "mypushbutton.h"

//MyPushButton::MyPushButton(QWidget *parent) : QPushButton(parent)
//{

//}

MyPushButton::MyPushButton(QString normalImg, QString pressImg){
    //保存正常显示路径和 选中后显示的路径
    this->normalImgPath = normalImg;
    this->pressImgPath = pressImg;

    QPixmap pix;
    if(! pix.load(this->normalImgPath))
    {
        QString str = QString("%1 图片加载失败").arg(this->normalImgPath);
        qDebug()<<str;
        return;
    }
    //以下4个是必填的！
    //设定图片区域的大小
    this->setFixedSize(pix.width(),pix.height());

    //设置不规则图片样式
    this->setStyleSheet("QPushButton{border:0px;}");//这里是CSS语言

    //设置图标
    this->setIcon(pix);

    //设置图片的大小
    this->setIconSize(QSize(pix.width(),pix.height()));
}

//向下跳跃
void MyPushButton::zoomdown(){
    QPropertyAnimation * animation = new QPropertyAnimation(this,"geometry");//应用矩形的方法？
    //设定时间间隔
    animation->setDuration(200);
    //设置动态对象起始位置
    animation->setStartValue(QRect(this->x(),this->y(),this->width(),this->height()));
    //设置动画对象结束位置
    animation->setEndValue(QRect(this->x(),this->y()+10,this->width(),this->height()));
    //设置缓和曲线
    animation->setEasingCurve(QEasingCurve::OutBounce);
    //执行动画
    animation->start();
}
//向上跳跃
void MyPushButton::zoomup(){
    QPropertyAnimation * animation = new QPropertyAnimation(this,"geometry");//应用矩形的方法？
    //设定时间间隔
    animation->setDuration(200);
    //设置动态对象起始位置
    animation->setStartValue(QRect(this->x(),this->y()+10,this->width(),this->height()));
    //设置动画对象结束位置
    animation->setEndValue(QRect(this->x(),this->y(),this->width(),this->height()));
    //设置缓和曲线
    animation->setEasingCurve(QEasingCurve::OutBounce);
    //执行动画
    animation->start();
}

//鼠标按下事件
void MyPushButton::mousePressEvent(QMouseEvent *e){
    if(this->pressImgPath != "")//传入选中后的图片路径不为空，需要选中后来进行图片的切换
    {
        QPixmap pix;
        bool ret = pix.load(this->pressImgPath);
        if(!ret){
            qDebug()<<"load failed";
            return;
        }
        //设定图片区域的大小
        this->setFixedSize(pix.width(),pix.height());
        //设置不规则图片样式
        this->setStyleSheet("QPushButton{border:0px;}");//这里是CSS语言
        //设置图标
        this->setIcon(pix);
        //设置图片的大小
        this->setIconSize(QSize(pix.width(),pix.height()));
    }
    //其他事情 交给父类处理
    QPushButton::mousePressEvent(e);
}
//鼠标释放事件
void MyPushButton::mouseReleaseEvent(QMouseEvent *e){
    if(this->pressImgPath != "")//传入选中后的图片路径不为空，需要选中后来进行图片的切换
    {
        QPixmap pix;
        bool ret = pix.load(this->normalImgPath);
        if(!ret){
            qDebug()<<"load failed";
            return;
        }
        //设定图片区域的大小
        this->setFixedSize(pix.width(),pix.height());
        //设置不规则图片样式
        this->setStyleSheet("QPushButton{border:0px;}");//这里是CSS语言
        //设置图标
        this->setIcon(pix);
        //设置图片的大小
        this->setIconSize(QSize(pix.width(),pix.height()));
    }
    //其他事情 交给父类处理
    QPushButton::mouseReleaseEvent(e);
}
