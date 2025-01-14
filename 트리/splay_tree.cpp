template <typename value_t>
struct splay_tree {
    struct splay_tree_node {
        splay_tree_node *l, *r;
        int sz;
        bool flip;
        value_t val;
        splay_tree_node() : l(nullptr), r(nullptr), sz(1), flip(false), val(0) {}
        splay_tree_node(value_t _val) : l(nullptr), r(nullptr), sz(1), flip(false), val(_val) {}
    };

    splay_tree() : root(nullptr) {}
    splay_tree(vector<value_t> &arr) : root(init(0, arr.size(), arr)) {}

    void set(int k, value_t va)
    {  // 0-index
        root = splay(root, k);
        root->val = va;
        update(root);
    }
    value_t get(int k)
    {  // 0-index
        root = splay(root, k);
        return root->val;
    }
    void flip(int l, int r)
    {  // 0-index
        if (l == r)
            return;
        if (l == 0 && r == size(root))
            flip(root);
        else if (l == 0) {
            root = splay(root, r);
            flip(root->l);
        }
        else if (r == size(root)) {
            root = splay(root, l - 1);
            flip(root->r);
        }
        else {
            auto [a, b] = split(root, l);
            b = splay(b, r - l);
            flip(b->l);
            root = merge(a, b);
        }
    }
    void insert(int k, value_t va)
    {  // 0-index
        splay_tree_node *tm = new splay_tree_node(va);
        if (k == 0) {
            tm->r = root;
            root = tm;
            update(root);
        }
        else if (k == size(root)) {
            tm->l = root;
            root = tm;
            update(root);
        }
        else {
            root = splay(root, k);
            tm->l = root->l, root->l = tm;
            update(tm), update(root);
        }
    }
    void erase(int k)
    {  // 0-index
        root = splay(root, k);
        root = merge(root->l, root->r);
    }
    int size() { return size(root); }

   private:
    splay_tree_node *root;

    splay_tree_node *init(int l, int r, vector<value_t> &arr)
    {
        if (l >= r)
            return nullptr;
        int md = (l + r) / 2;
        splay_tree_node *tm = new splay_tree_node(arr[md]);
        if (md > l) tm->l = init(l, md, arr);
        if (md + 1 < r) tm->r = init(md + 1, r, arr);
        update(tm);
        return tm;
    }
    /*
            v                 l
         ↙   ↘           ↙   ↘
        l       r   =>   ll       v
     ↙   ↘                   ↙   ↘
    ll     lr                lr       r
    */
    splay_tree_node *rotate_right(splay_tree_node *v)
    {
        splay_tree_node *tm = v->l;
        v->l = tm->r;
        tm->r = v;
        update(v), update(tm);
        return tm;
    }
    /*
        v                           r
     ↙   ↘                     ↙   ↘
    l       r       =>         v       rr
         ↙   ↘            ↙   ↘
        rl     rr         l       rl
    */
    splay_tree_node *rotate_left(splay_tree_node *v)
    {
        splay_tree_node *tm = v->r;
        v->r = tm->l;
        tm->l = v;
        update(v), update(tm);
        return tm;
    }
    /*
           v
        ↙   ↘       make k-th splay_tree_node in S to v (0-index)
      ↙   S   ↘
    ↙___________↘
    */
    splay_tree_node *splay(splay_tree_node *v, int k)
    {
        push_down(v);
        int sz = v->l ? v->l->sz : 0;
        if (k == sz) return v;
        if (k < sz) {
            v->l = splay(v->l, k);
            v = rotate_right(v);
        }
        else {
            v->r = splay(v->r, k - sz - 1);
            v = rotate_left(v);
        }
        update(v);
        return v;
    }
    /*
                           r
    merge l and r to    ↙   ↘
                      l       ...
    */
    splay_tree_node *merge(splay_tree_node *l, splay_tree_node *r)
    {
        if (!l || !r) return !l ? r : l;
        r = splay(r, 0);
        r->l = l;
        update(r);
        return r;
    }
    // split {v} to {0 ~ (k-1)-th splay_tree_node, k-th splay_tree_node ~ end}
    pair<splay_tree_node *, splay_tree_node *> split(splay_tree_node *v, int k)
    {
        if (k >= size(v)) return {v, nullptr};
        v = splay(v, k);
        splay_tree_node *tm = v->l;
        v->l = nullptr;
        update(v);
        return {tm, v};
    }
    int size(splay_tree_node *v)
    {
        return !v ? 0 : v->sz;
    }
    void update(splay_tree_node *v)
    {
        v->sz = 1;
        if (v->l) {
            v->sz += v->l->sz;
        }
        if (v->r) {
            v->sz += v->r->sz;
        }
    }
    void flip(splay_tree_node *v)
    {
        if (v) {
            swap(v->l, v->r);
            v->flip ^= true;
        }
    }
    void push_down(splay_tree_node *v)
    {
        if (!v) return;
        if (v->flip) {
            if (v->l) flip(v->l);
            if (v->r) flip(v->r);
            v->flip = false;
        }
    }
};