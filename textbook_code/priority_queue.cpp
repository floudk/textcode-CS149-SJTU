//大顶堆
#include <iostream>
#include <vector>
using namespace std;

#define DEFAULE_CA 60
template <typename T>
class priority_queue
{
private:
    T *_elem;
    int _size; //注意下标从1开始
    int capacity;

    void swim(int i)
    {

        while (i > 1)
        {
            if (_elem[i] > _elem[i / 2])
            {
                swap(_elem[i], _elem[i / 2]);
                i = i / 2;
            }
            else
                break;
        }
    }
    void sink(int i)
    {
        while (i < _size)
        {
            if (2 * i <= _size)
            {
                int max = 2 * i;
                if (max + 1 <= _size && _elem[max + 1] > _elem[max])
                    ++max;
                if (_elem[max] > _elem[i])
                {
                    swap(_elem[i], _elem[max]);
                    i = max;
                }
                else
                    break;
            }
            else
            {
                break;
            }
        }
    }

    void swap(T &a, T &b)
    {
        T tmp = b;
        b = a;
        a = tmp;
        return;
    }

public:
    void build()
    {
        for (int i = size / 2; i >= 1; --i)
            sink(i);
    }
    priority_queue(int a = DEFAULE_CA)
    {
        _size = 0;
        capacity = a;
        _elem = new T[capacity];
    };
    priority_queue(const vector<T> &p)
    {
        _size = p.size();
        build(p);
        capacity = _size * 2;
        _elem = new T[capacity];
        for (int i = 0; i < _size; i++)
        {
            _elem[i] = p[i];
        }
        build();
    }
    ~priority_queue()
    {
        delete[] _elem;
    };
    void push(const T &p)
    {
        if (_size >= capacity)
            exit(-1);
        _elem[++_size] = p;
        swim(_size);
    };
    T top()
    {
        return _elem[1];
    };
    void pop()
    {
        _elem[1] = _elem[_size--];
        sink(1);
    }
    int size()
    {
        return _size;
    }
    bool empty()
    {
        return _size == 0;
    }
};
