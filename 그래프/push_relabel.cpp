template <typename flow_t>
struct push_relabel {
    struct edge {
        int v, rev;
        flow_t flw;
        edge(int _v, flow_t _f, int _rev) : v(_v), flw(_f), rev(_rev) {}
    };
    push_relabel(int _N)
    {
        N = _N;
        S = T = -1;
        grp.resize(N);
    }
    void add_edge(int u, int v, flow_t flow)
    {
        grp[u].push_back({v, flow, int(grp[v].size())});
        grp[v].push_back({u, 0, int(grp[u].size()) - 1});
    }
    void set_ST(int _S, int _T)
    {
        S = _S, T = _T;
    }

    // max_flow
    flow_t max_flow()
    {
        assert(S != -1 && T != -1);

        vector<int> idx(N);
        vector<int> hei(N, 0);
        vector<int> cnt(N * 2);
        vector<flow_t> ex(N);
        vector<vector<int>> stk(N * 2);

        function<void(edge &, flow_t)> push = [&](edge &lo, flow_t va) {
            if (ex[lo.v] == 0 && va > 0)
                stk[hei[lo.v]].push_back(lo.v);
            lo.flw -= va;
            grp[lo.v][lo.rev].flw += va;
            ex[grp[lo.v][lo.rev].v] -= va;
            ex[lo.v] += va;
        };

        function<void(int)> relabel = [&](int u) {
            hei[u] = 3 * N;
            for (int i = 0; i < grp[u].size(); i++) {
                edge &lo = grp[u][i];
                if (lo.flw > 0 && hei[lo.v] + 1 < hei[u]) {
                    hei[u] = hei[lo.v] + 1;
                    idx[u] = i;
                }
            }
        };

        hei[S] = N;
        ex[T] = 1;
        cnt[0] = N - 1;
        for (edge &ne : grp[S])
            push(ne, ne.flw);
        if (stk[0].size() > 0) {
            int hi = 0;
            while (hi >= 0) {
                int u = stk[hi].back();
                stk[hi].pop_back();
                // discharge u
                while (ex[u] > 0) {
                    if (idx[u] == grp[u].size()) {
                        relabel(u);
                        cnt[hei[u]]++, cnt[hi]--;
                        // gap heuristic
                        if (cnt[hi] == 0 && hi < N)
                            for (int i = 0; i < N; i++)
                                if (hei[i] > hi && hei[i] < N) {
                                    cnt[hei[i]]--;
                                    stk[hei[i]].clear();
                                    hei[i] = N + 1;
                                }
                        hi = hei[u];
                    }
                    while (idx[u] < grp[u].size()) {
                        edge &lo = grp[u][idx[u]];
                        if (lo.flw > 0 && hei[u] == hei[lo.v] + 1) {
                            push(lo, min(ex[u], lo.flw));
                            if (ex[u] == 0)
                                break;
                        }
                        idx[u]++;
                    }
                }
                while (hi >= 0 && stk[hi].size() == 0)
                    hi--;
            }
        }
        return -ex[S];
    }

   private:
    int N, S, T;
    vector<vector<edge>> grp;
};
