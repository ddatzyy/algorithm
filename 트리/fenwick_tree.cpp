template <typename T>
struct fenwick_tree {
    fenwick_tree(int _N) : N(_N) { seg.resize(N + 1); }
    fenwick_tree(vector<T>& arr) : N(arr.size())
    {
        seg.resize(N + 1);
        copy(arr.begin(), arr.end(), seg.begin() + 1);
    }

    void update(int lo, T va)
    {
        while (lo <= N) {
            seg[lo] += va;
            lo += lo & -lo;
        }
    }
    T find(int lo)
    {
        T ret = 0;
        while (lo > 0) {
            ret += seg[lo];
            lo -= lo & -lo;
        }
        return ret;
    }

   private:
    int N;
    vector<T> seg;
};