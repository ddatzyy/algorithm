struct strongly_connected_component {
    strongly_connected_component(int _N) : N(_N)
    {
        grp.resize(N);
        dfn.resize(N, -1);
        idx.resize(N, -1);
        dN = 0, sz_over_2 = 0;
    }

    void add_edge(int u, int v) { grp[u].push_back(v); }

    void init()
    {
        for (int i = 0; i < N; i++)
            if (dfn[i] == -1)
                dfs(i);
        dag_grp.resize(scc.size());
        vector<bool> vis(N), vig(scc.size());
        for (int i = 0; i < scc.size(); i++) {
            for (auto ne : scc[i])
                for (auto v : grp[ne])
                    vis[v] = 1, vig[idx[v]] = 1;
            for (auto ne : scc[i])
                vis[ne] = 0;
            vig[i] = 0;
            for (auto ne : scc[i])
                for (auto v : grp[ne])
                    if (vis[v] == 1) {
                        if (vig[idx[v]] == 1)
                            dag_grp[i].push_back(idx[v]);
                        vis[v] = 0, vig[idx[v]] = 0;
                    }
            if (scc[i].size() > 1)
                sz_over_2++;
        }
    }

    int N, sz_over_2;
    vector<int> idx;
    vector<vector<int>> grp, scc, dag_grp;

   private:
    int dN;
    vector<int> stk, dfn;

    int dfs(int lo)
    {
        int ret;
        dfn[lo] = ret = dN++;
        stk.push_back(lo);
        for (auto ne : grp[lo]) {
            if (dfn[ne] == -1)
                ret = min(ret, dfs(ne));
            else if (idx[ne] == -1)
                ret = min(ret, dfn[ne]);
        }
        if (ret == dfn[lo]) {
            scc.emplace_back();
            auto &tem = scc.back();
            int si = scc.size() - 1;
            do {
                tem.push_back(stk.back());
                idx[stk.back()] = si;
                stk.pop_back();
            } while (tem.back() != lo);
        }
        return ret;
    }
};