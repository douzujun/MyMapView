#ifndef DIJKSTRAFINDPATH_H
#define DIJKSTRAFINDPATH_H
#include <QVector>


struct ArcCell{  //弧信息
    int adj;     //对无权图有1，0表示是否相邻，对带权图，则为权值类型
//    string info; //该弧的相关信息
};

const int MAX_VERTEX_NUM = 31;
const int INF = INT_MAX;

struct MGraph{
    QVector<int> vexs;                                    //顶点集合
    //临接矩阵
    ArcCell arcs[MAX_VERTEX_NUM][MAX_VERTEX_NUM];
    int vexnum;                                           //顶点数
    int arcnum;                                           //边数
//    int kind;                                             //图的类型
};

typedef int PathMatrix[MAX_VERTEX_NUM][MAX_VERTEX_NUM];   // 路径矩阵，二维数组
typedef int ShortPathTable[MAX_VERTEX_NUM];               // 最短距离表，一维数组

class DijkstraFindPath
{
public:
    DijkstraFindPath();
    MGraph mgraph;
    void CreateGraph();
    void ShortestPath(int v0, PathMatrix &path, ShortPathTable &DP, int Pa[]);
};

#endif // DIJKSTRAFINDPATH_H
