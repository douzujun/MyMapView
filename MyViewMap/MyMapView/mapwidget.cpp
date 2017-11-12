#include "mapwidget.h"
#include <QSlider>         //滑动条
#include <QGridLayout>
#include <QFile>
#include <QTextStream>
#include <QGraphicsScene>
#include <math.h>

MapWidget::MapWidget()
{
    //读取地图信息--用于读取描述地图信息的文件(包括地图名及经纬度等信息)
    readMap ();
    zoom = 50;
    int width = map.width ();
    int height = map.height ();
    //新建一个QGraphicsScene对象为主窗口连接一个场景
    QGraphicsScene *scene = new QGraphicsScene(this);
    //限定场景的显示区域为地图大小
    scene->setSceneRect (-width/2, -height/2, width, height);
    setScene (scene);
    /***
     * The background is cached(隐藏，缓存). This affects both custom backgrounds, and
     * backgrounds based on the backgroundBrush property. When this flag is enabled,
     * QGraphicsView will allocate one pixmap with the full size of the viewport
     */
    setCacheMode (CacheBackground);
    /***
     * 用于地图缩放的滑动条
     * 新建一个QSlider对象作为地图的缩放控制
     */
    QSlider *slider = new QSlider;
    //设置滚动条方向-垂直
    slider->setOrientation (Qt::Vertical);
    slider->setRange (1, 100);        //设置地图缩放比例值范围为0~100
    slider->setTickInterval (10);     //显示刻度间隔为10
    slider->setValue (0);            //设置当前初始值为50
    // 将缩放控制条的valueChanged()信号与地图缩放slotZoom()槽函数相关联
    connect (slider, SIGNAL(valueChanged(int)), this, SLOT(slotZoom(int)));

    //缩放图标
    QLabel *zoominLabel = new QLabel;
    zoominLabel->setScaledContents (true);
    zoominLabel->setPixmap (QPixmap("zoomin.png"));
    QLabel *zoomoutLabel = new QLabel;
    zoomoutLabel->setScaledContents (true);
    zoomoutLabel->setPixmap (QPixmap("zoomout.png"));

    //坐标值显示区
    QLabel *label1 = new QLabel(tr("GraphicsView:"));
    viewCoord = new QLabel;
    QLabel *label2 = new QLabel(tr("GraphicsScene:"));
    sceneCoord = new QLabel;
    QLabel *label3 = new QLabel(tr("map:"));
    mapCoord = new QLabel;

    //坐标显示区布局
    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->addWidget (label1, 0, 0);
    gridLayout->addWidget (viewCoord, 0, 1);

    gridLayout->addWidget (label2, 1, 0);
    gridLayout->addWidget (sceneCoord, 1, 1);

    gridLayout->addWidget (label3, 2, 0);
    gridLayout->addWidget (mapCoord, 2, 1);

    gridLayout->setSizeConstraint (QLayout::SetFixedSize);
    QFrame *coordFrame = new QFrame;
    coordFrame->setLayout (gridLayout);

    //缩放控制子布局
    QVBoxLayout *zoomLayout = new QVBoxLayout;
    zoomLayout->addWidget (zoominLabel);
    zoomLayout->addWidget (slider);
    zoomLayout->addWidget (zoomoutLabel);

    //坐标显示区布局
    QVBoxLayout *coordLayout = new QVBoxLayout;
    coordLayout->addWidget (coordFrame);
    coordLayout->addStretch ();
    //主布局
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout (zoomLayout);
    mainLayout->addLayout (coordLayout);
    mainLayout->addStretch ();          //平均分割
    mainLayout->setMargin (30);         //设置对话框(或窗体)的边距为30
    mainLayout->setSpacing (10);        //设定各个控件之间的间距为10
    setLayout (mainLayout);             //在场景中设置为布局

    setWindowTitle ("Map Widget");
    setMinimumSize (600, 400);
}

//读取地图信息
void MapWidget::readMap ()
{
    QString mapName;
    //新建一个QFile对象，“map.txt"是描述地图信息的文本文件
    QFile mapFile("maps.txt");
    //以"只读"的方式打开此文件
    int ok = mapFile.open (QIODevice::ReadOnly);
    if (ok)                   //分别以读取地图的名称和四个经纬度信息
    {
        QTextStream ts(&mapFile);
        if (!ts.atEnd ()) {              //没有到末尾返回false
            ts >> mapName;               //储存字符串
            ts >> x1 >> y1 >> x2 >> y2;  //储存地图左上角和右下角的经纬度
        }
    }
    map.load (mapName);      //将地图读取至私有标量map中
}

//根据缩放滑动条的当前值,确定缩放的比例,调用scale()函数实现地图缩放--完成地图缩放功能slotZoom
void MapWidget::slotZoom (int value)
{
    qreal s;                  //缩放大小
    if (value > zoom) {       //放大
        s = pow (1.01, (value - zoom));
    }
    else {
        s = pow(1/1.01, (zoom - value));
    }
    scale(s, s);
    zoom = value;            //当前放大值
}

//drawBackground()--以地图图片重绘场景的背景来实现地图显示
void MapWidget::drawBackground (QPainter *painter, const QRectF &rect)
{
    /***
     * The scene rectangle defines the extent of the scene, and in the view's case,
     * this means the area of the scene that you can navigate using the scroll bars.
     */
    painter->drawPixmap (int(sceneRect ().left ()) + 10, int(sceneRect ().top ()) - 10, map);

}

//完成某点在各层坐标中的映射及显示
void MapWidget::mouseMoveEvent (QMouseEvent *event)
{
    //QGraphicesView 坐标
    QPoint viewPoint = event->pos ();   //鼠标事件位置
    viewCoord->setText (QString::number (viewPoint.x ()) + "," + QString::number (viewPoint.y ()));

    //QGraphiccsScene 坐标 -- 将视图坐标转换成场景坐标
    QPointF scenePoint = mapToScene (viewPoint);
    sceneCoord->setText (QString::number (scenePoint.x ()) + "," + QString::number (scenePoint.y ()));

    //地图坐标(经，纬度值)
    QPointF latLon = mapToMap (scenePoint);
    mapCoord->setText (QString::number (latLon.x ()) + "," + QString::number (latLon.y ()));


}

//完成从场景至地图坐标的转换mapToMap()
QPointF MapWidget::mapToMap (QPointF p)
{
    QPointF latLon;       //地图坐标
    qreal w = sceneRect ().width ();      //场景长度
    qreal h = sceneRect ().height ();     //场景高度
    qreal lon = y1 - ( (h/2+p.y ()) * abs(y1-y2)/h );
    qreal lat = x1 + ( (w/2+p.x ()) * abs(x1-x2)/w );

    latLon.setX (lat);
    latLon.setY (lon);
    return latLon;
}
























