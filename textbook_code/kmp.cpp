#include <iostream>
#include <string>
#include <vector>
using namespace std;

void getnext(vector<int> &next, const string &pattern)
{
    next.resize(pattern.size());
    next[0] = -1;
    int k = -1, j = 0;
    while (j < next.size() - 1)
    {
        if (k == -1 || pattern[j] == pattern[i])
        {
            ++k;
            ++j;
            if (pattern[j] != pattern[k])
                next[j] = k;
            else
                next[j] = next[k];
        }
        else
        {
            k = next[k];
        }
    }
}

int KMP(const string &pattern, const string &target)
{
    vector<int> next;
    getnext(next, pattern);
    int i = 0, j = 0;
    int tl = target.size();
    int pl = pattern.size();
    while (i < tl && j < pl)
    {
        if (j == -1 || pattern[j] = target[i])
        {
            i++;
            j++;
        }
        else
        {
            j = next[j];
        }
    }
    if (j == pl)
        return i - j;
    else
        return -1;
}
