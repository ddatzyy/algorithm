struct hopcroft_karp {
    hopcroft_karp(int _N, int _iN)
    {
        N = _N, iN = _iN;
        grp.resize(N);
        que.resize(N);
        index_u.resize(N, -1), index_v.resize(iN, -1);
        dsj_set.resize(N);
        prev_u.resize(N);
    }
    void add_edge(int u, int v)
    {
        grp[u].push_back(v);
    }
    int match_index_u(int lo)
    {
        return index_u[lo];
    }
    int match_index_v(int lo)
    {
        return index_v[lo];
    }
    int matching()
    {
        int u, loc, flow, cur_flow, q_begin, q_end;
        flow = 0;
        vector<pii> lst(N + 2);  // {N} | - {0} - ... - {N - 1} - | {N + 1}
        auto connect = [&](int x, int y) { lst[x].second = y, lst[y].first = x; };
        connect(0, N), connect(N - 1, N + 1);
        for (int i = 0; i < N - 1; i++)
            connect(i, i + 1);
        vector<int> last_rep(N);
        for (int rep = 1;; rep++) {
            cur_flow = 0;
            q_begin = 0, q_end = 0;
            loc = lst[N].second;
            while (loc < N + 1) {
                if (index_u[loc] == -1) {
                    que[q_end++] = dsj_set[loc] = prev_u[loc] = loc;
                    last_rep[loc] = rep;
                }
                loc = lst[loc].second;
            }
            while (q_begin < q_end) {
                u = que[q_begin++];
                if (index_u[dsj_set[u]] != -1)
                    continue;
                for (auto v : grp[u]) {
                    if (index_v[v] == -1) {
                        while (v != -1) {
                            index_v[v] = u;
                            swap(index_u[u], v);
                            u = prev_u[u];
                            /*
                            Before : {u} <-> {index_u[u]}
                            After : {u} <-> {v}
                            Update since index_u[u] matching is missing : {v} to {u} and {u} to previous vertex {prev_u[u]}
                            */
                        }
                        connect(lst[u].first, lst[u].second);
                        cur_flow++;
                        break;
                    }
                    if (last_rep[index_v[v]] < rep) {
                        que[q_end++] = index_v[v];
                        prev_u[index_v[v]] = u, dsj_set[index_v[v]] = dsj_set[u];
                        last_rep[index_v[v]] = rep;
                    }
                }
            }
            if (cur_flow == 0)
                break;
            flow += cur_flow;
        }
        return flow;
    }

   private:
    int N, iN;
    vector<int> que, index_u, index_v, dsj_set, prev_u;
    vector<vector<int>> grp;
};
