//a RED-BLACK TREE Implementation

#include<iostream>
using std::swap;
using std::cin;
using std::cout;

#define Nodep(T) Binnode<T> *
#define RED 0
#define BLACK 1
template <typename T>
struct Binnode
{
    T val;
    Binnode<T> *lc, *rc;
    bool col;
    Binnode() : lc(nullptr), rc(nullptr), col(RED){};
    Binnode(const T &a) : val(a), lc(nullptr), rc(nullptr), col(RED){};
};

template <typename T>
class RBTree
{
private:
    Nodep(T) _root;
    void clear(Nodep(T) &);
    void insert(Nodep(T) &, const T &);
    void insertAdjust(Nodep(T) &, Nodep(T) &, Nodep(T) &);
    void removeAdjust(Nodep(T) & p, Nodep(T) & c, Nodep(T) & t, const T &e);
    void RR(Nodep(T) &);
    void LL(Nodep(T) &);
    void RL(Nodep(T) &);
    void LR(Nodep(T) &);

public:
    RBTree();
    ~RBTree();
    Nodep(T) search(const T &);
    void insert(const T &);
    void remove(const T &);

    Nodep(T) getroot() { return _root; };
};

//==============================================================
//==============================================================
//public functions implementation

template <typename T>
RBTree<T>::RBTree() : _root(nullptr){};

template <typename T>
RBTree<T>::~RBTree()
{
    clear(_root);
}

template <typename T>
Nodep(T) RBTree<T>::search(const T &e)
{
    if (!_root)
        return nullptr;
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
        { // ==e
            return tmp;
        }
    }
    return nullptr;
}

template <typename T>
void RBTree<T>::insert(const T &e)
{

/*
*红黑树的插入只有以下几种情况
*1.插入不能作为孙子进行讨论
    *a.插入根——设为黑色
    *b.插入作为根的孩子——红色
*2.插入可以作为孙子进行讨论
    *a.父亲是黑色
        不用做什么，原红黑树符合结构，父亲是黑色，孙子是红色，依旧合法
    *b.父亲是红色（且未添加孙子前红黑树合法）
        *I.父亲的兄弟节点为黑色（空结点被认为是黑色
            此时爷爷结点必然是黑色的（否则连续红）
            此时观察孙子相对于爷爷在内测还是外侧
            若在外侧，执行一次旋转必然能变成父亲在中间的三角形态，而后重新着色
            若在内测，执行两次旋转应该能达到相同的情形
        *II.父亲的兄弟节点为红色
            直接父亲和父亲兄弟和爷爷互换颜色，但爷爷的颜色有可能出现非法
            需要进行递归检查
*综上,应该尽量让父亲变黑，至少尽量让父亲的兄弟变黑，从而不用递归检查
*/
    if (!_root)
    {
        _root = new Binnode<T>(e);
        _root->col = BLACK;
        return;
    }

    Nodep(T) t;
    Nodep(T) parent;
    Nodep(T) graparent;
    t = parent = graparent = _root;
    while (1)
    {
        if (t)
        {
            if (t->lc && t->lc->col == RED && t->rc && t->rc->col == RED)
            {
                /*
                通过自上而下的这样一个步骤
                最大可能的保证调整的简单化，不停的自上而下调换
                能够保证插入时，父亲要么为黑，要么父亲兄弟为黑
                不会出现需要自下而上一直向上递归检查的情况
                */
                t->col = RED;
                t->lc->col = t->rc->col = BLACK;
                insertAdjust(graparent, parent, t);
            }
            graparent = parent;
            parent = t;
            t = (t->val > e ? t->lc : t->rc);
        }
        else
        {
            t = new Binnode<T>(e);
            if (e < parent->val)
                parent->lc = t;
            else
                parent->rc = t;
            insertAdjust(graparent, parent, t);
            _root->col = BLACK;
            return;
        }
    }
}
template <typename T>
void RBTree<T>::remove(const T &e){

    Nodep(T) c;
    Nodep(T) p;
    Nodep(T) t;//c--当前结点 p——父亲节点  t——兄弟结点
    if(!_root)
        return;
    //只有根节点并删除根节点
    T del = e;

    if(_root->val==del&&_root->lc==nullptr&&_root->rc==nullptr){
        delete _root;
        _root = nullptr;
        return;
    }

    p = c = t = _root;
    while(1){
        removeAdjust(p, c, t, e);
        //递归执行检查
        //保证c是被删除结点
        //同时保证被删除点是红色的
        if(c->val==del&&c->lc&&c->rc){
            //找到该点，并且有两个孩子
            //找替身
            Nodep(T) tmp = c->lc;
            while(tmp->rc)
                tmp = tmp->rc;
            c->val = tmp->val;
            del = tmp->val;
            p = c;
            c = c->lc;
            t = p->rc;
            continue;
        }

        if(c->val==del){
        //找到该点，并且只有1个孩子或者没孩子
        //通过前置手段保证了
            delete c;
            if(p->lc==c)
                p->lc = nullptr;
            else
                p->rc = nullptr;

            _root->col = BLACK;
            return;
        }

        p = c;
        c = (del < p->val ? p->lc : p->rc);
        t = (c == p->lc ? p->rc : p->lc);
    }
}


//==============================================================
//==============================================================
//private functions implementation
template <typename T>
void RBTree<T>::clear(Nodep(T) & p)
{
    if (!p)
        return;
    clear(p->lc);
    clear(p->rc);
    delete p;
    return;
}

template <typename T>
void RBTree<T>::insertAdjust(Nodep(T) & gp, Nodep(T) & p, Nodep(T) & t)
{
    //check after having been inserted
    if (p->col == BLACK) //No continuous RED
        return;
    if (p == _root) //if(p==_root)
    {
        p->col = BLACK;
        return;
    }

    //when program runs here
    //it guarantees there is at least 3 generations
    //and middle generation is RED
    if (gp->lc == p)
    {
        if (p->lc == t)
            LL(gp);
        else
            LR(gp);
    }
    else
    {
        if (p->rc == t)
            RR(gp);
        else
            RL(gp);
    }
}
template <typename T>
void RBTree<T>:: RR(Nodep(T) &p){
//此时有且p点作为爷爷黑色，右儿子红色，左儿子黑色，右孙子红色
    Nodep(T) tmp = p->rc;
    p->rc = p->rc->rc;
    tmp->rc = tmp->lc;
    tmp->lc = p->lc;
    p->lc = tmp;
    swap(tmp->val, p->val);
}
template <typename T>
void RBTree<T>:: LL(Nodep(T) &p){
    Nodep(T) tmp = p->lc;
    p->lc = p->lc->lc;
    tmp->lc = tmp->rc;
    tmp->rc = p->rc;
    p->rc = tmp;
    swap(tmp->val, p->val);
}
template <typename T>
void RBTree<T>::RL(Nodep(T) &p){
    Nodep(T) fa = p->rc;
    Nodep(T) gson = p->rc->lc;
    fa->lc = gson->rc;
    gson->rc = p->lc;
    p->lc = gson;
    swap(gson->val, p->val);
}
template <typename T>
void RBTree<T>::LR(Nodep(T) &p){
    Nodep(T) fa = p->lc;
    Nodep(T) gson = p->lc->rc;
    fa->rc = gson->lc;
    gson->lc = p->rc;
    p->rc = gson;
    swap(gson->val, p->val);
}

template <typename T>
void RBTree<T>::removeAdjust(Nodep(T) &p, Nodep(T) &c,Nodep(T) &t,const T &e){
    //c为当前结点，p为父结点，t为c的兄弟结点
    //调整使c变成红色结点
    T del=e;
   if(c->col==RED)//删除结点为红色，直接删除
       return;

    //如果是根节点BLAC
    if(c==_root){
        if(c->lc&&c->rc&&c->rc->col==c->lc->col){
            c->col = RED;
            c->lc->col = c->rc->col = BLACK;
            return;
        }
    }

    //c不是根节点 或者是  根节点但不存在两个儿子或两个儿子颜色不同


    if((c->lc&&c->lc->col==BLACK||c->lc==nullptr)&&(c->rc&&c->rc->col==BLACK||c->rc==nullptr)){
        //两个儿子颜色都为黑 或者 两个儿子都不存在
        if((t->lc&&t->lc->col==BLACK||t->lc==nullptr)&&(t->rc&&t->rc->col==BLACK||t->rc==nullptr)){
            //兄弟两个儿子都为黑或者两个儿子都不存在
            p->col = BLACK;
            c->lc->col = c->rc->col = RED;
            return;
        }else{
            //兄弟有红孩子
            //开始转
            if(p->lc==t){
                //如果兄弟是左孩子
                if(t->lc&&t->lc->col==RED){
                    //左兄弟左孩子红
                    t->lc->col = BLACK;
                    LL(p);
                    p = t;
                }else{
                    //左兄弟右孩子红
                    LR(p);
                    p = p->rc;
                    p->col = BLACK;
                }

            }else if(t->rc&&t->rc->col==RED){
                //兄弟是右孩子 且右孩子红
                t->rc->col = BLACK;
                RR(p);
                p = t;
            }else{
                RL(p);
                p = p->lc;
                p->col = BLACK;
            }
            c->col = RED;
        }

    }else{
        //自己两个孩子中有红
        if(c->val==del){
            //c刚好是这个点
            if(c->lc&&c->rc){
                //两个孩子都存在
                if(c->rc->col==BLACK){
                    LL(c);
                    c = c->rc;
                }
                return;
            }


            if(c->lc){
                //左孩子存在
                LL(c);
                p = c;
                c = c->rc;
            }else{
                //右孩子存在
                RR(c);
                p = c;
                c = c->lc;
            }
        }else{
            //c不是这个点
            p = c;
            c = (del < p->val ? p->lc : p->rc);
            t = (c == p->lc ? p->rc : p->lc);
            if(c->col==BLACK){
                if(t==p->rc)
                    RR(p);
                else
                    LL(p);
                p = t;
                t = (c == p->lc ? p->rc : p->lc);
                removeAdjust(p, c, t, del);
            }

        }
    }
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




//=====================================================

int main(){
    int i;
    char w;
    RBTree<int> A;
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
