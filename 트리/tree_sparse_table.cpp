template <typename cost_t, bool norm_tree = true>
struct tree_sparse_table {  // 1-based tree
    tree_sparse_table(int _N) : N(_N), root(0), sp(log2(_N + 1) + 1)
    {
        nds.resize(N + 1);
        dep.resize(N + 1);
        dst.resize(N + 1);
        idst_sum.resize(N + 1);
        grp.resize(N + 1);
    }
    void add_edge(int u, int v, cost_t dt = 1)
    {
        grp[u].push_back({v, dt});
        grp[v].push_back({u, dt});
    }
    void init(int _root)
    {
        root = _root;
        spt.resize(N + 1, vector<int>(sp));
        if (norm_tree)
            normalize_tree(root, 0);
        dfs(root, 0, 0);
    }
    int longest_common_ancestor(int u, int v)
    {
        assert(root > 0);
        if (dep[u] < dep[v])
            swap(u, v);
        for (int i = 0; i < sp; i++)
            if (((dep[u] - dep[v]) >> i) & 1)
                u = spt[u][i];
        if (u == v)
            return u;
        for (int i = sp - 1; i >= 0; i--)
            if (spt[u][i] != spt[v][i])
                u = spt[u][i], v = spt[v][i];
        return spt[u][0];
    }
    int go_up_index(int lo, int va)
    {
        assert(root > 0);
        for (int i = 0; i < sp && va > 0; i++) {
            if (va & 1)
                lo = spt[lo][i];
            va >>= 1;
        }
        return lo;
    }
    int go_up_distance(int lo, cost_t va)
    {
        assert(root > 0);
        int st, en, md;
        st = 0, en = dep[lo] - dep[root] + 1;
        while (st + 1 < en) {
            md = (st + en) >> 1;
            auto ne = go_up_index(lo, md);
            if (dst[lo] - dst[ne] > va)
                en = md;
            else
                st = md;
        }
        return go_up_index(lo, st);
    }
    void update()
    {
        assert(root > 0);
        // make new sparse table
        update_dfs(root, 0);
    }

   private:
    int root, N, sp;
    vector<int> nds, dep;          // nds : count(descendants + node), dep : count(root -> node)
    vector<cost_t> dst, idst_sum;  // dst : |root -> node|, idst : |node -> descendants|
    vector<vector<pair<int, cost_t>>> grp;
    vector<vector<int>> spt;

    void normalize_tree(int lo, int be)
    {
        vector<pair<int, cost_t>> tem;
        for (auto [ne, dt] : grp[lo])
            if (ne != be) {
                normalize_tree(ne, lo);
                tem.push_back({ne, dt});
            }
        grp[lo].swap(tem);
    }
    void dfs(int lo, int be, cost_t de)
    {
        spt[lo][0] = be, nds[lo] = 1, dst[lo] = de, dep[lo] = dep[be] + 1;
        for (int i = 1; i < sp; i++)
            spt[lo][i] = spt[spt[lo][i - 1]][i - 1];
        for (auto [ne, dt] : grp[lo])
            if (norm_tree || ne != be) {
                dfs(ne, lo, de + dt);
                nds[lo] += nds[ne];
                idst_sum[lo] += idst_sum[ne] + nds[ne] * dt;
            }
    }
    void update_dfs(int lo, int be)
    {  // if norm_tree is false : remember grp[lo] -> ne has ancestor
        for (auto [ne, dt] : grp[lo])
            if (norm_tree || ne != be) {
                update_dfs(ne, lo);
            }
    }
};
