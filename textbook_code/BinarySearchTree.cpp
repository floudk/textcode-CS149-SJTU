//动态查找表
//二叉查找树，左小右大
#define Nodep(T) Binnode<T> *

template <typename T>
struct Binnode
{
    T val;
    Binnode<T> *lc, *rc;
    Binnode() : lc(nullptr), rc(nullptr){};
    Binnode(const T &a) : val(a), lc(nullptr), rc(nullptr){};
};

template <typename T>
class Binsearch
{
private:
    Nodep(T) _root;
    //_clear()递归函数辅助析构和清空
    void _clear(Nodep(T) & a);
    void insert(Nodep(T) & a, const T &e);
    void remove(Nodep(T) & a, const T &e);

public:
    Binsearch() : _root(nullptr){};
    ~Binsearch() { _clear(_root); };

    //insert
    bool insert(const T &);
    //remove
    void remove(const T &); //删除值(允许重复版本只保证删除重复中的一个)

    //search
    Nodep(T) find(const T &);

    void print(Nodep(T) p, int space = 0)
    {
        if (!p)
            return;
        space += 4;
        print(p->rc, space);
        for (int i = 4; i < space; i++)
            cout << "  ";
        cout << p->val << "\n";
        print(p->lc, space);
    }
    Nodep(T) root() { return _root; };
};

template <typename T>
void Binsearch<T>::_clear(Nodep(T) & a)
{
    if (!a)
        return;
    _clear(a->lc);
    _clear(a->rc);
    delete a;
    a = nullptr;
}

template <typename T>
Nodep(T) Binsearch<T>::find(const T &e)
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

//这里只考虑没有重复的情况，如果有重复可以考虑是用替身，递归插入替身

template <typename T>
bool Binsearch<T>::insert(const T &e)
{
    insert(_root, e);
}

template <typename T>
void Binsearch<T>::remove(const T &e)
{
    remove(_root, e);
}

template <typename T>
void Binsearch<T>::remove(Nodep(T) & a, const T &e)
{
    if (!a)
        return;
    if (e < a->val)
        remove(a->lc, e);
    else if (e > a->val)
        remove(a->rc, e);
    else if (a->rc && a->lc)
    { //被删对象有两个孩子
        //找左子树的最右结点当替身
        Nodep(T) tmp = a->lc;
        while (tmp->rc)
            tmp = tmp->rc;
        a->val = tmp->val;
        remove(a->lc, a->val);
    }
    else
    { //1个孩子或0个
        Nodep(T) tmp = a;
        a = a->rc ? a->rc : a->lc;
        delete tmp;
    }
}

template <typename T>
void Binsearch<T>::insert(Nodep(T) & a, const T &e)
{
    if (!a)
    {
        a = new Binnode<T>(e);
        return;
    }

    if (e > a->val)
    {
        insert(a->rc, e);
    }
    else
    {
        insert(a->lc, e);
    }
}