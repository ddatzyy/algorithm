template <typename value_t>
struct lazy_min_max_segment_tree {  // 0-based index
    struct lazy_mix_sgt_node {
        value_t vmin, vmax, lzy;
        bool del;
        lazy_mix_sgt_node() : vmin(INF), vmax(-INF), lzy(0), del(0) {}
        lazy_mix_sgt_node(value_t _va) : vmin(_va), vmax(_va), lzy(0), del(0) {}
    };

    lazy_min_max_segment_tree(int _N)
    {
        N = 1;
        while (N < _N) N *= 2;
        seg.resize(N * 2);
    }
    lazy_min_max_segment_tree(vector<value_t> &arr, int si = 0)
    {  // si : start value index of arr
        N = 1;
        while (N < arr.size()) N *= 2;
        seg.resize(N * 2);
        for (int i = si; i < arr.size(); i++) seg[N + i] = lazy_mix_sgt_node(arr[i]);
        for (int i = N - 1; i > 0; i--) merge(seg[i], seg[i * 2], seg[i * 2 + 1]);
    }

    void insert(int lo, value_t va)
    {
        lo += N;
        seg[lo] = lazy_mix_sgt_node(va);
        lo /= 2;
        while (lo > 0) merge(seg[lo], seg[lo * 2], seg[lo * 2 + 1]), lo /= 2;
    }
    void update(int lo, value_t va) { update(0, N, lo, lo + 1, 1, va); }
    void update(int st, int en, value_t va) { update(0, N, st, en + 1, 1, va); }
    void erase(int lo) { erase(0, N, lo, lo + 1, 1); }
    void erase(int st, int en) { erase(0, N, st, en + 1, 1); }
    // return (min, max)
    pair<value_t, value_t> find(int st, int en)
    {
        fres = lazy_mix_sgt_node();
        find(0, N, st, en + 1, 1);
        return pair<value_t, value_t>(fres.vmin, fres.vmax);
    }

    value_t get_value(int lo)
    {
        update(lo, 0);
        return seg[N + lo].vmax;
    }
    // return (min, max)
    pair<value_t, value_t> top() { return pair<value_t, value_t>(seg[1].vmin, seg[1].vmax); }

    int N;
    lazy_mix_sgt_node fres;
    vector<lazy_mix_sgt_node> seg;

   private:
    static constexpr value_t INF = numeric_limits<value_t>::max();

    void lazy_propagation(int lo)
    {
        if (seg[lo].vmin != INF) seg[lo].vmin += seg[lo].lzy;
        if (seg[lo].vmax != -INF) seg[lo].vmax += seg[lo].lzy;
        if (seg[lo].del) seg[lo] = lazy_mix_sgt_node();
        if (lo < N) {
            int lt, rt;
            lt = lo * 2, rt = lt + 1;
            seg[lt].lzy += seg[lo].lzy, seg[rt].lzy += seg[lo].lzy;
            seg[lt].del |= seg[lo].del, seg[rt].del |= seg[lo].del;
        }
        seg[lo].lzy = seg[lo].del = 0;
    }
    void merge(lazy_mix_sgt_node &bse, lazy_mix_sgt_node &t1, lazy_mix_sgt_node &t2)
    {
        bse.vmax = max(t1.vmax, t2.vmax);
        bse.vmin = min(t1.vmin, t2.vmin);
    }

    void update(int lx, int ly, int fx, int fy, int lo, value_t va)
    {
        lazy_propagation(lo);
        if (lx >= fy || ly <= fx) return;
        if (lx >= fx && ly <= fy) {
            seg[lo].lzy += va;
            lazy_propagation(lo);
            return;
        }
        int md = (lx + ly) / 2;
        update(lx, md, fx, fy, lo * 2, va);
        update(md, ly, fx, fy, lo * 2 + 1, va);
        merge(seg[lo], seg[lo * 2], seg[lo * 2 + 1]);
    }
    void erase(int lx, int ly, int fx, int fy, int lo)
    {
        lazy_propagation(lo);
        if (lx >= fy || ly <= fx) return;
        if (lx >= fx && ly <= fy) {
            seg[lo].del = true;
            lazy_propagation(lo);
            return;
        }
        int md = (lx + ly) / 2;
        erase(lx, md, fx, fy, lo * 2);
        erase(md, ly, fx, fy, lo * 2 + 1);
        merge(seg[lo], seg[lo * 2], seg[lo * 2 + 1]);
    }
    void find(int lx, int ly, int fx, int fy, int lo)
    {
        lazy_propagation(lo);
        if (lx >= fy || ly <= fx) return;
        if (lx >= fx && ly <= fy) {
            fres.vmin = min(fres.vmin, seg[lo].vmin);
            fres.vmax = max(fres.vmax, seg[lo].vmax);
            return;
        }
        int md = (lx + ly) / 2;
        find(lx, md, fx, fy, lo * 2);
        find(md, ly, fx, fy, lo * 2 + 1);
        merge(seg[lo], seg[lo * 2], seg[lo * 2 + 1]);
    }
};