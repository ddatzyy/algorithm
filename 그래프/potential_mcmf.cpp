template <typename flow_t, typename cost_t>
struct potential_mcmf {
    struct edge {
        int v, rev;
        flow_t flw;
        cost_t cst;
        edge(int _v, flow_t _f, cost_t _c, int _rev) : v(_v), flw(_f), cst(_c), rev(_rev) {}
    };
    potential_mcmf(int _N)
    {
        N = _N;
        S = T = -1;
        grp.resize(N);
        ptl.resize(N);
    }
    void add_edge(int u, int v, flow_t flow, cost_t cst)
    {
        grp[u].push_back({v, flow, cst, int(grp[v].size())});
        grp[v].push_back({u, 0, -cst, int(grp[u].size()) - 1});
    }
    void set_ST(int _S, int _T)
    {
        S = _S, T = _T;
    }
    void spfa()
    {
        assert(S != -1 && T != -1);
        
        fill(ptl.begin(), ptl.end(), numeric_limits<cost_t>::max());
        vector<bool> inn(N);
        queue<int> que;
        que.push(S);
        inn[S] = 1;
        ptl[S] = 0;
        while (que.size() > 0) {
            auto lo = que.front();
            que.pop();
            inn[lo] = 0;
            for (auto &ne : grp[lo])
                if (ne.flw > 0 && ptl[ne.v] > ptl[lo] + ne.cst) {
                    ptl[ne.v] = ptl[lo] + ne.cst;
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

        fill(ptl.begin(), ptl.end(), numeric_limits<cost_t>::max());
        vector<int> ind(N);
        for (int i = 0; i < N; i++)
            for (edge &ne : grp[i])
                if (ne.flw > 0)
                    ind[ne.v]++;
        queue<int> que;
        que.push(S);
        ptl[S] = 0;
        while (que.size() > 0) {
            int lo = que.front();
            que.pop();
            for (edge &ne : grp[lo])
                if (ne.flw > 0) {
                    ptl[ne.v] = min(ptl[ne.v], ptl[lo] + ne.cst);
                    ind[ne.v]--;
                    if (ind[ne.v] == 0)
                        que.push(ne.v);
                }
        }
    }
    pair<flow_t, cost_t> min_cost_max_flow()
    {
        assert(S != -1 && T != -1);
        
        pair<flow_t, cost_t> ret = {0, 0};
        vector<edge *> edg(N);
        vector<bool> vis(N);
        vector<flow_t> flw(N);
        vector<cost_t> dst(N, numeric_limits<cost_t>::max());
        for (;;) {
            priority_queue<pair<cost_t, int>, vector<pair<cost_t, int>>, greater<>> pq;
            pq.push({0, S});
            dst[S] = 0;
            flw[S] = numeric_limits<flow_t>::max();
            while (pq.size() > 0) {
                auto [dt, lo] = pq.top();
                pq.pop();
                if (vis[lo] == 1)
                    continue;
                vis[lo] = 1;
                for (auto &ne : grp[lo]) {
                    if (vis[ne.v] == 0 && ne.flw > 0 && dst[ne.v] > dt + ne.cst + ptl[lo] - ptl[ne.v]) {
                        dst[ne.v] = dt + ne.cst + ptl[lo] - ptl[ne.v];
                        pq.push({dst[ne.v], ne.v});
                        flw[ne.v] = min(flw[lo], ne.flw);
                        edg[ne.v] = &ne;
                    }
                }
            }
            if (vis[T] == 0)
                break;
            for (int i = 0; i < N; i++)
                if (vis[i] == 1) {
                    // dst[i] -= ptl[S] - ptl[i];
                    // ptl[i] = dst[i];
                    ptl[i] += dst[i];
                    vis[i] = 0;
                    dst[i] = numeric_limits<cost_t>::max();
                }
            flow_t va = flw[T];
            ret.first += va;
            ret.second += ptl[T] * va;
            int lo = T;
            while (lo != S) {
                auto ne = edg[lo];
                ne->flw -= va;
                grp[ne->v][ne->rev].flw += va;
                lo = grp[ne->v][ne->rev].v;
            }
        }
        return ret;
    }

   private:
    int N, S, T;
    vector<cost_t> ptl;
    vector<vector<edge>> grp;
};
