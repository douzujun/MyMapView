#include "mainwindow.h"
#include <qdebug.h>
#include <QToolBar>
#include <QtAlgorithms>
#include <iostream>
#include <windows.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle ("可爱的豆豆豆 校园导航");
    dj = new MainWindow::DijkstraFindPath();
    dj->CreateGraph ();

    scene = new QGraphicsScene;
    scene->setSceneRect (-100, -100, 700, 700);
    initScene();

    view = new QGraphicsView;
    view->setScene (scene);
    view->setMinimumSize (800, 800);
    view->show ();
    setCentralWidget (view);

    createAction ();
    createToolBar ();           //实现一个工具栏
    setMinimumSize (800, 800);  //设置最小尺寸
    Sleep(2000);
}

MainWindow::DijkstraFindPath::DijkstraFindPath()
{
    mgraph.vexnum = 40;                        //初始化点数目
       for (int i = 0; i < mgraph.vexnum; i++) //初始化点编号
           mgraph.vexs.push_back (i);
       mgraph.arcnum = 98;                     //暂定
       for (int i = 0; i < mgraph.vexnum; i++) {
           for (int j = 0; j < mgraph.vexnum; j++) {
               if (i == j)
                   mgraph.arcs[i][j].adj = 0;
               else
                   mgraph.arcs[i][j].adj = INF;
   //            mgraph.arcs[i][j].info = "";
           }
       }
}

void MainWindow::DijkstraFindPath::CreateGraph ()
{
        mgraph.arcs[0][1].adj = mgraph.arcs[1][0].adj = 45;    //6 - 5
        mgraph.arcs[0][6].adj = mgraph.arcs[6][0].adj = 165;   //6 - 10
        mgraph.arcs[1][2].adj = mgraph.arcs[2][1].adj = 45;    //5 - 4
        mgraph.arcs[2][3].adj = mgraph.arcs[3][2].adj = 45;    //4 - 3
        mgraph.arcs[3][4].adj = mgraph.arcs[4][3].adj = 45;    //3 - 2
        mgraph.arcs[3][15].adj = mgraph.arcs[15][3].adj = 24;  //3 - 22
        mgraph.arcs[4][5].adj = mgraph.arcs[5][4].adj = 45;    //2 - 1
        mgraph.arcs[13][15].adj = mgraph.arcs[15][13].adj = 85;//23 - 22
        mgraph.arcs[0][13].adj = mgraph.arcs[13][0].adj = 55;  //6 - 23
        mgraph.arcs[13][2].adj = mgraph.arcs[2][13].adj = 50;  //23 - 4
        mgraph.arcs[5][11].adj = mgraph.arcs[11][5].adj = 65;  //1 - 一食堂
        mgraph.arcs[11][12].adj = mgraph.arcs[12][11].adj = 10;//一食堂-操场
        mgraph.arcs[11][27].adj = mgraph.arcs[27][11].adj = 85;//一食堂-祁通1
        mgraph.arcs[27][28].adj = mgraph.arcs[28][27].adj = 85;//祁通1-祁通2(路口)
        mgraph.arcs[5][29].adj = mgraph.arcs[29][5].adj = 87;  //一食堂-岔路口
//        mgraph.arcs[29][9].adj = mgraph.arcs[9][29].adj = 80;  //岔路-7
        mgraph.arcs[29][11].adj = mgraph.arcs[11][29].adj = 50;//一食堂到岔路(通向7号楼的)
        mgraph.arcs[29][30].adj = mgraph.arcs[30][29].adj = 32;//岔路-祁通大道
        mgraph.arcs[30][10].adj = mgraph.arcs[10][30].adj = 90;//祁通大道-图书馆
        mgraph.arcs[30][28].adj = mgraph.arcs[28][30].adj = 80;//祁通大道-祁通2
        mgraph.arcs[28][26].adj = mgraph.arcs[26][28].adj = 15;//祁通2-方肇周
        mgraph.arcs[25][27].adj = mgraph.arcs[27][25].adj = 273;   //西大门-祁通1
        mgraph.arcs[27][28].adj = mgraph.arcs[28][27].adj = 84;   //祁通1-祁通2
        mgraph.arcs[5][12].adj = mgraph.arcs[12][5].adj = 70;  //1 - 操场
        mgraph.arcs[6][7].adj = mgraph.arcs[7][6].adj = 45;    //10 - 9
        mgraph.arcs[7][8].adj = mgraph.arcs[8][7].adj = 45;    //9 - 8
        mgraph.arcs[8][9].adj = mgraph.arcs[9][8].adj = 45;    //8 - 7
        mgraph.arcs[9][10].adj = mgraph.arcs[10][9].adj = 150; //7 - 图书馆
        mgraph.arcs[6][14].adj = mgraph.arcs[14][6].adj = 140; //10 - 13
        mgraph.arcs[14][16].adj = mgraph.arcs[16][14].adj = 39;//13 - 12
        mgraph.arcs[14][17].adj = mgraph.arcs[17][14].adj = 39;//13 - 16
        mgraph.arcs[16][18].adj = mgraph.arcs[18][16].adj = 39;//12 - 15
        mgraph.arcs[17][18].adj = mgraph.arcs[18][17].adj = 39;//16 - 15
        mgraph.arcs[18][19].adj = mgraph.arcs[19][18].adj = 20;//15 - 14
        mgraph.arcs[17][20].adj = mgraph.arcs[20][17].adj = 137;//16 - 19
        mgraph.arcs[20][21].adj = mgraph.arcs[21][20].adj = 39; //19 - 18
        mgraph.arcs[21][22].adj = mgraph.arcs[22][21].adj = 39; //18 - 17
        mgraph.arcs[19][22].adj = mgraph.arcs[22][19].adj = 130;//14 - 17
        mgraph.arcs[22][23].adj = mgraph.arcs[23][22].adj = 53; //17 - 二超
        mgraph.arcs[23][24].adj = mgraph.arcs[24][23].adj = 5;  //二超 - 二食堂
//        mgraph.arcs[24][10].adj = mgraph.arcs[10][24].adj = 260;//二食堂-图书馆

        //以下处理细节
        mgraph.arcs[30][31].adj = mgraph.arcs[31][30].adj = 30; //祁通大道-祁通大道2
        mgraph.arcs[31][32].adj = mgraph.arcs[32][31].adj = 10; //祁通大道2-祁通大道3
        mgraph.arcs[32][10].adj = mgraph.arcs[10][32].adj = 20; //祁通大道3-图书馆
        mgraph.arcs[10][33].adj = mgraph.arcs[33][10].adj = 80; //图书馆-祁通大道4
        mgraph.arcs[33][34].adj = mgraph.arcs[34][33].adj = 45; //祁通4-祁通5
        mgraph.arcs[34][24].adj = mgraph.arcs[24][34].adj = 45; //祁通5-二食堂
        mgraph.arcs[34][23].adj = mgraph.arcs[23][34].adj = 45; //祁通5-二超
        mgraph.arcs[33][35].adj = mgraph.arcs[35][33].adj = 30; //祁通4-通向14号楼的小道
        mgraph.arcs[35][19].adj = mgraph.arcs[19][35].adj = 10; //小道-14号楼
        mgraph.arcs[35][36].adj = mgraph.arcs[36][35].adj = 10; //小道14-小道15
        mgraph.arcs[36][18].adj = mgraph.arcs[18][36].adj = 10; //小道15-15
        mgraph.arcs[36][16].adj = mgraph.arcs[16][36].adj = 5;  //小道15-12
        mgraph.arcs[37][29].adj = mgraph.arcs[29][37].adj = 40; //岔路-岔路2
        mgraph.arcs[37][9].adj = mgraph.arcs[9][37].adj = 45;   //岔路2-7


}

void MainWindow::DijkstraFindPath::dijkstra (int startPos)
{
    for (int i = 0; i < mgraph.vexnum; i++) d[i] = INF;
    for (int i = 0; i < mgraph.vexnum; i++) used[i] = false;
    for (int i = 0; i < mgraph.vexnum; i++) prev[i] = -1;
    d[startPos] = 0;

    while (true) {
        int v = -1;
        for (int u = 0; u < mgraph.vexnum; u++) {
            if (!used[u] && (v == -1 || d[u] < d[v])) v = u;
        }

        if (v == -1) break;
        used[v] = true;

        for (int u = 0; u < mgraph.vexnum; u++) {
            if (d[u] > d[v] + mgraph.arcs[v][u].adj) {
                d[u] = d[v] + mgraph.arcs[v][u].adj;
                prev[u] = v;
            }
        }
    }
}

QVector<int> MainWindow::DijkstraFindPath::get_Path (int endPos)
{
    QVector<int> path;

    for ( ; endPos != -1; endPos = prev[endPos]) {
//        std::cout << "EndPos: " << endPos << ", ";
        path.push_back (endPos);
    }

    std::reverse(path.begin (), path.end ());

    return path;
}

void MainWindow::initScene ()
{
    QGraphicsPixmapItem *item =
            scene->addPixmap (QPixmap("NanTong.jpg"));
//    item->setFlag (QGraphicsItem::ItemIsMovable);
    item->setPos (-500, -420);
}

MainWindow::~MainWindow()
{

}

void MainWindow::createAction ()
{
    findPathAction = new QAction(QIcon("Search.png"),tr("搜索路径"), this);
    findPathAction->setShortcut (tr("Ctrl+F"));
    findPathAction->setStatusTip (tr("搜索路径"));
    connect (findPathAction, SIGNAL(triggered(bool)), this, SLOT(FindPath()));

    clearAction = new QAction(QIcon("Clear.png"), tr("清理路径"), this);
    clearAction->setShortcut (tr("Ctrl+W"));
    clearAction->setStatusTip (tr("清理路径"));
    connect (clearAction, SIGNAL(triggered(bool)), this, SLOT(Clear()));

    reviseAction = new QAction(QIcon("revise.png"), tr("修改景点"), this);
    reviseAction->setShortcut (tr("Ctrl+R"));
    clearAction->setStatusTip (tr("修改景点"));
    connect (reviseAction, SIGNAL(triggered(bool)), this, SLOT(Revise()));

    callMap = new QAction(QIcon("map.png"), tr("调用测试地图"), this);
    callMap->setShortcut (tr("Ctrl+M"));
    callMap->setStatusTip (tr("调用测试地图"));
    connect (callMap, SIGNAL(triggered(bool)), this, SLOT(callOtherMap()));

}

void MainWindow::createToolBar ()
{
    QToolBar *toolBar = addToolBar ("Tool");
    startLabel = new QLabel(tr("起点: "));
    startComboBox = new QComboBox;
    startComboBox->addItem (tr("公寓6号楼"));  //0
    startComboBox->addItem (tr("公寓5号楼"));  //1
    startComboBox->addItem (tr("公寓4号楼"));  //2
    startComboBox->addItem (tr("公寓3号楼"));  //3
    startComboBox->addItem (tr("公寓2号楼"));  //4
    startComboBox->addItem (tr("公寓1号楼"));  //5

    startComboBox->addItem (tr("公寓10号楼")); //6
    startComboBox->addItem (tr("公寓9号楼"));  //7
    startComboBox->addItem (tr("公寓8号楼"));  //8
    startComboBox->addItem (tr("公寓7号楼"));  //9
    startComboBox->addItem (tr("图书馆"));     //10
    startComboBox->addItem (tr("一食堂")); startComboBox->addItem (tr("西操场"));  //11 12
    startComboBox->addItem (tr("公寓23号楼")); startComboBox->addItem (tr("公寓13号楼")); //13 14
    startComboBox->addItem (tr("公寓22号楼")); startComboBox->addItem (tr("公寓12号楼")); //15 16
    startComboBox->addItem (tr("公寓楼16")); startComboBox->addItem (tr("公寓楼15"));     //17 18
    startComboBox->addItem (tr("公寓楼14"));  startComboBox->addItem (tr("公寓楼19"));    //19 20
    startComboBox->addItem (tr("公寓楼18")); startComboBox->addItem (tr("公寓楼17"));     //21 22
    startComboBox->addItem (tr("二超")); startComboBox->addItem (tr("二食堂"));           //23 24
    startComboBox->addItem (tr("西大门")); startComboBox->addItem (tr("方肇周教学楼"));    //25 26

    endLabel = new QLabel(tr("终点: "));

    endComboBox = new QComboBox;
    endComboBox->addItem (tr("公寓6号楼"));
    endComboBox->addItem (tr("公寓5号楼"));
    endComboBox->addItem (tr("公寓4号楼"));
    endComboBox->addItem (tr("公寓3号楼"));
    endComboBox->addItem (tr("公寓2号楼"));
    endComboBox->addItem (tr("公寓1号楼"));
    endComboBox->addItem (tr("公寓10号楼"));
    endComboBox->addItem (tr("公寓9号楼"));
    endComboBox->addItem (tr("公寓8号楼"));
    endComboBox->addItem (tr("公寓7号楼"));
    endComboBox->addItem (tr("图书馆"));
    endComboBox->addItem (tr("一食堂"));    endComboBox->addItem (tr("西操场"));
    endComboBox->addItem (tr("公寓23号楼"));endComboBox->addItem (tr("公寓13号楼"));
    endComboBox->addItem (tr("公寓22号楼"));endComboBox->addItem (tr("公寓12号楼"));
    endComboBox->addItem (tr("公寓楼16"));  endComboBox->addItem (tr("公寓楼15"));
    endComboBox->addItem (tr("公寓楼14"));  endComboBox->addItem (tr("公寓楼19"));
    endComboBox->addItem (tr("公寓楼18"));  endComboBox->addItem (tr("公寓楼17"));
    endComboBox->addItem (tr("二超"));      endComboBox->addItem (tr("二食堂"));
    endComboBox->addItem (tr("西大门"));    endComboBox->addItem (tr("方肇周教学楼"));

    connect (startComboBox, SIGNAL(activated(int)), this, SLOT(setStartStation()));
    connect (endComboBox, SIGNAL(activated(int)), this, SLOT(setEndStation()));

    toolBar->addWidget (startLabel);
    toolBar->addWidget (startComboBox);
    toolBar->addSeparator ();
    toolBar->addWidget (endLabel);
    toolBar->addWidget (endComboBox);
    toolBar->addSeparator ();
    toolBar->addAction(findPathAction);
    toolBar->addSeparator ();
    toolBar->addAction(clearAction);
    toolBar->addSeparator ();
    toolBar->addAction(reviseAction);
    toolBar->addSeparator ();
    toolBar->addAction(callMap);

}

void MainWindow::setStart(int X, int Y) {
    startX = X; startY = Y;
//    qDebug() << X << ", " << Y;
}

void MainWindow::setEnd (int X, int Y)
{
    endX = X; endY = Y;
}

void MainWindow::setStartStation ()
{
    switch (startComboBox->currentIndex ()) {
    case 0:
        setStart(-660, -534); break;
    case 1:
        setStart (-673, -490); break;
    case 2:
        setStart (-682, -446); break;
    case 3:
        setStart (-690, -400); break;
    case 4:
        setStart (-701, -355); break;
    case 5:
        setStart (-711, -310); break;
    case 6:
        setStart (-457, -555); break;
    case 7:
        setStart (-449, -485); break;
    case 8:
        setStart(-446, -432); break;
    case 9:
        setStart (-451, -400); break;
    case 10:
        setStart (-328, -368); break;
    case 11:
        setStart (-720, -247); break;
    case 12:
        setStart (-789, -252); break;
    case 13:
        setStart (-721, -517); break;
    case 14:
        setStart (-271, -540); break;
    case 15:
        setStart (-721, -439); break;
    case 16:
        setStart (-274, -495); break;
    case 17:
        setStart (-180, -552); break;
    case 18:
        setStart (-178, -508); break;
    case 19:
        setStart (-179, -456); break;
    case 20:
        setStart (-41, -552); break;
    case 21:
        setStart (-56, -500); break;
    case 22:
        setStart (-59, -448); break;
    case 23:
        setStart (-83, -393); break;
    case 24:
        setStart (-69, -351); break;
    case 25:
        setStart (-1070, -92); break;
    case 26:
        setStart (-438, -125); break;
    case 27:
        setStart (-721, -119); break;
    case 28:
        setStart (-550, -122); break;
    case 29:
        setStart (-555, -263); break;
    case 30:
        setStart (-498, -235); break;
    case 31:
        setStart (-439, -306); break;
    case 32:
        setStart (-390, -344); break;
    case 33:
        setStart (-226, -386); break;
    case 34:
        setStart (-144, -370); break;
    case 35:
        setStart (-218, -454); break;
    case 36:
        setStart (-223, -495); break;
    case 37:
        setStart (-513, -379); break;
    default:
        break;
    }
}

void MainWindow::setEndStation ()
{
    switch (endComboBox->currentIndex ()) {
    case 0:
        setEnd(-660, -534); break;
    case 1:
        setEnd (-673, -490); break;
    case 2:
        setEnd (-682, -446); break;
    case 3:
        setEnd (-690, -400); break;
    case 4:
        setEnd (-701, -355); break;
    case 5:
        setEnd (-711, -310); break;
    case 6:
        setEnd (-457, -555); break;
    case 7:
        setEnd (-449, -485); break;
    case 8:
        setEnd (-446, -432); break;
    case 9:
        setEnd (-451, -400); break;
    case 10:
        setEnd (-328, -368); break;
    case 11:
        setEnd (-720, -247); break;
    case 12:
        setEnd (-789, -252); break;
    case 13:
        setEnd (-721, -517); break;
    case 14:
        setEnd (-271, -540); break;
    case 15:
        setEnd (-721, -439); break;
    case 16:
        setEnd (-274, -495); break;
    case 17:
        setEnd (-180, -552); break;
    case 18:
        setEnd (-178, -508); break;
    case 19:
        setEnd (-179, -456); break;
    case 20:
        setEnd (-41, -552); break;
    case 21:
        setEnd (-56, -500); break;
    case 22:
        setEnd (-59, -448); break;
    case 23:
        setEnd (-83, -393); break;
    case 24:
        setEnd (-69, -351); break;
    case 25:
        setEnd (-1070, -92); break;
    case 26:
        setEnd (-438, -125); break;
    case 27:
        setEnd (-721, -119); break;
    case 28:
        setEnd (-550, -122); break;
    case 29:
        setEnd (-555, -263); break;
    case 30:
        setEnd (-498, -235); break;
    case 31:
        setEnd (-439, -306); break;
    case 32:
        setEnd (-390, -344); break;
    case 33:
        setEnd (-226, -386); break;
    case 34:
        setEnd (-144, -370); break;
    case 35:
        setEnd (-218, -454); break;
    case 36:
        setEnd (-223, -495); break;
    case 37:
        setEnd (-513, -379); break;
    default:
        break;
    }
}

void MainWindow::setNextPos (int index)
{
    switch (index) {
    case 0:
        setEnd(-660, -534); break;
    case 1:
        setEnd (-673, -490); break;
    case 2:
        setEnd (-682, -446); break;
    case 3:
        setEnd (-690, -400); break;
    case 4:
        setEnd (-701, -355); break;
    case 5:
        setEnd (-711, -310); break;
    case 6:
        setEnd (-457, -555); break;
    case 7:
        setEnd (-449, -485); break;
    case 8:
        setEnd (-446, -432); break;
    case 9:
        setEnd (-451, -400); break;
    case 10:
        setEnd (-328, -368); break;
    case 11:
        setEnd (-720, -247); break;
    case 12:
        setEnd (-789, -252); break;
    case 13:
        setEnd (-721, -517); break;
    case 14:
        setEnd (-271, -540); break;
    case 15:
        setEnd (-721, -439); break;
    case 16:
        setEnd (-274, -495); break;
    case 17:
        setEnd (-180, -552); break;
    case 18:
        setEnd (-178, -508); break;
    case 19:
        setEnd (-179, -456); break;
    case 20:
        setEnd (-41, -552); break;
    case 21:
        setEnd (-56, -500); break;
    case 22:
        setEnd (-59, -448); break;
    case 23:
        setEnd (-83, -393); break;
    case 24:
        setEnd (-69, -351); break;
    case 25:
        setEnd (-1070, -92); break;
    case 26:
        setEnd (-438, -125); break;
    case 27:
        setEnd (-721, -119); break;
    case 28:
        setEnd (-550, -122); break;
    case 29:
        setEnd (-555, -263); break;
    case 30:
        setEnd (-498, -235); break;
    case 31:
        setEnd (-439, -306); break;
    case 32:
        setEnd (-390, -344); break;
    case 33:
        setEnd (-226, -386); break;
    case 34:
        setEnd (-144, -370); break;
    case 35:
        setEnd (-218, -454); break;
    case 36:
        setEnd (-223, -495); break;
    case 37:
        setEnd (-513, -379); break;
    default:
        break;
    }
}

void MainWindow::FindPath ()
{
    //Demo 在图片上绘线 在原有基础上 (+700, +440);
    QVector<QPoint> v;

    dj->dijkstra (startComboBox->currentIndex ());
    //设置下一处的终点
    nextPath = dj->get_Path (endComboBox->currentIndex ());

    //准备绘制
    Clear ();
    //将路线绘制下来
    QGraphicsPathItem *item = new QGraphicsPathItem();

    QPen pen;
    pen.setWidth (4);
    pen.setColor (Qt::red);
    item->setPen (pen);
    item->setFlag (QGraphicsItem::ItemIsPanel);

//    qDebug() << startX << " " << startY << " " << endX << " " << endY;

//    qDebug() << "Hello World !";


    for (int i = 1; i < nextPath.size (); i++) {
        qDebug() << nextPath[i] << " , ";
    }
    scene->addItem (item);

    QPainterPath pa;               //path

    pa.moveTo (startX + 700, startY + 440);

    //将路径用坐标存入到路径
    for (int i = 1; i < nextPath.size() ; i++) {
        setNextPos (nextPath[i]);
        pa.lineTo (endX + 700, endY + 440);
    }
    item->setPath (pa);

}

void MainWindow::Clear ()
{
    QList<QGraphicsItem*> listItem = scene->items ();
    while (!listItem.empty ())
    {
        scene->removeItem (listItem.at (0));
        listItem.removeAt (0);
    }
    QGraphicsPixmapItem *item =
            scene->addPixmap (QPixmap("NanTong.jpg"));
//    item->setFlag (QGraphicsItem::ItemIsMovable);
    item->setPos (-500, -420);
}

void MainWindow::Revise ()
{
    QDialog *reviseDlg = new QDialog;
    QLabel *reviseLabel = new QLabel(tr("修改图片路径："));
    QLineEdit *revison = new QLineEdit(tr("\\image\\library.png"));
    QPushButton *SureBtn = new QPushButton(tr("确定"));
    QPushButton *CancelBtn = new QPushButton(tr("取消"));

    revison->setEnabled (false);

    reviseComboBox = new QComboBox;
    reviseComboBox->addItem (tr("图书馆"));
    reviseComboBox->addItem (tr("食堂"));
    reviseComboBox->addItem (tr("西操"));
    reviseComboBox->addItem (tr("jsj教学楼"));

    connect(reviseComboBox, SIGNAL(activated(int)), this, SLOT(ShowDialog()));
    connect (SureBtn, SIGNAL(clicked(bool)), reviseDlg, SLOT(close()));
    connect (CancelBtn, SIGNAL(clicked(bool)), reviseDlg, SLOT(close ()));

    revison->setText (strPath);

    QGridLayout *mainlayout = new QGridLayout(reviseDlg);
    mainlayout->addWidget (reviseLabel, 0, 0); mainlayout->addWidget (revison, 0, 1);
    mainlayout->addWidget (reviseComboBox, 0 , 2);
    mainlayout->addWidget (SureBtn, 1, 1, 1, 1); mainlayout->addWidget (CancelBtn, 1, 2, 1, 1);


    reviseDlg->setWindowTitle (tr("修改景点信息"));
    reviseDlg->show ();
}

void MainWindow::ShowDialog ()
{
    strPath = QFileDialog::getOpenFileName (this, "打开", "/", "PNG图像(*.png)::JPEG图像(*.jpg)");
    switch (reviseComboBox->currentIndex ()) {
    case 0:
        library.load (strPath); break;
    case 1:
        canteen.load (strPath); break;
    case 2:
        westground.load (strPath); break;
    case 3:
        jxjBuilding.load (strPath); break;
    default:
        break;
    }
}

//鼠标事件
void MainWindow::mouseDoubleClickEvent (QMouseEvent *e)
{
    QDialog *dialog = new QDialog;
    QGridLayout *layout = new QGridLayout(dialog);
    label = new QLabel;

    qDebug() << "x : " << e->x () << ", y : " << e->y ();
    //食堂(1060, 260)
    if ( (e->x () >= 1060-40 && e->x () <= 1060 + 40) &&
         (e->y () >= 260 - 10 && e->y () <= 260 + 10 ))
    {
        strPath = "images//canteen";
        canteen.load (strPath);
        label->setPixmap (canteen);
        layout->addWidget (label);
        dialog->setMinimumSize (1193, 274);
        dialog->setWindowTitle ("二食堂");
        dialog->show ();
    }
    else if ((e->x () >= 708-50 && e->x () <= 708 + 50) &&
             (e->y () >= 498 - 40 && e->y () <= 498 + 40 ) ) {
        strPath = "images//computer";
        jxjBuilding.load (strPath);
        label->setPixmap (jxjBuilding);
        layout->addWidget (label);
        dialog->setMinimumSize (437, 265);
        dialog->setWindowTitle ("计算机楼(方肇周教学楼)");
        dialog->show ();
    }
    else if ((e->x () >= 787-50 && e->x () <= 787 + 50) &&
             (e->y () >= 287 - 50 && e->y () <= 287 + 50 ) ) {
        strPath = "images//library";
        library.load (strPath);
        label->setPixmap (library);
        layout->addWidget (label);
        dialog->setMinimumSize (397, 340);
        dialog->setWindowTitle ("图书馆");
        dialog->show ();
    }
    else if ((e->x () >= 64-50 && e->x () <= 64 + 100) &&
             (e->y () >= 516 - 100 && e->y () <= 516 + 100 ) ) {
        strPath = "images//westgate";
        westgate.load (strPath);
        label->setPixmap (westgate);
        layout->addWidget (label);
        dialog->setMinimumSize (1100, 283);
        dialog->setWindowTitle ("西大门");
        dialog->show ();
    }
    else if ((e->x () >= 200-200 && e->x () <= 200 + 200) &&
             (e->y () >= 167 - 500 && e->y () <= 167 + 500 ) ) {
        strPath = "images//westground";
        westground.load (strPath);
        label->setPixmap (westground);
        layout->addWidget (label);
        dialog->setMinimumSize (395, 345);
        dialog->setWindowTitle ("西操");
        dialog->show ();
    }
    else if ((e->x () >= 1045 - 5 && e->x () <= 1045 + 5) &&
             (e->y () >= 233 - 5 && e->y () <= 233 + 5 ) ) {
        strPath = "images//twoMarket";
        twoMarket.load (strPath);
        label->setPixmap (twoMarket);
        layout->addWidget (label);
        dialog->setMinimumSize (611, 294);
        dialog->setWindowTitle ("二超");
        dialog->show ();
    }
}

void MainWindow::callOtherMap ()
{
    mapWidget = new MapWidget;
    mapWidget->show ();
}









