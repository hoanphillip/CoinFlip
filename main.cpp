#include "mainscene.h"
#include <QApplication>
#include "dataconfig.h"
#include <QDebug>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainScene w;
    w.show();

//    dataConfig data;
//    for(int i=0;i<4;i++)
//    {
//        for(int j=0;j<4;j++)
//        {
//            qDebug()<<data.mData[1][i][j];
//        }
//        qDebug()<<"";
//    }

    return a.exec();
}
