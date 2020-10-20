/*
一个好的hsah表的实现依赖于两部分： 好的散列函数 + 好的冲突解决策略
散列函数：
    常见的有 除留余数  平方取中  取固定位数 数字分析  折叠   直接定址

冲突解决：
    常见的有 拉链法（开散列表）  线性探测  二次探测  再次散列

但具体的散列实现是需要对具体存储数据进行分析的，
这里，针对int型数据采用开散列法进行实现 散列函数是除留余数
*/
#include<iostream>
using namespace std;

#define default_size 71

class hashtable{
private:
    struct node{
        int val;
        node *next;
        node(int a=-77777, node *b = nullptr) : val(a), next(b){};

    };
    node **_elem;
    unsigned int _size;
    int _capacity;

public:
    hashtable(int a=default_size):_capacity(a){
        _elem = new node* [a];
        for(int i=0;i<_capacity;i++)
            _elem[i]=new node;
    }
    ~hashtable(){
        for (int i = 0; i < _capacity;i++){
            node *tmp = _elem[i];
            while(tmp->next){
                node *a = tmp->next;
                tmp->next = a->next;
                delete a;
            }
        }
        delete[] _elem;
    }
    void insert(const int e){

        int hashcode = e % _capacity;

        node *tmp = _elem[hashcode];



        node *newnode = new node(e);


        newnode->next = tmp->next;

        tmp->next = newnode;

    }
    void remove(const int e){
        int hashcode = e % _capacity;
        node *tmp = _elem[hashcode];
        while(tmp->next){
            if(tmp->next->val==e){
                node *p = tmp->next;
                tmp->next = p->next;
                delete p;
                return;
            }
        }
    }
    bool find(const int e){
        int hashcode = e % _capacity;
        node *tmp = _elem[hashcode];
        while(tmp->next){
            if(tmp->next->val==e){
                node *p = tmp->next;
                tmp->next = p->next;
                p->next = _elem[hashcode]->next;
                _elem[hashcode]->next = p;
                return 1;
            }
            tmp=tmp->next;
        }
        return 0;
    }
};
//开散列表还使用了类似splaytree的一些想法
