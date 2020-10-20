//插入排序
//直接插入  二分插入

//希尔排序

//选择排序
//直接选择     //堆排序

//交换排序
//冒泡    //快排

//归并排序

//基数排序
#include <iostream>
#include <vector>
using namespace std;

//=======================================
//非降序排列
//=======================================
//直接插入排序  O(N^2)  stable
template <typename T>
void straInsert(vector<T> &a)
{
    for (int i = 1; i < a.size(); i++)
    {
        //a[i]插入到 0~i-1 形成的有序队列
        T tmp = a[i];
        int j;
        for (j = i - 1; a[i] < a[j] && j >= 0; j--)
        {
            a[j + 1] = a[j];
        }
        a[j] = tmp;
    }
}
//=======================================
//二分插入排序 unstable O(N^2)
//比较次数降低为nlogn 但仍旧有移动成本O(N)
template <typename T>
void binInsert(vector<T> &a)
{
    for (int i = 1; i < a.size(); i++)
    {
        T tmp = a[i];
        int hi = i;
        int lo = 0;
        while (lo <= hi)
        {
            int mi = (lo + hi) >> 1;
            if (a[mi] < a[i])
            {
                lo = mi + 1;
            }
            else if (a[mi] > a[i])
            {
                hi = mi - 1;
            }
            else
            {
                break;
            }
            if (lo >= hi)
                break;
        }
        int j;
        for (j = i - 1; j >= lo; j--)
            a[j + 1] = a[j];
        a[j + 1] = tmp;
        /*
        cout<<i<<" times:.....\n";
        for(auto i:a)
            cout<<i<<' ';
        cout<<endl;
        */
    }
    return;
}

//====================================================
//shell 排序  较快算法中最简单的算法  O(n^1.25)左右（统计结果）  unstable

template <typename T>
void shellsort(vector<T> &a)
{
    int step, i, j;
    for (step = a.size() / 2; step > 0; step >>= 1)
    {
        for (i = step; i < a.size(); ++i)
        {
            tmp = a[i];
            for (j = i - step; j >= 0 && a[j] > tmp; j -= step)
            {
                a[j + step] = a[j];
            }
            a[j + step] = tmp;
        }
    }
}

//===========================================
//直接选择排序 unstable O(N^2)

template <typename T>
void straSele(vector<T> &a)
{
    for (int i = a.size() - 1; i >= 0; i--)
    {
        //找到0~i中最大的元素
        int tmpmax = i;
        for (int j = i - 1; j >= 0; --j)
        {
            if (a[j] > a[tmpmax])
            {
                tmpmax = j;
            }
            swap(a[tmpmax], a[i]);
        }
    }
}

//============================================
//堆排序 O(NlogN)  unstable
template <typename T>
void heap(vector<T> &a)
{
    //建大顶堆
    // 2i+1 2i+2 ----- i-1 /2   i-2  /2
    int _size = a.size();
    auto swim = [&](int i) -> void {
        while (i >= 0)
        {
            if ((i - 1) / 2 > 0 && a[(i - 1) / 2] < a[i])
            {
                swap(a[(i - 1) / 2], a[i]);
                i = (i - 1) / 2;
            }
            else
            {
                break;
            }
        }
    };
    auto sink = [&](int i) -> void {
        while (2 * i + 1 < _size)
        {
            int lar = 2 * i + 1;
            if (lar + 1 < _size && a[lar + 1] > a[lar])
            {
                ++lar;
            }
            if (a[lar] > a[i])
            {
                swap(a[lar], a[i]);
                i = lar;
            }
            else
            {
                break;
            }
        }
    };
    //建堆
    for (int i = _size / 2; i >= 0; i--)
    {
        sink(i);
    }
    //不断取出最大值
    int idx = _size - 1;
    for (; idx > 0; --idx)
    {
        swap(a[idx], a[0]);
        --_size;
        sink(0);
    }
}

//=================================
//冒泡排序 O(N^2)  stable

template <typename T>
void Bubble(vector<T> &a)
{
    for (int i = a.size() - 1; i > 0; --i)
    {
        for (int j = 0; j < i; j++)
        {
            if (a[j] > a[j + 1])
                swap(a[j], a[j + 1]);
        }
    }
}

//=======================================
//快速排序  nlogn  unstable
template <typename T>
int partition(vector<T> &a, int lo, int hi)
{
    swap(a[lo], a[rand() % (hi - lo) + lo]);
    --hi;
    T tmp = a[lo];
    while (lo < hi)
    {
        while (lo < hi && a[hi] > tmp)
            --hi;
        if (hi > lo)
        {
            a[lo++] = a[hi];
        }
        while (lo < hi && a[lo] <= tmp)
            ++lo;
        if (lo < hi)
            a[hi--] = a[lo];
    }
    a[lo] = tmp;

    return lo;
}

template <typename T>
void Quicksort(vector<T> &a, int lo, int hi)
{
    if (lo + 1 >= hi)
    {
        return;
    }
    int mi = partition(a, lo, hi);
    Quicksort(a, lo, mi);
    Quicksort(a, mi, hi);
}

//========================
//归并排序 nlogn 最好最坏平均都是

template <typename T>
void Mergesort(vector<T> &a, int lo, int mi, int hi)
{
    //hi是上届不参与归并
    int i = lo, j = mi;
    //原地归并也可以实现，这里不原地
    vector<T> tmp(hi - lo);
    int k = 0;
    while(i < mi && j < hi)
    {
        if (a[i] < a[j])
        {
            tmp[k++] = a[i++];
        }
        else
        {
            tmp[k++] = a[j++];
        }
    }
    while (i < mi)
    {
        tmp[k++] = a[i++];
    }
    while (j < hi)
    {
        tmp[k++] = a[j++];
    }
    for (int m = lo; m < hi;m++){
        a[m] = tmp[m - lo];
    }
}

template <typename T>
void Mergesort(vector<T> &a, int lo, int hi)
{
    if (lo + 1 >= hi)
    {
        return;
    }
    int mi = (hi + lo) >> 1;
    Mergesort(a, lo, mi);
    Mergesort(a, mi, hi);
    Merge(a, lo, mi, hi);
}

//====================================
//基数排序——属于桶排序的一种
//针对int类型
void BaseSort(vector<int> &a)
{
    int maxdit = 0;
    for(auto i :a){
        maxdit = maxdit > (1 + i / 10) ? maxdit : (1 + i / 10);
    }
    vector<vector<int>> bucks(10);
    for (int i = 0; i < maxdit;i++){
        int base = (i+1) * 10;
        for(auto item:a){
            bucks[item / base].push_back(item);
        }
        a.clear();
        for(auto buck:bucks){
            for(auto elem:buck){
                a.push_back(elem);
            }
        }
        bucks.clear();
        bucks.resize(10);
    }
}

