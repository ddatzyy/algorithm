template <typename coord_t>
struct li_chao_tree {  // default : max
    struct line {
        coord_t a, b;
        line() : a(0), b(-COORD_INF) {}
        line(coord_t _a, coord_t _b) : a(_a), b(_b) {}
        coord_t get(coord_t va) { return a * va + b; }
        pair<coord_t, coord_t> intersect(line& va)
        {
            coord_t dx, dy;
            if (a == va.a)
                return {1, 0};
            dx = a - va.a, dy = va.b - b;
            return {dy / dx - (dy % dx > 0 && dy * dx < 0), dy / dx + (dy % dx > 0 && dy * dx > 0)};
        }
    };
    struct node {
        int lt, rt;
        line lin;
        node() : lt(-1), rt(-1), lin() {}
        node(int _lt, int _rt, line _lin) : lt(_lt), rt(_rt), lin(_lin) {}
    };

    li_chao_tree(int _N, coord_t _rng) : N(_N)  // N line, x range [-rng, rng]
    {
        int ri;
        rng = 1, ri = 2;
        while (rng < _rng)
            rng *= 2, ri++;
        seg.resize(N * ri);
        new_node_num = 0;
        root = new_node();
    }
    void insert(coord_t a, coord_t b)  // insert {y = ax + b}
    {
        line va(a, b);
        update(-rng, rng, root, va);
    }
    coord_t find(coord_t va)
    {
        find_ret = -COORD_INF;
        find(-rng, rng, root, va);
        return find_ret;
    }

   private:
    static constexpr coord_t COORD_INF = numeric_limits<coord_t>::max() / 2;
    int N, root, new_node_num;
    coord_t rng, find_ret;
    vector<node> seg;
    int new_node() { return new_node_num++; }
    void update(coord_t st, coord_t en, int lo, line va)
    {
        coord_t md;
        line lw, hi;
        md = (st + en) / 2;
        lw = seg[lo].lin, hi = va;
        if (pair<coord_t, coord_t>(hi.get(st), hi.get(en)) < pair<coord_t, coord_t>(lw.get(st), lw.get(en)))
            swap(lw, hi);
        if (hi.get(en) >= lw.get(en)) {
            seg[lo].lin = hi;
            return;
        }
        if (st + 1 == en)
            return;
        if (lw.get(md) > hi.get(md)) {
            seg[lo].lin = lw;
            if (seg[lo].lt == -1)
                seg[lo].lt = new_node();
            update(st, md, seg[lo].lt, hi);
        }
        else {
            seg[lo].lin = hi;
            if (seg[lo].rt == -1)
                seg[lo].rt = new_node();
            update(md, en, seg[lo].rt, lw);
        }
    }
    void find(coord_t st, coord_t en, int lo, coord_t va)
    {
        coord_t md;
        if (lo == -1)
            return;
        md = (st + en) / 2;
        find_ret = max(find_ret, seg[lo].lin.get(va));
        if (va < md)
            find(st, md, seg[lo].lt, va);
        else
            find(md, en, seg[lo].rt, va);
    }
};
