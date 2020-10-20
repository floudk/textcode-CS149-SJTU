//简单数组实现
using Rank = unsigned int;
#define DE_CAPACITY 20
template <typename T>
class stack_arr
{
private:
    T *_elem;
    Rank _size;
    Rank _capacity;
    void expand()
    {
        if (_size * 1.5 < _capacity)
            return;
        T *old = _elem;
        _capacity <<= 1;
        _elem = new T[_capacity];
        for (Rank i = 0; i < _size; i++)
            _elem[i] = old[i];
        delete[] old;
    }

public:
    stack_arr() : _size(0), _capacity(DE_CAPACITY)
    {
        _elem = new T[_capacity];
    };
    ~stack_arr() { delete[] _elem; };

    void push(const T &p)
    {
        expand();
        _elem[_size++] = p;
    }
    T top() { return _elem[_size - 1]; };
    void pop() { _size--; }
    bool empty() { return !bool(_size); }
    Rank size() { return _size; }
};

//带尾节点的单循环链表实现
#define Node(T) node<T> *
template <typename T>
struct node
{
    T val;
    node<T> *next;
    node(const T &p, Node(T) a = nullptr) : val(p), next(a){};
    node() : next(nullptr){};
};

template <typename T>
class stack_list
{
private:
    Rank _size;
    Node(T) trail;

public:
    stack_list()
    {
        trail = new node<T>;
        trail->next = trail;
    }
    ~stack_list()
    {
        Node(T) tmp = trail->next;
        while (tmp != trail)
        {
            Node(T) a = tmp->next;
            delete tmp;
            tmp = a;
        }
        delete trail;
    }

    void push(const T &p)
    {

        trail->next = new node<T>(p, trail->next);
        _size++;
    };
    void pop()
    {
        if (trail->next == trail)
            return;
        Node(T) tmp = trail->next;
        trail->next = trail->next->next;
        delete tmp;
        _size--;
    };
    T top()
    {
        return trail->next->val;
    }
    Rank size()
    {
        return _size;
    };
    bool empty()
    {
        return !bool(_size);
    };
};