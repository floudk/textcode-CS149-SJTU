
//BFS & DFS
//寻找欧拉回路、拓扑排序、关键路径
//kruskal  Prim
//dijkstra  Floyd

#include <iostream>
#include <list>
#include <queue>
#include<string>
#include <set>
#include <stack>
#include <unordered_set>
#include <vector>
using namespace std;




class Disjoint {//只接受int，使用者如果想判断其它类型的结点，利用hashtable等转换为int
private:
    int* joint;

public:
    Disjoint(int size) {
        joint = new int[size];
        for (int i = 0; i < size; i++) {
            joint[i] = -1;
        }
    };
    ~Disjoint() {
        delete[] joint;
    };

    void union1(int a, int b);
    int find(int a);
};
int Disjoint::find(int a) {

    int root_a = a;
    while (joint[root_a] >= 0)
        root_a = joint[root_a];
    return root_a;
}
void Disjoint::union1(int a, int b)
{
    //第一个版本，朴素拼接，直接拼接两数的根
    if (a == b)
        return;
    int root_a = find(a);
    int root_b = find(b);
    if (root_a == root_b)
        return;
    if (joint[root_b] > joint[root_a]) {
        //a的size更大
        joint[root_a] += joint[root_b];
        joint[root_b] = root_a;
    }
    else {
        joint[root_b] += joint[root_a];
        joint[root_a] = root_b;
    }
};



//*****************************************************************



template <typename Tv>
struct Vertex
{
    Tv val;
    //============
    //只有在使用时，才会初始化为正确数值，加减点或边时不维护
    //除非显式的重新计算，否则不保证其数据的正确性
    int indegree;  //入度
    int outdegree; //出度
    //============
    Vertex(Tv a) : val(a), indegree(0), outdegree(0) {};
};
template <typename Te>
struct Edge
{
    Te val;
    int visited; //实际最多三个bit位就可以表示
    //0代表 没访问 1 访问过
    Edge(Te a) : val(a), visited(0) {};
};

struct MinT_edge {
    int weight;
    int beg;
    int end;

    MinT_edge(int a, int s, int e) : weight(a), beg(s), end(e) {};
     friend bool operator<(const MinT_edge a, const MinT_edge& b) {
         return a.weight > b.weight;
    }
    bool operator==(const MinT_edge &b) {
        return b.weight == weight;
    }
    bool operator<=(const MinT_edge& b) {
        return weight >= b.weight;
    }
};

template <typename Tv, typename Te>
class Graph
{
private:
    //结点相关
    int n;
    vector<Vertex<Tv>> V;
    int _find(const Tv& v)
    {
        for (int i = 0; i < n; i++)
        {
            if (V[i].val == v)
                return i;
        }
        return -1;
    }
    void updatedegree();

    //边相关
    int e;
    vector<vector<Edge<Te>*>> E;
    void BFS(unordered_set<int>& visited, int i);
    void DFS(unordered_set<int>& visited, int i);
    void initEdge()
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if (E[i][j])
                    E[i][j]->visited = 0;
            }
        }
    }

    void find_cir(int start, vector<int>& lane) {
        //找到从s出发的回路
        vector<int> tmp;
        stack<int> s;
        s.push(start);
        bool second = 0;
        bool update = 0;
        while (!s.empty()) {
            int i = s.top();
            if (i == start) {
                if (second) {
                    break;
                }
                second ^= 1;
            }
            for (int j = 0; j < n; j++) {
                if (E[i][j] && E[i][j]->visited == 0) {
                    s.push(j);
                    update = 1;
                    break;
                }

            }
            if (update) {
                update = 0;
                continue;
            }
            s.pop();
        }
        //stack=[start,N1,N2,...start]
        int i = 0;
        for (; i < lane.size(); i++) {
            if (lane[i] == start)
                break;
        }
        while (!s.empty()) {
            tmp.push_back(s.top());
            s.pop();
        }

        for (int t = tmp.size() - 1; t > 0; t--) {
            int a = tmp[t];
            int b = tmp[t - 1];
            E[a][b]->visited = 1;
        }

        int j = 0;
        if (lane.size())
            j++;
        for (; j < tmp.size(); j++) {
            lane.insert(lane.begin() + i, tmp[j]);
        }
    }

    //其它辅助函数

    bool topsort(vector<int>& q) {
        updatedegree();
        bool update = 0;
        vector<bool> visited(V.size(), 0);
        while (q.size() < V.size()) {
            for (int i = 0; i < V.size(); i++) {
                if (visited[i] == 0 && V[i].indegree == 0) {
                    q.push_back(i);
                    visited[i] = 1;
                    for (int j = 0; j < n; j++) {
                        if (E[i][j]) {
                            V[j].indegree -= 1;;
                        }
                    }
                    update = 1;
                }
            }
            if (update) {

                update = 0;
                continue;
            }
            return 0;
        }
        return 1;
    }

public:
    Graph();
    ~Graph();

    //结点相关基本操作
    void insertVer(const Tv& ver);
    bool removeVer(const Tv& ver);
    bool existsVer(const Tv& ver);

    //边相关基本操作
    bool insertEdge(const Te& edge, const Tv& v1, const Tv& v2);
    bool removeEdge(const Tv& v1, const Tv& v2);
    bool existsEdge(const Tv& v1, const Tv& v2);

    //图算法相关

    //遍历
    void bfs()
    {
        /*考虑两个问题
            1.图如果不连通，我的遍历应该怎么办？ 对森林的所有树进行遍历
            2.我怎么判断一个点已经遍历过了呢？   搞一个额外的东西来存储已经遍历过的结点
        */
        unordered_set<int> visited; //hash支持O(1)的查询操作
        for (int i = 0; i < n; i++)
        {
            if (visited.find(i) == visited.end())
                BFS(visited, i);
        }
    }
    void dfs()
    {
        unordered_set<int> visited; //hash支持O(1)的查询操作
        for (int i = 0; i < n; i++)
        {
            if (visited.find(i) == visited.end())
                DFS(visited, i);
        }
    }

    //寻找欧拉回路(if have,print)
    void EulerCircuit(const Tv& start);
    void topSort();
    void criticalPath();
    void kruskal();
    void prim();
    void dijkstra(const Tv&a);
    void Floyd();
};

//=======================================================================
//========================================================================
template <typename Tv, typename Te>
Graph<Tv, Te>::Graph()
{
    n = e = 0;
}
template <typename Tv, typename Te>
Graph<Tv, Te>::~Graph()
{
    for (int i = 0; i < E.size(); i++)
    {
        for (int j = 0; j < E[i].size(); j++)
            delete E[i][j];
    }
}

template <typename Tv, typename Te>
void Graph<Tv, Te>::insertVer(const Tv& ver)
{
    V.push_back(Vertex<Tv>(ver));
    ++n;
    //边存放的矩阵需要增加一行和一列
    for (int i = 0; i < E.size(); i++)
    {
        E[i].push_back(nullptr);
    }
    E.push_back(vector<Edge<Te>*>(n, (Edge<Te>*)nullptr));
}

template <typename Tv, typename Te>
bool Graph<Tv, Te>::removeVer(const Tv& ver)
{

    int i = _find(ver);
    if (i == -1)
        //不存在这样一个点
        return 0;

    //删除出边
    for (int j = 0; j < n; j++)
    {
        if (E[i][j])
        {
            delete E[i][j];
            --e;
        }
    }

    //删除边集中i行
    E.erase(E.begin() + i);

    //删除点集
    V.erase(V.begin() + i);
    --n;

    //删除入边
    for (int j = 0; j < n; j++)
    {
        if (E[j][i])
        {
            delete E[j][i];
            --e;
        }
        E[j].erase(E[j].begin() + i);
    }
    return 1;
}

template <typename Tv, typename Te>
bool Graph<Tv, Te>::existsVer(const Tv& ver)
{
    return _find(ver) != -1;
}

template <typename Tv, typename Te>
bool Graph<Tv, Te>::insertEdge(const Te& edge, const Tv& v1, const Tv& v2)
{
    //有向图，从v1->v2
    int i = _find(v1), j = _find(v2);
    if (i == -1 || j == -1)
        return 0; //结点不在图中

    if (E[i][j])
        return 0; //不能表示多重边

    E[i][j] = new Edge<Te>(edge);
    ++e;

    return 1;
}

template <typename Tv, typename Te>
bool Graph<Tv, Te>::removeEdge(const Tv& v1, const Tv& v2)
{
    int i = _find(v1), j = _find(v2);
    if (i == -1 || j == -1 || E[i][j] == nullptr)
        return 0; //结点不在图中
    delete E[i][j];
    E[i][j] = nullptr;
    --e;
    return 1;
}

template <typename Tv, typename Te>
bool Graph<Tv, Te>::existsEdge(const Tv& v1, const Tv& v2)
{
    int i = _find(v1), j = _find(v2);
    if (i == -1 || j == -1 || E[i][j] == nullptr)
        return 0;
    return 1;
}

template <typename Tv, typename Te>
void Graph<Tv, Te>::BFS(unordered_set<int>& visited, int i)
{
    //从i开始，对包含i的部分的连通分支做BFS
    queue<int> q;
    q.push(i);
    while (!q.empty())
    {
        int tmp = q.front();
        q.pop();
        if (visited.find(tmp) != visited.end())
        {
            //防止某节点被多次加入队列
            //也可以再建1个hashtable
            //或者从结点struct下手，定义一个状态变量
            continue;
        }
        cout << V[tmp].val << '\n';
        visited.insert(tmp);
        for (int j = 0; j < E[tmp].size(); j++)
        {
            if (E[tmp][j] && visited.find(j) == visited.end())
            {
                q.push(j);
            }
        }
    }
}

template <typename Tv, typename Te>
void Graph<Tv, Te>::DFS(unordered_set<int>& visited, int i)
{
    stack<int> s;
    s.push(i);
    while (!s.empty())
    {
        int tmp = s.top();
        s.pop();
        if (visited.find(tmp) != visited.end())
        {
            continue;
        }
        cout << V[tmp].val << '\n';
        visited.insert(tmp);
        for (int j = E[tmp].size() - 1; j >= 0; j--)
        {
            if (E[tmp][j] && visited.find(j) == visited.end())
            {
                s.push(j);
            }
        }
    }
}
template <typename Tv, typename Te>
void Graph<Tv, Te>::updatedegree()
{
    vector<int> tmpin(n, 0);

    for (int i = 0; i < n; i++)
    {
        int tmpout = 0;
        for (int j = 0; j < n; j++)
        {
            if (E[i][j])
            {
                ++tmpin[j];
                ++tmpout;
            }
        }
        V[i].outdegree = tmpout;
    }
    for (int i = 0; i < n; i++)
        V[i].indegree = tmpin[i];
}

template <typename Tv, typename Te>
void Graph<Tv, Te>::EulerCircuit(const Tv& start)
{
    if (e == 0)
    {
        cout << "Not exist!\n";
        return;
    }
    updatedegree();
    for (int i = 0; i < n; i++)
    {
        if (V[i].indegree > 0 && V[i].indegree == V[i].outdegree)
            continue;
        cout << "Not exist!\n";
        return;
    }

    /*能运行到这里的图，必然存在欧拉回路
    1.考虑如何表示一个边已经被访问过了？维护一个边列表，或者给边struct一定的状态变量
    2.如何组合所有的边形成欧拉回路？课本采用拼接法（套圈法），也可采用Fleury（非必要不过桥）
    考虑到后面经常会用到边的状态，这里选择添加边的状态变量
    !!!!now<=========>inser
    */
    //初始化所有边的状态
    initEdge();
    //套圈法找欧拉回路
    int inlet = _find(start);
    vector<int> lane;
    find_cir(inlet, lane);
    bool update = 0;

    while (1) {
        for (int i = 0; i < lane.size(); i++) {
            for (int j = 0; j < n; j++) {
                if (E[lane[i]][j] && E[lane[i]][j]->visited == 0) {
                    find_cir(i, lane);
                    update = 1;
                    break;
                }
            }
            if (update) {
                break;
            }
        }
        if (update) {
            update = 0;
            break;
        }
        break;
    }
    for (auto i : lane) {
        cout << V[i].val << ' ';
    }
}
template <typename Tv, typename Te>
void Graph<Tv, Te>::topSort() {
    updatedegree();
    //不断将入度为0的结点加入到队列中
    bool update = 0;
    queue<int> q;
    vector<bool> visited(V.size(), 0);

    while (q.size() < V.size()) {
        for (int i = 0; i < V.size(); i++) {
            if (visited[i] == 0 && V[i].indegree == 0) {
                q.push(i);
                visited[i] = 1;
                for (int j = 0; j < n; j++) {
                    if (E[i][j]) {
                        V[j].indegree -= 1;;
                    }
                }
                update = 1;
            }
        }
        if (update) {

            update = 0;
            continue;
        }
        cout << "no applicable topSort\n";
        return;
    }
    while (!q.empty()) {
        cout << V[q.front()].val << ' ';
        q.pop();
    }
}

template <typename Tv, typename Te>
void Graph<Tv, Te>::criticalPath() {
    //find the topSort
    vector<int> top;
    if (topsort(top) == 0)
        cout << "no Solution";


    //先求最晚完成时间
    vector<int> ee(n, 0);
    for (int k = 0; k < top.size(); k++) {
        int  i = top[k];
        for (int j = 0; j < n; j++) {
            if (E[i][j]) {
                if (E[i][j]->val + ee[i] > ee[j])
                    ee[j] = E[i][j]->val + ee[i];
            }
        }
    }

    //求最早完成时间
    vector<int> le(n, ee[top.back()]);
    for (int k = top.size() - 1; k >= 0; k--) {
        int i = top[k];
        for (int j = 0; j < n; j++) {
            if (E[j][i]) {
                if (le[i] - E[j][i]->val < le[j])
                    le[j] = le[i] - E[j][i]->val;
            }
        }
    }

    //两者相同，即为criticalPath
    for (int i = 0; i < n; i++) {
        if (ee[i] == le[i]) {
            cout << "( " << V[i].val << ' ' << le[i] << " )";
        }
    }
}

template <typename Tv, typename Te>
void Graph<Tv, Te>::kruskal() {
    priority_queue<MinT_edge> pq;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (E[i][j]) {
                pq.push(MinT_edge(E[i][j]->val, i, j));
            }
        }
    }

    vector<MinT_edge> MT;
    int minW = 0;
    Disjoint a(n);
    while (!pq.empty()) {
        MinT_edge tmp = pq.top();
        pq.pop();
        int u = a.find(tmp.beg);
        int v = a.find(tmp.end);
        if ((u != v) || (u == -1)) {
            //<u,v> 可以入选
            minW += tmp.weight;
            MT.push_back(tmp);
            if (MT.size() == n - 1)
                break;
            a.union1(u, v);
        }
    }
    cout << minW << endl;
}


template <typename Tv, typename Te>
void Graph<Tv, Te>::prim(){
    vector<bool> flag(n, 0);

    int node = 1;
    int start = 0;
    flag[start] = 1;
    int sum = 0;

    vector<int> lastcost(n,INT_MAX); //记录从{S-U} --> {U}最小代价
    vector<int> adjNode(n);//记录中间结点

    while(node<n){
        for (int i = 0; i < n;i++){
        //更新距离信息(有向图，所以检查行和列)
            for (int j = 0; j < n;j++){
                if(E[i][j]&&(flag[i]^flag[j])){
                    int tmp = flag[i] ? j : i;
                    lastcost[tmp] = lastcost[tmp] < E[i][j]->val ? lastcost[tmp] : E[i][j]->val;
                    adjNode[tmp] = tmp == i ? j : i;
                }
            }
        }

        //找到最小代价的新结点加入
        int curr = 0;
        int mincost = INT_MAX;
        for (int i = 0; i < n;i++){
            if(!flag[i]){
                if(lastcost[i]<mincost){
                    mincost = lastcost;
                    curr = i;
                }
            }
        }
        flag[curr] = 1;
        ++node;

        sum += lastcost[curr];
    }
    
}

template <typename Tv, typename Te>
void Graph<Tv, Te>::dijkstra(const Tv& a) {
    //寻找所有边的最短路径,不能有负权值(事实上，这个实现版本允许有负，不能有负环）
    //如果到某个点没有路径，那么就是INT_MAX
    int start = _find(a);
    int origin = start;
    vector<int> dis(n, INT_MAX);
    vector<bool> known(n, 0);

    dis[start] = 0;
    known[start] = 1;

    while (1)
    {
        //更新新放入结点邻接点的距离
        for (int i = 0; i < n; i++)
        {
            if (E[start][i] && dis[i] > dis[start] + E[start][i]->val) {
                dis[i] = dis[start] + E[start][i]->val;
            }

        }
        //选取未加入集合的最短路径
        int minLane = INT_MAX;
        int curr = 0;
        bool full = 1;
        for (int i = 0; i < n; i++)
        {
            if (!known[i] && dis[i] < minLane)
            {
                minLane = dis[i];
                curr = i;
                full = 0;
            }
        }
        if (full)
            break;
        known[curr] = 1;
        start=curr;
    }

    for (int i = 0; i < n; i++) {
        int tmp = (origin + i) % n;
        string length = dis[tmp] == INT_MAX ? "No Way!" : to_string(dis[tmp]);
        cout << "from " << V[origin].val << " to " << V[tmp].val << ": " << length << '\n';
    }
}

template <typename Tv, typename Te>
void Graph<Tv, Te>::Floyd() {
    //寻找所有定点对得最短路径（还可以对每一个顶点dijkstra）
    //对矩阵进行操作
    vector<vector<int>> d(n);  //存放最短路径值
    vector<vector<int>> prev(n); //存放路径前直接点
    for (int i = 0; i < n; i++) {
        d[i].resize(n);
        prev[i].resize(n);
        for (int j = 0; j < n; j++) {
            d[i][j] = i==j? 0: (E[i][j] ? E[i][j]->val : 20000) ;//20000标识不存在

            prev[i][j] = E[i][j] ? i : -1;
        }

    }

    for (int k = 0; k < n; ++k) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (d[i][k]<20000&&d[k][j]<20000&&d[i][k] + d[k][j] < d[i][j]) {
                    d[i][j] = d[i][k] + d[k][j];
                    prev[i][j] = prev[k][j];
                }
            }
        }
    }
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; j++) {
            string length = (d[i][j] ==20000 ? "N" : to_string(d[i][j]) );
            cout << V[i].val << "-" << V[j].val<<":" << length << '\t';
        }
        cout << endl;
    }
    cout << "*************************\n";
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; j++) {
            cout << prev[i][j] <<'\t';
        }
        cout << endl;
    }
}


int main()
{
    Graph<char, int> A;
    //A为有向图
    for (int i = 0; i < 5; i++)
    {
        A.insertVer('a' + i);
    }
    //变为完全图
    A.insertEdge(4, 'a', 'b');
    A.insertEdge(2, 'b', 'c');
    A.insertEdge(1, 'c', 'd');
    A.insertEdge(1, 'd', 'e');
    A.insertEdge(2, 'a', 'c');
    A.insertEdge(7, 'e', 'c');


    A.kruskal();
}