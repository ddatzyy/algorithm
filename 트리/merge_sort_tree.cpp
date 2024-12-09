template <typename T>
struct merge_tree {
    void push(vector<T> &tem_arr)
    {
        int i, de;
        _arr = tem_arr;
        _size = _arr.size();
        i = 1, de = 1;
        while (i < _size)
            i *= 2, de++;
        _seg_tree.assign(de, vector<T>(_size));
        make(0, 0, _size);
    }
    void clear()
    {
        int i;
        _size = 0;
        _arr.clear();
        for (i = 0; i < _seg_tree.size(); i++)
            _seg_tree[i].clear();
    }
    bool find(int st, int en, T va)
    {
        assert(_size > 0);
        st = max(st, 0), en = min(en, _size - 1);
        return fns(0, 0, _size, st, en + 1, va);
    }

   private:
    int _size;
    T _find_va;
    vector<T> _arr;
    vector<vector<T>> _seg_tree;
    void make(int lo, int st, int en)
    {
        int md, x, y, z;
        md = (st + en) / 2;
        if (st + 1 == en) {
            _seg_tree[lo][st] = _arr[st];
            return;
        }
        make(lo + 1, st, md);
        make(lo + 1, md, en);
        auto it = _seg_tree[lo + 1].begin();
        merge(it + st, it + md, it + md, it + en, _seg_tree[lo].begin() + st);
    }
    bool fns(int lo, int lx, int ly, int fx, int fy, T va)
    {
        int md, id;
        if (lx >= fy || ly <= fx)
            return false;
        md = (lx + ly) / 2;
        if (lx >= fx && ly <= fy) {
            id = lower_bound(_seg_tree[lo].begin() + lx, _seg_tree[lo].begin() + ly, va) - _seg_tree[lo].begin();
            if (id >= ly || _seg_tree[lo][id] != va)
                return false;
            _find_va = _seg_tree[lo][id];
            return true;
        }
        return fns(lo + 1, lx, md, fx, fy, va) | fns(lo + 1, md, ly, fx, fy, va);
    }
};
