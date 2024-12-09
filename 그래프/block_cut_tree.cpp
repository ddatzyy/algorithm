// need biconnected_component
struct block_cut_tree : public biconnected_component {
    block_cut_tree(int _N) : biconnected_component(_N), N(_N), BN(-1) {}
    void add_edge(int u, int v) { biconnected_component::add_edge(u, v); }
    void init()
    {
        biconnected_component::init();
        BN = biconnected_component::bcc.size();
        blk.resize(BN + N); // cut(i >= BN) -> blk(i < BN) -> cut(i >= BN) ...
        vector<bool> chk(N);
        for (int i = 0; i < BN; i++) {
            for (auto id : biconnected_component::bcc[i]) {
                for (auto ne : edg[id])
                    if (biconnected_component::is_cut_v[ne])
                        chk[ne] = 1;
            }
            for (auto id : biconnected_component::bcc[i]) {
                for (auto ne : edg[id])
                    if (biconnected_component::is_cut_v[ne])
                        chk[ne] = 0, blk[ne + BN].push_back(i), blk[i].push_back(ne + BN);
            }
        }
    }

    int BN, N;
    vector<vector<int>> blk;
};
