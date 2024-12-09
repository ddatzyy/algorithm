
vector<int> get_Z(string &a, string &b)
{
    // get a[0..] and b[i..] max prefix
    int x, y;
    string tem = a + "$" + b;
    vector<int> ret(tem.size());
    ret[0] = tem.size();
    x = y = -1;
    for (int i = 1; i < tem.size(); i++) {
        ret[i] = ret[i - x];
        if (i < y && ret[i] < y - i)
            continue;
        x = i, y = max(y, i);
        while (y < tem.size() && tem[y] == tem[y - x])
            y++;
        ret[i] = y - x;
    }
    return ret;
}
