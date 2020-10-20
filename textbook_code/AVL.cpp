//二叉平衡树
//是二叉搜索树的一个特例
//不同之处主要在于二叉树更平衡，相应的，维护代价也有所提升

//类似于二叉树，如果结点是有向的——即没有指向父亲的，那么我们最好使用递归的去定义相关函数

#include <iostream>
using namespace std;

#define Nodep(T) Binnode<T> *

template <typename T>
struct Binnode
{
    T val;
    int height;
    Binnode<T> *lc, *rc;
    Binnode() : lc(nullptr), rc(nullptr), height(1){};
    Binnode(const T &a) : val(a), lc(nullptr), rc(nullptr), height(1){};
};

template <typename T>
class AVLTree
{
private:
    Nodep(T) _root;

    void insert(Nodep(T) & a, const T &e);
    void _clear(Nodep(T) & a);
    int height(Nodep(T) & a) { return a == nullptr ? 0 : a->height; };
    bool remove(Nodep(T) & a, const T &e);
    bool adjust(Nodep(T) & a, int subTree);

    void RR(Nodep(T) & a);
    void LL(Nodep(T) & a);
    void LR(Nodep(T) & a);
    void RL(Nodep(T) & a);

public:
    AVLTree() : _root(nullptr){};
    ~AVLTree() { _clear(_root); };
    //search
    Nodep(T) find(const T &);

    //insert
    void insert(const T &p) { insert(_root, p); };

    Nodep(T) root() { return _root; }
    //========================================================

    //remove
    void remove(const T &e) { remove(_root, e); };
};

template <typename T>
void AVLTree<T>::insert(Nodep(T) & a, const T &e)
{
    if (!a)
    {
        a = new Binnode<T>(e);
        return;
    }
    if (e > a->val)
    { //向右
        insert(a->rc, e);
        //在右子树上插入执行检查
        if (height(a->rc) - height(a->lc) == 2)
        {
            //需要调整
            if (e > a->rc->val) //右孩子的右侧
                //RR
                RR(a);
            else //右孩子左侧
                RL(a);
        }
    }
    else
    {
        insert(a->lc, e);
        //执行检查
        if (height(a->lc) - height(a->rc) == 2)
        {
            //需要调整l
            if (e < a->lc->val) //左孩子的左侧
                //RR
                LL(a);
            else //左孩子右侧
                LR(a);
        }
    }
    a->height = max(height(a->lc), height(a->rc)) + 1;
}

template <typename T>
Nodep(T) AVLTree<T>::find(const T &e)
{
    Nodep(T) tmp = _root;
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
void print(Nodep(T) p, int space = 0)
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

template <typename T>
void AVLTree<T>::_clear(Nodep(T) & a)
{
    if (!a)
        return;
    _clear(a->lc);
    _clear(a->rc);
    delete a;
    a = nullptr;
}

template <typename T>
void AVLTree<T>::RR(Nodep(T) & a)
{
    Nodep(T) tmp = a;
    a = a->rc;
    tmp->rc = a->lc;
    a->lc = tmp;
    tmp->height = max(height(tmp->lc), height(tmp->rc)) + 1;
    a->height = max(height(a->lc), height(a->rc)) + 1;
}
template <typename T>
void AVLTree<T>::LL(Nodep(T) & a)
{
    Nodep(T) tmp = a;
    a = a->lc;
    tmp->lc = a->rc;
    a->rc = tmp;
    tmp->height = max(height(tmp->lc), height(tmp->rc)) + 1;
    a->height = max(height(a->lc), height(a->rc)) + 1;
}
template <typename T>
void AVLTree<T>::LR(Nodep(T) & a)
{
    RR(a->rc);
    LL(a);
}
template <typename T>
void AVLTree<T>::RL(Nodep(T) & a)
{
    LL(a->lc);
    RR(a);
}
//remove不对！！！！
template <typename T>
bool AVLTree<T>::remove(Nodep(T) & a, const T &e)
{
    if (!a)
        return 1;
    if (a->val == e)
    { //删除该节点
        if (a->rc && a->lc)
        { //有两个孩子
            Nodep(T) tmp = a->lc;
            while (tmp->rc)
                tmp = tmp->rc;
            a->val = tmp->val;
            if (remove(a->lc, a->val)) //如果高度没变，就停止
                return 1;
            return adjust(a, 0); //否则一致向根迭代，直到找到一个根节点
        }
        else
        { //1个或者无
            Nodep(T) tmp = a;
            a = a->lc ? a->lc : a->rc;
            delete tmp;
            return 0; //返回0代表什么高度变化了
        }
    }
    if (e < a->val)
    { //左子树上删除
        if (remove(a->lc, e))
            return 1;
        return adjust(a, 0);
    }
    else
    {
        if (remove(a->rc, e))
            return 1;
        return adjust(a, 1);
    }
}

//subTree参数是啥??????????
//subTree=1 代表右子树  subTree=0 代表左子树
template <typename T>
bool AVLTree<T>::adjust(Nodep(T) & a, int subTree)
{
    if (subTree)
    { //右子树删除
        if (height(a->lc) - height(a->rc) == 1)
            return 1;

        if (height(a->lc) == height(a->rc))
        {
            --a->height;
            return 0;
        }

        if (height(a->lc->rc) > height(a->lc->lc))
        {
            LR(a);
            return 0;
        }
        LL(a);
        if (height(a->lc) == height(a->rc))
            return 0;
        return 1;
    }
    else
    { //左子树删除，左子树变矮

        //左右子树原来一样高，现在左子树矮了1层
        //该节点符合avl，且自身height不变
        if (height(a->rc) - height(a->lc) == 1)
            return 1;

        //左子树本来高一层
        //该节点符合avl，但自身height变化
        if (height(a->rc) == height(a->lc))
        {
            --a->height;
            return 0;
        }

        //左子树本来就低一层
        //不符合avl，需要调整，调整后高度变化随情况确定
        if (height(a->rc->lc) > height(a->rc->rc))
        {
            //右子树的左子树大于右子树的左子树
            RL(a);
            return 0;
        }
        RR(a);
        if (height(a->rc) == height(a->lc))
            return 0; //右边的右子树比右子树的左子树高
        else
            return 1; //本来右边左右子树一样高
    }
}

/*
递归的用法，加入以及删除时如何运用递归来很好的更新height和判断是否平衡
avl树很好的展现了递归的强大

*/

int main(){
    int i;
    AVLTree<int> A;
    while(cin>>i){
        A.insert(i);
        cout<<"====================================\n";
        print(A.root());
        cout<<"====================================\n";
    }


}
