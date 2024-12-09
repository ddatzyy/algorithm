
vector<int> get_manacher(string &a)
{
    int n, m, k;
    string str;
    str.push_back('$');
    for (auto ne : a)
        str.push_back(ne), str.push_back('$');
    m = str.size();
    vector<int> tem(m);
    for (int i = 1, n = 0; i < m; i++) {
        tem[i] = i <= n + tem[n] ? min(n + tem[n] - i, tem[2 * n - i]) : 0;
        for (int p = tem[i] + 1; i - p >= 0 && i + p < m && str[i - p] == str[i + p]; p++)
            tem[i]++;
        if (i + tem[i] > n + tem[n])
            n = i;
    }
    vector<int> ret;
    for (int i = 1; i < m; i += 2)
        ret.push_back(tem[i]);
    return ret;
}
