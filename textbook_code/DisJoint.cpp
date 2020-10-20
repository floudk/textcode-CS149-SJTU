//并查集
//union & find

#include<iostream>

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
/*
int Disjoint::find(int a) {

    int root_a = a;
    while (joint[root_a] != -1)
        root_a = joint[root_a];
    return root_a;
}
void Disjoint::union1(int a, int b)
{
    //第一个版本，朴素拼接，直接拼接两数的根
    if (a == b)
        return;
    int root_a = a;
    int root_b = b;
    while (joint[root_a] != -1)
        root_a = joint[root_a];
    while (joint[root_b] != -1)
        root_b = joint[root_b];

    if (root_a == root_b)
        return;
    joint[root_b] = root_a;
};
*/
/*
以上并查集存在一个比较大的问题，就是说并查集的树形结构很可能会退化为线性结构，从而使速度减慢，
改进方法是，根节点存储size，按规模进行并,为了配合-1作为搜索终点（其实其它实现也可）
size用负数记录 -1 ---> 1
小树接在大树上，尽可能不退化
*/
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
    if(joint[root_b]>joint[root_a]){
        //a的size更大
        joint[root_a] += joint[root_b];
        joint[root_b] = root_a;
    }else{
        joint[root_b] += joint[root_a];
        joint[root_a] = root_b;
    }
};