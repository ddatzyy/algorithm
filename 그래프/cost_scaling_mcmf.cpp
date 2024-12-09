template <typename flow_t, typename cost_t, bool use_look_ahead = true>
struct cost_scaling_mcmf {
    struct edge {
        int v, rev;
        flow_t flw;
        cost_t cst;
        edge(int _v, flow_t _f, cost_t _c, int _rev) : v(_v), flw(_f), cst(_c), rev(_rev) {}
    };
    cost_scaling_mcmf(int _N)
    {
        eps = 0;
        N = _N;
        S = T = -1;
        idx.resize(N);
        grp.resize(N);
        ex.resize(N);
        ptl.resize(N);
    }
    void add_edge(int u, int v, flow_t flow, cost_t cost)
    {
        cost *= N;
        eps = max(eps, abs(cost));
        grp[u].push_back({v, flow, cost, int(grp[v].size())});
        grp[v].push_back({u, 0, -cost, int(grp[u].size()) - 1});
    }
    void set_ST(int _S, int _T)
    {
        S = _S, T = _T;
    }

    // max_flow
    flow_t max_flow()
    {
        assert(S != -1 && T != -1);

        fill(idx.begin(), idx.end(), 0);
        fill(ex.begin(), ex.end(), 0);
        vector<int> hei(N, 0);
        vector<int> cnt(N * 2);
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

    // min_cost_max_flow
    bool look_ahead(int u)
    {
        if (ex[u] != 0)
            return false;
        cost_t new_ptl = ptl[u] - N * eps;
        for (edge &ne : grp[u])
            if (ne.flw > 0) {
                if (ptl[u] - ptl[ne.v] + ne.cst < 0)
                    return false;
                new_ptl = max(new_ptl, ptl[ne.v] - ne.cst);
            }
        ptl[u] = new_ptl - eps;
        return true;
    }
    pair<flow_t, cost_t> min_cost_max_flow()
    {
        cost_t ret_cost = 0;
        for (int i = 0; i < N; i++)
            for (edge &ne : grp[i])
                ret_cost += ne.cst * ne.flw;
        flow_t ret_flow = max_flow();

        fill(ex.begin(), ex.end(), 0);
        fill(ptl.begin(), ptl.end(), 0);
        vector<int> stk, isq(N);

        function<void(edge &, flow_t)> push = [&](edge &lo, flow_t va) {
            lo.flw -= va;
            grp[lo.v][lo.rev].flw += va;
            ex[grp[lo.v][lo.rev].v] -= va;
            ex[lo.v] += va;
        };

        function<void(int)> relabel = [&](int u) {
            // {cost_u_v + potential_u - potential_v >= -eps} then {new_potential_u = max(potential_v - cost_u_v) - eps}
            cost_t new_ptl = -INF_COST;
            for (int i = 0; i < grp[u].size(); i++) {
                edge &lo = grp[u][i];
                if (lo.flw > 0 && ptl[lo.v] - lo.cst > new_ptl) {
                    new_ptl = ptl[lo.v] - lo.cst;
                    idx[u] = i;
                }
            }
            ptl[u] = new_ptl - eps;
        };

        while (eps > 0) {
            fill(idx.begin(), idx.end(), 0);
            for (int i = 0; i < N; i++)
                for (edge &ne : grp[i])
                    if (ptl[i] - ptl[ne.v] + ne.cst < 0 && ne.flw > 0)
                        push(ne, ne.flw);
            for (int i = 0; i < N; i++)
                if (ex[i] > 0) {
                    stk.push_back(i);
                    isq[i] = 1;
                }
            while (stk.size() > 0) {
                int u = stk.back();
                stk.pop_back();
                isq[u] = 0;
                while (ex[u] > 0) {
                    if (idx[u] == grp[u].size())
                        relabel(u);
                    while (idx[u] < grp[u].size()) {
                        edge &lo = grp[u][idx[u]];
                        if (lo.flw > 0 && ptl[u] - ptl[lo.v] + lo.cst < 0) {
                            if (use_look_ahead == 1 && look_ahead(lo.v) == 1)
                                continue;
                            push(lo, min(lo.flw, ex[u]));
                            if (isq[lo.v] == 0) {
                                stk.push_back(lo.v);
                                isq[lo.v] = 1;
                            }
                            if (ex[u] == 0)
                                break;
                        }
                        idx[u]++;
                    }
                }
            }
            if (eps > 1 && (eps >> scale) == 0)
                eps = 1 << scale;
            eps >>= scale;
        }
        for (int i = 0; i < N; i++)
            for (edge &ne : grp[i])
                ret_cost -= ne.cst * ne.flw;
        return pair<flow_t, cost_t>(ret_flow, ret_cost / 2 / N);
    }

   private:
    static constexpr int scale = 2;
    static constexpr cost_t INF_COST = numeric_limits<cost_t>::max() / 2;

    int N, S, T;
    cost_t eps;
    vector<int> idx;
    vector<vector<edge>> grp;
    vector<flow_t> ex;
    vector<cost_t> ptl;
};
