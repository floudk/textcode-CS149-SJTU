//固定长度的数组实现
using Rank = unsigned int;
#define DE_CAPACITY 30
template <typename T>
class queue_arr
{
private:
    T *_elem;
    Rank top, bottom;
    Rank _capacity;

public:
    queue_arr() : top(0), bottom(0), _capacity(DE_CAPACITY)
    {
        _elem = new T[_capacity];
    };
    ~queue_arr() { delete[] _elem; };

    bool push(const T &p)
    {

        if ((bottom + 1) % _capacity == top % _capacity)
            return 0; //队列已满，加入失败
        _elem[(++bottom) % _capacity] = p;
        return 1;
    }
    T front() { return _elem[(top + 1) % _capacity]; };
    void pop() { top++; }
    bool empty() { return bottom == top; }
    Rank size() { return bottom - top; }
};

//带头尾节点的单链表实现
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
class queue_list
{
private:
    Rank _size;
    Node(T) head;
    Node(T) trail;

public:
    queue_list()
    {
        head = new node<T>;
        trail = new node<T>;
        head->next = trail;
        _size = 0;
    }
    ~queue_list()
    {
        Node(T) tmp = head;
        while (tmp)
        {
            Node(T) a = tmp->next;
            delete tmp;
            tmp = a;
        }
    }

    void push(const T &p)
    {

        trail->next = new node<T>;
        trail->val = p;
        trail = trail->next;
        _size++;
    };
    void pop()
    {
        if (head->next == trail)
            return;
        Node(T) tmp = head->next;
        head->next = head->next->next;
        delete tmp;
        _size--;
    };
    T front()
    {
        return head->next->val;
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