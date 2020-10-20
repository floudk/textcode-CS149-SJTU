#include <iostream>
#include <stack>
using namespace std;

#define Node(T) node<T> *
using Rank = unsigned int;
template <typename T>
struct node
{
    T val;
    node<T> *lc;
    node<T> *rc;
    node(const T &a) : val(a), lc(nullptr), rc(nullptr){};
};

template <typename T>
class BinTree
{
private:
    Node(T) _root;
    Rank _size;
    void clear(Node(T) p)
    {
        if (!p)
            return;
        clear(p->lc);
        clear(p->rc);
        delete p;
    }
    void _build(Node(T) & a, Node(T) p)
    {
        if (!p)
            return;
        a = new node<T>(p->val);
        _build(a->lc, p->lc);
        _build(a->rc, p->rc);
    }

public:
    BinTree() : _root(nullptr), _size(0){

                                };
    BinTree &operator=(const BinTree &p)
    {
        clear(_root);
        _size = p._size;
        if (!p._root)
            return;
        _build(_root, p->_root);
        return *this;
    };
    ~BinTree()
    {
        clear(_root);
    };
    //遍历

    //前序遍历 中前后
    void travel_pre()
    {
        stack<Node(T)> ss;
        if (!_root)
            return;
        ss.push(_root);
        Node(T) tmp;
        while (!ss.empty())
        {
            tmp = ss.top();
            ss.pop();
            if (tmp->rc)
                ss.push(tmp->rc);
            if (tmp->lc)
                ss.push(tmp->lc);
            cout << tmp->val;
        }
    }
    //中序遍历 前中后
    void travel_in()
    {
        stack<Node(T)> ss;
        if (!_root)
            return;
        Node(T) p = _root;
        while (p || !ss.empty())
        {
            while (p)
            {
                ss.push(p);
                p = p->lc;
            }
            if (!ss.empty())
            {
                p = ss.top();
                ss.pop();
                cout << p->val;
                p = p->rc;
            }
        }
    }

    void travel_post()
    {
        stack<Node(T)> ss;
        if (!_root)
            return;
        Node(T) p = _root;
        ss.push(p);
        while (!ss.empty())
        {
            if (ss.top()->rc != p && ss.top()->lc != p)
            {
                while (p = ss.top())
                {
                    if (p->rc)
                        ss.push(p->rc);
                    ss.push(p->lc);
                }
                ss.pop();
            }
            p = ss.top();
            ss.pop();
            cout << p->val;
        }
    }
    void insertAsRoot(const T &p) { _root = new node<T>(p); };
    void insertAsLc(const T &p, Node(T) x) { x->lc = new node<T>(p); };
    void insertAsRc(const T &p, Node(T) x) { x->rc = new node<T>(p); };
    Node(T) & root() { return _root; };
};