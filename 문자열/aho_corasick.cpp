template <typename converter_t, int char_size>
struct aho_corasick {
    struct node {
        int kmp, cnt;
        vector<int> next;
        node() : kmp(-1), cnt(0) {}
    };
    aho_corasick(converter_t _cvt) : sz(0), new_node_index(0), is_init(0), cvt(_cvt) {}
    void add_string(string va)
    {
        sz += va.size();
        vector<int> str;
        for (auto ne : va)
            str.push_back(cvt[ne]);
        vector_pool.push_back(str);
    }
    void init()
    {
        is_init = 1;
        tri.resize(sz + 1);
        root = new_node();
        make_trie();
        make_kmp();
    }
    int count(string str)
    {
        assert(is_init == 1);
        int lo, va, ret;
        lo = root;
        ret = 0;
        for (auto ne : str) {
            va = cvt[ne];
            while (tri[lo].next[va] == -1 && lo != root)
                lo = tri[lo].kmp;
            if (tri[lo].next[va] != -1)
                lo = tri[lo].next[va];
            ret += tri[lo].cnt;
        }
        return ret;
    }

   private:
    int sz, root, new_node_index, is_init;
    converter_t cvt;
    vector<node> tri;
    vector<vector<int>> next_que, vector_pool;

    int new_node()
    {
        tri[new_node_index].next.resize(char_size, -1);
        return new_node_index++;
    }
    void make_trie()
    {
        int lo, ne;
        next_que.resize(tri.size());
        for (int i = 0; i < vector_pool.size(); i++) {
            lo = root;
            for (auto ne : vector_pool[i]) {
                if (tri[lo].next[ne] == -1) {
                    tri[lo].next[ne] = new_node();
                    next_que[lo].push_back(ne);
                }
                lo = tri[lo].next[ne];
            }
            tri[lo].cnt++;
        }
    }
    void make_kmp()
    {
        int n, m, lo;
        queue<int> que;
        for (auto ne : next_que[root]) {
            n = tri[root].next[ne];
            tri[n].kmp = root;
            que.push(n);
        }
        while (que.size() > 0) {
            lo = que.front();
            que.pop();
            for (auto ne : next_que[lo]) {
                n = tri[lo].next[ne], m = tri[lo].kmp;
                while (tri[m].next[ne] == -1 && m != root)
                    m = tri[m].kmp;
                if (tri[m].next[ne] != -1)
                    m = tri[m].next[ne];
                tri[n].kmp = m;
                tri[n].cnt += tri[m].cnt;
                que.push(n);
            }
        }
    }
};
