#include "mainwindow.h"
#include "mapwidget.h"
#include <QApplication>
#include <QFont>
#include <QDebug>
#include <QPixmap>
#include <QSplashScreen>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QPixmap pixmap("312");        //创建QPixmap对象，设置启动图片
    QSplashScreen splash(pixmap); //利用QPixmap对象创建一个QSplashScreen对象
    splash.show ();               //显示此启动图片

    a.processEvents ();           //使程序在显示启动动画的同时能能响应鼠标等其他事件

    QFont font("ARPL KaitiM GB", 12);
    font.setBold (true);
    a.setFont (font);

    qDebug() << "Run............";
    MainWindow w;
    w.show ();

    //表示在主窗体对象初始化完成后，结束启动画面
    splash.finish (&w);


    return a.exec();
}
