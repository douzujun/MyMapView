#include "dijkstrafindpath.h"

DijkstraFindPath::DijkstraFindPath()
{
    mgraph.vexnum = 30;                      //初始化点数目
    for (int i = 0; i <= mgraph.vexnum; i++) //初始化点编号
        mgraph.vexs.push_back (i);
    mgraph.arcnum = 900;                     //暂定
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

void DijkstraFindPath::CreateGraph ()
{
    mgraph.arcs[0][1].adj = mgraph.arcs[1][0].adj = 45;    //6 - 5
    mgraph.arcs[0][6].adj = mgraph.arcs[6][0].adj = 165;   //6 - 10
    mgraph.arcs[1][2].adj = mgraph.arcs[2][1].adj = 45;    //5 - 4
    mgraph.arcs[2][3].adj = mgraph.arcs[3][2].adj = 45;    //4 - 3
    mgraph.arcs[3][4].adj = mgraph.arcs[4][3].adj = 45;    //3 - 2
    mgraph.arcs[4][5].adj = mgraph.arcs[5][4].adj = 45;    //2 - 1
    mgraph.arcs[13][15].adj = mgraph.arcs[15][13].adj = 50;//23 - 22
    mgraph.arcs[0][13].adj = mgraph.arcs[13][0].adj = 45;  //6 - 23
    mgraph.arcs[13][2].adj = mgraph.arcs[2][13].adj = 30;  //22 - 4
    mgraph.arcs[5][11].adj = mgraph.arcs[11][5].adj = 65;  //1 - 一食堂
    mgraph.arcs[11][12].adj = mgraph.arcs[12][11].adj = 10;//一食堂-操场
    mgraph.arcs[11][27].adj = mgraph.arcs[27][11].adj = 85;//一食堂-祁通1
    mgraph.arcs[27][28].adj = mgraph.arcs[28][27].adj = 85;//祁通1-祁通2(路口)
    mgraph.arcs[5][29].adj = mgraph.arcs[29][5].adj = 87;  //一食堂-岔路口
    mgraph.arcs[29][9].adj = mgraph.arcs[9][29].adj = 80;  //岔路-7
    mgraph.arcs[29][30].adj = mgraph.arcs[30][29].adj = 32;//岔路-祁通大道
    mgraph.arcs[30][10].adj = mgraph.arcs[10][30].adj = 90;//祁通大道-图书馆
    mgraph.arcs[30][28].adj = mgraph.arcs[28][30].adj = 80;//祁通大道-祁通2
    mgraph.arcs[28][26].adj = mgraph.arcs[26][28].adj = 15;//祁通2-方肇周
    mgraph.arcs[25][27].adj = mgraph.arcs[27][25].adj = 273;    //西大门-祁通1
    mgraph.arcs[27][28].adj = mgraph.arcs[28][27].adj = 184;//祁通1-祁通2
    mgraph.arcs[25][12].adj = mgraph.arcs[12][25].adj = 108; //西大门-西操
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
    mgraph.arcs[18][19].adj = mgraph.arcs[19][18].adj = 39;//15 - 14
    mgraph.arcs[17][20].adj = mgraph.arcs[20][17].adj = 137;//16 - 19
    mgraph.arcs[20][21].adj = mgraph.arcs[21][20].adj = 39; //19 - 18
    mgraph.arcs[21][22].adj = mgraph.arcs[22][21].adj = 39; //18 - 17
    mgraph.arcs[19][22].adj = mgraph.arcs[22][19].adj = 130;//14 - 17
    mgraph.arcs[22][23].adj = mgraph.arcs[23][22].adj = 53; //17 - 二超
    mgraph.arcs[23][24].adj = mgraph.arcs[24][23].adj = 5;  //二超 - 二食堂
    mgraph.arcs[24][10].adj = mgraph.arcs[10][24].adj = 260;//二食堂-图书馆
    mgraph.arcnum = 80;
}


void DijkstraFindPath::ShortestPath (int v0, PathMatrix &path, ShortPathTable &DP, int Pa[])
{
    int v, w, i, j, min;
    bool final[MAX_VERTEX_NUM];   //辅助矩阵
    //初始化
    for (v = 0; v < mgraph.vexnum; v++)
    {
        final[v] = false;
        //存放v0->v最短距离,初值为v0->v的直接距离
        DP[v] = mgraph.arcs[v0][v].adj;
        for (w = 0; w < mgraph.vexnum; w++) {
            path[v][w] = false;   //设置空路径
        }
        if (DP[v] < INF) {        //设置开始可达路径
            path[v][v0] = path[v][v] = true;
        }
    }
    final[v0] = true;  //v0顶点并入S集
    for (i = 1; i < mgraph.vexnum; i++)
    {//其余G.vexnum - 1个顶点
     //开始主循环,每次求得v0到某个顶点v的最近距离，设初值INF
        min = INF;
        for (w = 0; w < mgraph.vexnum; w++)
        {//对所有顶点检查
            if (!final[w] && DP[w] < min)
            {//在S集之外的顶点(未访问)，并将其赋给v,距离给min
                v = w; min = DP[w];
            }
        }
        final[v] = true;   //离v0最近的v并入S集
        for (w = 0; w < mgraph.vexnum; w++)
        {//根据新并入的顶点，更新不在S集的顶点到v0距离和路径数组
            if (!final[w] && min < INF && mgraph.arcs[v][w].adj < INF &&
                    (min + mgraph.arcs[v][w].adj < DP[w]))
            { // w不属于S集且v0→v→w的距离<目前v0→w的距离
                DP[w] = min + mgraph.arcs[v][w].adj; // 更新D[w]
                Pa[w] = v;
                for (j = 0; j < mgraph.vexnum; j++)
                {// 修改path[w]，v0到w经过的顶点包括v0到v经过的顶点再加上顶点w
                    path[w][j] = path[v][j];
                }
                path[w][w] = true;
            }
        }

    }

}














