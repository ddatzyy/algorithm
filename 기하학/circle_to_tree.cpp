// !!!!! you can't use it when two circles intersect at two points !!!!!
template <typename T>
vector<int> circle_to_tree(int n, vector<array<T, 3>>& arr)  // arr : 1-index
{
    struct circle {
        circle(int64_t _x, int64_t _y, int64_t _z, int _id) : x(_x), y(_y), z(_z), id(_id) {}
        inline static int64_t dst(const int64_t va) { return va * va; }
        inline int64_t dst(const circle& va) const { return dst(x - va.x) + dst(y - va.y); }
        inline int64_t fx(const int64_t va) const { return z * z - (va - x) * (va - x); }
        bool operator<(const circle& va) const
        {
            if (va.y == y)
                return z > va.z;
            else if ((x - z < va.x - va.z ? fx(va.x - va.z) : va.fx(x - z)) < dst(y - va.y))
                return y < va.y;
            else
                return y + z >= va.y + va.z && y - z <= va.y - va.z;
        }
        int64_t x, y, z;
        int id;
    };
    int m, sp;
    vector<tuple<T, bool, T, int>> swp;
    for (int i = 1; i <= n; i++) {
        swp.push_back({arr[i][0] - arr[i][2], 1, -arr[i][2], i});
        swp.push_back({arr[i][0] + arr[i][2], 0, arr[i][2], i});
    }
    sort(swp.begin(), swp.end());
    sp = log2(n + 1) + 1;
    vector<vector<int>> spt(n + 1, vector<int>(sp, 0));
    set<circle> st;
    function<bool(int, int)> in_circle = [&](int a, int b) -> bool {
        return !(arr[b][2] > arr[a][2] || circle::dst(arr[a][0] - arr[b][0]) + circle::dst(arr[a][1] - arr[b][1]) >= circle::dst(arr[a][2] + arr[b][2]));
    };
    for (auto [lo, qr, dt, id] : swp)
        if (!qr)
            st.erase({arr[id][0], arr[id][1], arr[id][2], id});
        else {
            auto [it, _] = st.insert({arr[id][0], arr[id][1], arr[id][2], id});
            if (it != st.begin()) {
                m = (*prev(it)).id;
                if (!in_circle(m, id)) {
                    for (int i = sp - 1; i >= 0; i--)
                        if (spt[m][i] > 0 && !in_circle(spt[m][i], id))
                            m = spt[m][i];
                    m = spt[m][0];
                }
                spt[id][0] = m;
            }
            for (int i = 1; i < sp; i++)
                spt[id][i] = spt[spt[id][i - 1]][i - 1];
        }
    vector<int> ret(n + 1, -1);
    for (int i = 1; i <= n; i++)
        ret[i] = spt[i][0];
    return ret;
}
