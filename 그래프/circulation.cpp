// you need a base flow class
// ex : dinic<flow_t>
template <typename flow_t>
struct circulation : base_flow_class {
    using base_flow = base_flow_class;

    circulation(int _N) : base_flow(_N + 2)
    {
        N = _N;
        cur_flow = -1, dem_flow = 0;
        dem.resize(N);
    }
    void add_edge(int u, int v, flow_t low, flow_t flw)
    {
        dem[u] += low, dem[v] -= low;
        base_flow::add_edge(u, v, flw - low);
    }
    void set_ST(int _S, int _T)
    {
        S = _S, T = _T;
        base_flow::add_edge(T, S, numeric_limits<flow_t>::max());
    }
    flow_t max_flow()
    {
        assert(S != -1 && T != -1);

        base_flow::set_ST(N, N + 1);
        dem_flow = 0;
        for (int i = 0; i < N; i++) {
            dem_flow += max(flow_t(0), dem[i]);
            if (dem[i] < 0)
                base_flow::add_edge(N, i, -dem[i]);
            else if (dem[i] > 0)
                base_flow::add_edge(i, N + 1, dem[i]);
        }
        return cur_flow = base_flow::max_flow();
    }
    bool check_is_circulation()
    {
        assert(cur_flow != -1);
        return dem_flow == cur_flow;
    }

   private:
    int N, S, T;
    flow_t cur_flow, dem_flow;
    vector<flow_t> dem;
};
