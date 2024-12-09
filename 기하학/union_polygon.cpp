struct union_polygon {
    using coord_t = long double;  // if you need to use get_union_area, coord_t must be float type
    using point_t = complex<coord_t>;
    void add_point(coord_t x, coord_t y) { pol.push_back({x, y}); }
    void add_point(point_t va) { pol.push_back(va); }
    coord_t set_counter_clock_wise(bool va = true)
    {
        coord_t ret;
        ret = get_area();
        if (va && ret < -eps)
            reverse(pol.begin(), pol.end());
        else if (!va && ret > eps)
            reverse(pol.begin(), pol.end());
        return ret;
    }
    coord_t get_area()
    {
        coord_t ret;
        ret = 0;
        for (int i = 0; i < pol.size(); i++)
            ret += cross(pol[i], pol[(i + 1) % pol.size()]);
        return ret / 2;
    }
    static coord_t get_union_area(vector<union_polygon> pols)
    {
        int n, m, cw1, cw2;
        coord_t le, be, ar1, ar2, ret;
        n = pols.size();
        vector<pair<array<point_t, 2>, int>> arr;
        for (int i = 0; i < n; i++) {
            pols[i].set_counter_clock_wise();
            auto& tem = pols[i].pol;
            m = tem.size();
            tem.push_back(tem[0]);
            for (int p = 0; p < m; p++)
                arr.push_back({{tem[p], tem[p + 1]}, i});
        }
        ret = 0;
        for (auto& [va1, id1] : arr) {
            vector<pair<coord_t, int>> swp;
            for (auto& [va2, id2] : arr)
                if (id1 != id2) {
                    cw1 = ccw(va1[0], va1[1], va2[0]), cw2 = ccw(va1[0], va1[1], va2[1]);
                    if (cw1 != cw2 && cw1 + cw2 <= 0) {
                        // 2 * (tr(va2[0], va2[1], va1[0]) - tr(va2[0], va2[1], va1[1])) ~= sq(va2[0], va1[0], va2[1], va1[1])
                        ar1 = cross(va2[1] - va2[0], va1[0] - va2[0]), ar2 = cross(va2[1] - va2[0], va1[1] - va2[0]);
                        swp.push_back({clamp<coord_t>(ar1 / (ar1 - ar2), 0, 1), !(cw1 + 1)});
                    }
                    else if (id1 < id2 && cw1 == 0 && cw2 == 0 && signum(dot(va1[1] - va1[0], va2[1] - va2[0])) > 0) {
                        // S - v1 = x * (v2 - v1) -> (S - v1) / (v2 - v1) = x
                        swp.push_back({clamp<coord_t>((real((va2[0] - va1[0]) / (va1[1] - va1[0]))), 0, 1), 0});
                        swp.push_back({clamp<coord_t>((real((va2[1] - va1[0]) / (va1[1] - va1[0]))), 0, 1), 1});
                    }
                }
            swp.push_back({1, 1});
            sort(swp.begin(), swp.end());
            m = 0, be = 0, le = 0;
            for (auto& ne : swp) {
                if (m == 0)
                    le += ne.first - be;
                if (ne.second == 0)
                    m++;
                else
                    m--;
                be = ne.first;
            }
            ret += cross(va1[0], va1[1]) * le;
        }
        return ret / 2;
    }
    static point_t reflect(const point_t va, const point_t bs, const point_t v)
    {
        point_t prj = dot(va - bs, v) / dot(v, v) * v;
        return bs + prj * point_t(2, 0) - (va - bs);
    }
    static coord_t cross(point_t a, point_t b) { return imag(conj(a) * b); }
    static coord_t dot(point_t a, point_t b) { return real(conj(a) * b); }
    static int signum(coord_t va) { return (va > eps) - (va < -eps); }
    static int ccw(point_t a, point_t b, point_t c) { return signum(cross(b - a, c - a)); }

   private:
    constexpr static coord_t eps = 1e-9;

    vector<point_t> pol;
};
