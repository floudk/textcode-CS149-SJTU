//伸展树，每次查询将该结点伸展到根结点
//核心在于伸展操作

#include <iostream>
using namespace std;
using std::swap;
using std::cin;
using std::cout;
#define Nodep(T) Node<T> *

template <typename T>
struct Node
{
    T val;
    Node<T> *lc;
    Node<T> *rc;
    Node(const T &a) : val(a), lc(nullptr), rc(nullptr){};
};
template <typename T>
class SPTree{
private:
    Nodep(T) _root;
    void clear(Nodep(T) & p);

    void splay(const T &e, Nodep(T) & t);
    void RR(Nodep(T) &);
    void LL(Nodep(T) &);
    void LR(Nodep(T) &);
    void RL(Nodep(T) &);
    void R(Nodep(T) &);
    void L(Nodep(T) &);

public:
    SPTree() { _root = nullptr; };
    ~SPTree() { clear(_root); };

    Nodep(T) find(const T &);
    void insert(const T &);
    void remove(const T &);
    Nodep(T) getroot() { return _root; };
};


template <typename T>
void SPTree<T>::insert(const T &e){
    if(_root==nullptr){
        _root = new Node<T>(e);
        return;
    }
    Nodep(T) tmp = _root;
    while(1){
        if(tmp->val > e){
            if(tmp->lc){
                tmp=tmp->lc;
            }else{
                tmp->lc=new Node<T> (e);
                break;
            }
        }else{
            if(tmp->rc){
                tmp=tmp->rc;
            }else{
                tmp->rc=new Node<T> (e);
                break;
            }
        }
    }
    splay(e, _root);
}

template <typename T>
void SPTree<T>::remove(const T &e){
    splay(e, _root);
    if(_root->val!=e)
        return;
    if(_root->rc||_root->lc){
        Nodep(T) tmp;
        if(_root->lc){
            tmp = _root->lc;
            while(tmp->rc)
                tmp = tmp->rc;
            splay(tmp->val, _root->lc);
            //一定无右孩子
            tmp = _root->lc;
            tmp->rc = _root->rc;
            delete _root;
            _root = tmp;
        }else
        {
            tmp = _root->rc;
            while(tmp->lc)
                tmp = tmp->lc;
            splay(tmp->val, _root->rc);
            tmp = _root->rc;
            tmp->lc = _root->lc;
            delete _root;
            _root = tmp;
        }

    }else{
        delete _root;
        _root = nullptr;
    }
}
template <typename T>
Nodep(T) SPTree<T>::find(const T &e){
    splay(e, _root);
    return _root;
}

//=========================================================================
//=========================================================================
template <typename T>
void SPTree<T>::clear(Nodep(T) &p)
{
    if (!p)
        return;
    clear(p->lc);
    clear(p->rc);
    delete p;
}


template <typename T>
void SPTree<T>::splay(const T &e, Nodep(T) & t){
    //这要涉及到查询的都需要伸展的操作
    if(!t||t->val==e)
        return;

    if(t->val > e){
        if(t->lc==nullptr)
            return;

        if(t->lc->val > e){
            //左侧的左侧
            splay(e, t->lc->lc);
            LL(t);
        }else if(t->lc->val < e){
            //左侧的右侧
            splay(e, t->lc->rc);
            LR(t);
        }else{
            L(t);
        }

    }else if(t->val < e){
        if(t->rc==nullptr)
            return;

        if(t->rc->val > e){
            //右侧的左侧
            splay(e, t->rc->lc);
            RL(t);
        }else if(t->rc->val < e){
            //右侧的右侧
            splay(e, t->rc->rc);
            RR(t);
        }else{
            R(t);
        }
    }

}

template <typename T>
void SPTree<T>::RR(Nodep(T) &p){
    R(p->rc);
    R(p);
}
template <typename T>
void SPTree<T>::LL(Nodep(T) &p){
    L(p->lc);
    L(p);
}

template <typename T>
void SPTree<T>::LR(Nodep(T) &p){
    R(p->lc);
    L(p);
}
template <typename T>
void SPTree<T>::RL(Nodep(T) &p){
    L(p->rc);
    R(p);
}

template <typename T>
void SPTree<T>::R(Nodep(T) &p){
    Nodep(T) tmp = p->rc;
    if(tmp==nullptr)
        return;
    p->rc = tmp->lc;
    tmp->lc = p;
    p = tmp;
}
template <typename T>
void SPTree<T>::L(Nodep(T) &p){
    Nodep(T) tmp = p->lc;
    if(tmp==nullptr)
        return;
    p->lc = tmp->rc;
    tmp->rc = p;
    p = tmp;
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
    SPTree<int> A;
    while(cin>>w){
        cin>>i;
        if(w=='i'){
            A.insert(i);
            cout<<"====================================\n";
            print(A.getroot());
            cout<<"====================================\n";
        }else if(w=='f'){
            A.find(i);
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










