#include <iostream>

#define DEFAULTCAPACITY 50
using Sub = unsigned int;
template <class T>
class vector {
   private:
    T* _elem;
    unsigned int _size;
    unsigned int _capacity;
    void expand();

   public:
    vector<T>& operator=(const vector<T>& p);
    vector(const vector<T>& p);
    vector();
    vector(unsigned int n);
    ~vector();

    void push_back(const T&);
    void insert(Sub i, const T& p);
    void insert(const T& p) { insert(0, p); };
    void erase(Sub i);
    T& operator[](Sub i);
    bool operator==(const vector<T>& p);

    unsigned int size() { return _size; };
    bool empty() { return _size; };
};
//===================
template <class T>
void vector<T>::expand() {
    if (_size * 1.5 < _capacity)
        return;
    T* _newelem = new T[2 * _size];
    _capacity = 2 * _size;
    for (unsigned i = 0; i < _size; i++) {
        _newelem[i] = _elem[i];
    }
    delete[] _elem;
    _elem = _newelem;
}
//==================
template <class T>
vector<T>::vector() : _elem(nullptr), _size(0), _capacity(DEFAULTCAPACITY) {
    _elem = new T[_capacity];
}
template <class T>
vector<T>::vector(unsigned int n) : _elem(nullptr), _size(n), _capacity(2 * n) {
    _capacity = _capacity < DEFAULTCAPACITY ? DEFAULTCAPACITY : _capacity;
    _elem = new T[_capacity];
}
template <class T>
vector<T>& vector<T>::operator=(const vector<T>& p) {
    if (&p == this)
        return;
    delete _elem;
    _size = p._size;
    _capacity = p._capacity;
    _elem = new T[_capacity];
    for (unsigned int i = 0; i < _size; i++) {
        _elem[i] = p._elem[i];
    }
    return *this;
}
template <class T>
vector<T>::~vector() {
    delete[] _elem;
}
template <class T>
vector<T>::vector(const vector<T>& p) : vector<T>() {
    *this = p;
}
//==========================
template <class T>
void vector<T>::push_back(const T& v) {
    expand();
    _elem[_size++] = v;
}
template <class T>
void vector<T>::insert(Sub i, const T& p) {
    if (i > _size)
        throw std::exception();
    expand();
    for (Sub j = i; j < _size; j++) {
        _elem[j + 1] = _elem[j];
    }
    _elem[i] = p;
    _size++;
}
template <class T>
T& ::vector<T>::operator[](Sub i) {
    if (i >= _size)
        throw std::exception();
    return _elem[i];
}
template <class T>
void vector<T>::erase(Sub i) {
    if (i >= _size)
        throw std::exception();
    for (Sub j = _size - 1; j > i; j++) {
        _elem[j - 1] = _elem[j];
    }
    _size--;
}
template <class T>
bool vector<T>::operator==(const vector<T>& p) {
    if (&p == this)
        return 1;
    if (_size != p._size)
        return 0;
    for (Sub i = 0; i < _size; i++) {
        if (_elem[i] != p._elem[i])
            return 0;
    }
    return 1;
}