#include <iostream>
using namespace std;
using std::swap;
using std::cin;
using std::cout;
#define Node(T) node<T> *

template <typename T>
struct node
{
    T val;
    node<T> *lc;
    node<T> *rc;
    int level;
    node(const T &a) : val(a), lc(nullptr), rc(nullptr),level(1){};
};

template <typename T>
class AATree
{
private:
    Node(T) _root;

    void clear(Node(T) & p);
    void insert(Node(T) &, const T &);
    void remove(Node(T) &, const T &);

    void LL(Node(T) &);
    void RR(Node(T) &);

public:
    AATree();
    ~AATree();

    void insert(const T &);
    void remove(const T &);
    Node(T) search(const T &);
    Node(T) getroot(){return _root;};
};

template <typename T>
AATree<T>::AATree()
{

    _root = nullptr;
}

template <typename T>
AATree<T>::~AATree()
{

    clear(_root);
    _root = nullptr;
}

template <typename T>
Node(T) AATree<T>::search(const T &e)
{
    Node(T) tmp = _root;
    while (tmp)
    {
        if (tmp->val > e)
        {
            tmp = tmp->lc;
        }
        else if (tmp->val < e)
        {
            tmp = tmp->rc;
        }
        else
        {
            return tmp;
        }
    }
    return tmp;
}

template <typename T>
void AATree<T>::insert(const T &e){
    insert(_root, e);
}
template <typename T>
void AATree<T>::remove(const T &e){
    remove(_root, e);
}



//==============================
template <typename T>
void AATree<T>::clear(Node(T) &p)
{
    if (!p)
        return;
    clear(p->lc);
    clear(p->rc);
    delete p;
}

template <typename T>
void AATree<T>::insert(Node(T) &p, const T &e){
    if(!p){
        p = new node<T>(e);
    }else if(p->val < e)
        insert(p->rc, e);
    else if(p->val > e)
        insert(p->lc, e);
    else
        return;
    LL(p);
    RR(p);
}

template <typename T>
void AATree<T>::remove(Node(T) &p, const T &e){
    if(!p)
        return;
    if(e< p->val){
        remove(p->lc, e);
    }else if(e > p->val){
        remove(p->rc, e);
    }else{
        //撞上了
        if(p->lc&&p->rc){
            //两个孩子,找替身
            Node(T) tmp = p->lc;
            while(tmp->rc)
                tmp = tmp->rc;
            p->val = tmp->val;
            remove(p->lc, p->val);
        }else{
            //1个或无
            Node(T) tmp = p;
            p = p->lc ? p->lc : p->rc;
            delete tmp;
            return;
        }
   }
    //递归调整父结点level，以及调整可能失衡的树

    if(p->lc==nullptr||p->rc==nullptr){
        //一个孩子的结点必然是水平右链即最低级
        p->level = 1;
    }else{
        p->level = min(p->lc->level, p->rc->level) + 1;
    }

    //AAtree 归根结底是一颗比较特殊的红黑树，即红孩子只能出现在右边，并由此引入层级的概念
    //那么再删除中，必然有两种可能删除的情形，删除p的左孩子和删除p的右孩子
        //如果删除p的右孩子，如果是水平右孩子，则无需调整
        //如果是更低一层的右孩子
        //如果删除左孩子，也必然是更低一层的

        //那么即出现孩子的层级和当前结点的层级差为2，必须重新调整平衡
        //先降低父结点和其水平右结点的层级，然后再调整水平左链和连续水平右链

    if(p->rc&&p->rc->level>p->level){
        //父结点已经降级了，对父结点的水平右孩子降级
        p->rc->level = p->level;
    }

    //父结点如果没降级，删除的一定是底层的水平右链，LL左孩子为空，无作用
    //父结点降级，如果有左孩子，必然有水平左链，否则为空

    //执行3次LL和2次RR，是因为父辈降级后最复杂的情况下
    //需要三次LL和两次RR才能恢复平衡

    //大多是时候并不需要，当然，RR 与 LL是有条件的
    //当条件不满足的时候，相当于空语句，所以列出最复杂的情形即可
    LL(p);
    if(p->rc)
        LL(p->rc);
    if(p->rc&&p->rc->rc)
        LL(p->rc->rc);
    RR(p);
    if(p->rc)
        RR(p->rc);
}

template <typename T>
void AATree<T>::LL(Node(T) &p){
    //处理水平左链
    //同时出现优先处理水平左链
    if(p->lc&&p->lc->level==p->level){
        Node(T) tmp = p;
        p = p->lc;
        tmp->lc = p->rc;
        p->rc = tmp;
    }
}


template <typename T>
void AATree<T>::RR(Node(T) &p){
    //处理连续右链
    if(p->rc&&p->rc->rc&&p->rc->rc->level==p->level){
        Node(T) tmp = p->rc;
        p->rc = tmp->lc;
        tmp->lc = p;
        p = tmp;
        p->level++;
    }
}

template <typename T>
void print(Node(T) p, int space = 0)
{
    if (!p)
        return;
    space += 3;
    print(p->rc, space);
    for (int i = 3; i < space; i++)
        cout << "  ";
    cout << p->val << "\n";
    print(p->lc, space);
}




//=====================================================

int main(){
    int i;
    char w;
    AATree<int> A;
    while(cin>>w){
        cin>>i;
        if(w=='i'){
            A.insert(i);
            cout<<"====================================\n";
            print(A.getroot());
            cout<<"====================================\n";
        }
        else{
             A.remove(i);
            cout<<"====================================\n";
            print(A.getroot());
            cout<<"====================================\n";
        }
    }


}
