//带头尾结点的双向链表

#define Node(T) node<T>*
using Rank = unsigned int;
template <typename T>
struct node {
    T val;
    Node(T) prev;
    Node(T) succ;
    node(const T& p, Node(T) a = nullptr, Node(T) b = nullptr)
        : val(p), prev(a), succ(b){};
    node(){};
};

template <typename T>
class list {
   private:
    Node(T) head;
    Node(T) trail;
    Rank _size;
    void initial() {
        head = new node<T>;
        trail = new node<T>;
        head->succ = trail;
        trail->prev = head;
        _size = 0;
    }
    void clear() {
        Node(T) p = head->succ;
        Node(T) tmp;
        while (p != trail) {
            tmp = p->succ;
            delete p;
            p = tmp;
        }
        head->succ = trail;
        trail->prev = head;
    }

   public:
    list();
    ~list();
    list(const list<T>&);
    list<T>& operator=(const list<T>&);

    void insert_after(Node(T), const T&);
    void insert_before(Node(T), const T&);
    void erase(Node(T));

    Rank size() { return _size; };
    bool empty() { return _size; };
};

//====================
template <typename T>
list<T>::list() {
    initial();
}

template <typename T>
list<T>::list(const list<T>& p) {
    initial();
    *this = p;
}

template <typename T>
list<T>& list<T>::operator=(const list<T>& p) {
    if (&p == this)
        return *this;
    clear();
    _size = p._size;
    Node(T) local = head;
    Node(T) tmp = p.head->succ;
    while (tmp != p.trail) {
        insert_before(trail, tmp->val);
        tmp = tmp->succ;
    }
    return *this;
}

template <typename T>
list<T>::~list() {
    clear();
    delete head;
    delete trail;
    head = trail = nullptr;
}

//====================
template <typename T>
void list<T>::insert_after(Node(T) x, const T& p) {
    Node(T) tmp = new node<T>(p, x, x->succ);
    x->succ->prev = tmp;
    x->succ = tmp;
    _size++;
}

template <typename T>
void list<T>::insert_before(Node(T) x, const T& p) {
    Node(T) tmp = new node<T>(p, x->prev, x);
    x->prev->succ = tmp;
    x->prev = tmp;
    _size++;
}

template <typename T>
void list<T>::erase(Node(T) x) {
    x->prev->succ = x->succ;
    delete x;
    _size--;
}