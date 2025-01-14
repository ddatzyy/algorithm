template <typename value_t>
struct segment_tree {  // 0-based index, !!!you need to fill segment_tree_node::update(), find(), merge()!!!
    struct segment_tree_node {
        value_t val;
        segment_tree_node() : val(0) {}
        segment_tree_node(value_t _val) : val(_val) {}

        void update(value_t _val)
        {  // update val
            /*
            fill this!
            */
            assert(0);
        }
    };

    segment_tree(int _N)
    {
        N = 1;
        while (N < _N)
            N *= 2;
        seg.resize(N * 2);
    }

    void insert(int lo, value_t va)
    {
        lo += N;
        seg[lo].val = va;
        lo /= 2;
        while (lo > 0) {
            merge(seg[lo], seg[lo * 2], seg[lo * 2 + 1]);
            lo /= 2;
        }
    }
    void update(int lo, value_t va)
    {
        lo += N;
        seg[lo].update(va);
        lo /= 2;
        while (lo > 0) {
            merge(seg[lo], seg[lo * 2], seg[lo * 2 + 1]);
            lo /= 2;
        }
    }
    value_t find(int st, int en)
    {  // find range [st, en]
        fres = segment_tree_node();
        find(0, N, st, en + 1, 1);
    }

    int N;
    segment_tree_node fres;
    vector<segment_tree_node> seg;

   private:
    segment_tree_node merge(segment_tree_node &bse, segment_tree_node &t1, segment_tree_node &t2)
    {  // merge t1 and t2 to get bse
        /*
        fill this!
        */
        assert(0);
    }

    void find(int lx, int ly, int fx, int fy, int lo)
    {
        if (lx >= fy || ly <= fx)
            return;
        if (lx >= fx && ly <= fy) {
            find_operation(lo);
            return;
        }
        int md = (lx + ly) / 2;
        find(lx, md, fx, fy, lo * 2);
        find(md, ly, fx, fy, lo * 2 + 1);
    }

    inline void find_operation(int lo)
    {
        /*
        fill this!
        */
        assert(0);
    }
};