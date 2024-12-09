struct functional_graph {
    functional_graph(int _N) : N(_N)
    {
        grp.resize(N, -1);
        dst.resize(N, -1);
        go_cycle.resize(N, -1);
    }
    void add_edge(int u, int v) { grp[u] = v; }
    void init()
    {
        int n, m, k;
        vector<int> vis(N, -1);
        for (int i = 0; i < N; i++)
            if (vis[i] == -1 && grp[i] != -1) {
                n = i, m = 0;
                vector<int> tem;
                while (vis[n] == -1) {
                    tem.push_back(n);
                    vis[n] = i, dst[n] = m++;
                    n = grp[n];
                }
                if (vis[n] == i) {
                    m = dst[n], k = n;
                    cycle.push_back({});
                    do {
                        cycle.back().push_back(k);
                        go_cycle[k] = k, dst[k] = 0;
                        k = grp[k];
                    } while (k != n);
                }
                for (auto ne : tem)
                    if (go_cycle[ne] == -1)
                        go_cycle[ne] = go_cycle[n], dst[ne] = m - dst[ne] + dst[n];
            }
    }
    int N;
    vector<int> grp, dst, go_cycle;
    vector<vector<int>> cycle;
};
