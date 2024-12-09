template <typename flow_t, typename cost_t>
struct zkw_mcmf {
    struct edge {
        int v, rev;
        flow_t flw;
        cost_t cst;
        edge(int _v, flow_t _f, cost_t _c, int _rev) : v(_v), flw(_f), cst(_c), rev(_rev) {}
    };
    zkw_mcmf(int _N)
    {
        N = _N;
        S = T = -1;
        grp.resize(N);
        idx = new int[N + 1];
        vis = new bool[N + 1];
        dst.resize(N);
    }
    void add_edge(int u, int v, flow_t flow, cost_t cost)
    {
        grp[u].push_back({v, flow, cost, int(grp[v].size())});
        grp[v].push_back({u, 0, -cost, int(grp[u].size()) - 1});
    }
    void set_ST(int _S, int _T)
    {
        S = _S, T = _T;
    }
    void spfa()
    {
        assert(S != -1 && T != -1);

        fill(dst.begin(), dst.end(), numeric_limits<cost_t>::max());
        vector<bool> inn(N);
        queue<int> que;
        que.push(S);
        inn[S] = 1;
        dst[S] = 0;
        while (que.size() > 0) {
            auto lo = que.front();
            que.pop();
            inn[lo] = 0;
            for (auto &ne : grp[lo])
                if (ne.flw > 0 && dst[ne.v] > dst[lo] + ne.cst) {
                    dst[ne.v] = dst[lo] + ne.cst;
                    if (inn[ne.v] == 0) {
                        que.push(ne.v);
                        inn[ne.v] = 1;
                    }
                }
        }
    }
    void dag_spfa()
    {
        assert(S != -1 && T != -1);

        fill(dst.begin(), dst.end(), numeric_limits<cost_t>::max());
        vector<int> ind(N);
        for (int i = 0; i < N; i++)
            for (edge &ne : grp[i])
                if (ne.flw > 0)
                    ind[ne.v]++;
        queue<int> que;
        que.push(S);
        dst[S] = 0;
        while (que.size() > 0) {
            int lo = que.front();
            que.pop();
            for (edge &ne : grp[lo])
                if (ne.flw > 0) {
                    dst[ne.v] = min(dst[ne.v], dst[lo] + ne.cst);
                    ind[ne.v]--;
                    if (ind[ne.v] == 0)
                        que.push(ne.v);
                }
        }
    }
    pair<flow_t, cost_t> min_cost_max_flow()
    {
        assert(S != -1 && T != -1);

        flow_t va;
        pair<flow_t, cost_t> ret = {0, 0};
        for (;;) {
            memset(idx, 0, sizeof(int) * N);
            for (;;) {
                memset(vis, 0, sizeof(bool) * N);
                va = dfs(S, numeric_limits<flow_t>::max());
                if (va == 0)
                    break;
                ret.first += va;
                ret.second += va * dst[T];
            }
            if (update() == 0)
                break;
        }
        return ret;
    }

   private:
    int N, S, T;
    int *idx;
    bool *vis;
    vector<cost_t> dst;
    vector<vector<edge>> grp;
    flow_t dfs(int lo, flow_t flw)
    {
        flow_t va;
        vis[lo] = 1;
        if (lo == T)
            return flw;
        for (; idx[lo] < grp[lo].size(); idx[lo]++) {
            auto &ne = grp[lo][idx[lo]];
            if (vis[ne.v] == 0 && ne.flw > 0 && dst[ne.v] == dst[lo] + ne.cst) {
                va = dfs(ne.v, min(flw, ne.flw));
                if (va > 0) {
                    ne.flw -= va;
                    grp[ne.v][ne.rev].flw += va;
                    return va;
                }
            }
        }
        return 0;
    }
    bool update()
    {
        cost_t va;
        va = numeric_limits<cost_t>::max();
        for (int i = 0; i < N; i++)
            if (vis[i] == 1)
                for (auto &ne : grp[i])
                    if (ne.flw > 0 && vis[ne.v] == 0)
                        va = min(va, dst[i] + ne.cst - dst[ne.v]);
        if (va == numeric_limits<cost_t>::max())
            return 0;
        for (int i = 0; i < N; i++)
            if (vis[i] == 0)
                dst[i] += va;
        return 1;
    }
};
