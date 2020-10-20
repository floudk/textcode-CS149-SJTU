//无序表查找——大英博物馆算法

//有序表查找

//顺序查找
//插值查找——分布要服从某种特定模型
//分块查找——先建立索引表
//二分查找
//假定vector已经有序

void binsearch(vector<int> &b, int val, int lo, int hi, int &res)
{ //左闭右开
    if (lo + 1 > hi)
        return;
    int mi = (lo + hi) >> 1;
    if (val > b[mi])
        binsearch(b, val, mi + 1, hi, res);
    else if (val < b[mi])
        binsearch(b, val, lo, mi, res);
    else
    {
        res = mi;
        return;
    }
}

int Binsearch(vector<int> &b, int val)
{
    int res = -1;
    binsearch(b, val, 0, b.size(), res);
    return res;
}

//以上是递归版本，转换为迭代版本也是比较的方便的
