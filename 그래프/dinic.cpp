template <typename flow_t>
struct dinic {
    struct edge {
        int v, rev;
        flow_t flw;
        edge(int _v, flow_t _flw, int _rev) : v(_v), flw(_flw), rev(_rev) {}
    };
    dinic(int _N) : N(_N), S(-1), T(-1)
    {
        idx.resize(N);
        lev.resize(N);
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
    flow_t get_flow(flow_t flow)
    {
        assert(S != -1 && T != -1);

        fill(idx.begin(), idx.end(), 0);
        fill(lev.begin(), lev.end(), -1);
        queue<int> que;
        que.push(S);
        lev[S] = 1;
        while (que.size() > 0) {
            int lo = que.front();
            que.pop();
            for (auto& ne : grp[lo]) {
                if (lev[ne.v] == -1 && ne.flw > 0) {
                    que.push(ne.v);
                    lev[ne.v] = lev[lo] + 1;
                }
            }
        }
        if (lev[T] == -1)
            return -1;
        flow_t ret_flow = 0;
        while (flow > 0) {
            flow_t va = dfs(S, flow);
            if (va == 0)
                break;
            ret_flow += va;
            flow -= va;
        }
        return ret_flow;
    }
    flow_t max_flow()
    {
        flow_t ret_flow;
        ret_flow = 0;
        for (;;) {
            auto flw = get_flow(numeric_limits<flow_t>::max());
            if (flw == -1)
                break;
            ret_flow += flw;
        }
        return ret_flow;
    }

   private:
    int N, S, T;
    vector<int> idx, lev;
    vector<vector<edge>> grp;

    flow_t dfs(int lo, flow_t flw)
    {
        flow_t ret_flow;
        if (lo == T)
            return flw;
        for (; idx[lo] < grp[lo].size(); idx[lo]++) {
            auto& ne = grp[lo][idx[lo]];
            if (lev[ne.v] == lev[lo] + 1 && ne.flw > 0) {
                ret_flow = dfs(ne.v, min(flw, ne.flw));
                if (ret_flow > 0) {
                    ne.flw -= ret_flow;
                    grp[ne.v][ne.rev].flw += ret_flow;
                    return ret_flow;
                }
            }
        }
        return 0;
    }
};
