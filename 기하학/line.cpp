struct line {
    using line_t = int64_t;
    array<line_t, 2> u, v;
    line() : u({1, 0}), v({0, 1}) {}
    line(line_t x1, line_t y1, line_t x2, line_t y2) : u({x1, y1}), v({x2, y2}) { assert(u != v); }

    static pair<long double, long double> intersection(line l1, line l2)
    {
        long double x, y, d;
        d = (l1.u[0] - l1.v[0]) * (l2.u[1] - l2.v[1]) - (l1.u[1] - l1.v[1]) * (l2.u[0] - l2.v[0]);
        x = (l1.u[0] * l1.v[1] - l1.u[1] * l1.v[0]) * (l2.u[0] - l2.v[0]) - (l1.u[0] - l1.v[0]) * (l2.u[0] * l2.v[1] - l2.u[1] * l2.v[0]);
        y = (l1.u[0] * l1.v[1] - l1.u[1] * l1.v[0]) * (l2.u[1] - l2.v[1]) - (l1.u[1] - l1.v[1]) * (l2.u[0] * l2.v[1] - l2.u[1] * l2.v[0]);
        assert(d > 1e-9 || d < -1e-9);
        return {x / d, y / d};
    }
    static bool is_on_line(line li, array<line_t, 2> c)
    {  // point-point : O
        line_t x1, y1, x2, y2;
        x1 = min(li.v[0], li.u[0]), y1 = min(li.v[1], li.u[1]), x2 = max(li.v[0], li.u[0]), y2 = max(li.v[1], li.u[1]);
        if (c[0] >= x1 && c[0] <= x2 && c[1] >= y1 && c[1] <= y2) return 1;
        return 0;
    };
    static bool is_intersect(line l1, line l2)
    {  // line-point, point-point : O
        line_t x, y;
        auto is_intersect_ccw = [](array<line_t, 2> a, array<line_t, 2> b, array<line_t, 2> c) {
            line_t x1, y1, x2, y2, cw;
            x1 = b[0] - a[0], y1 = b[1] - a[1], x2 = c[0] - a[0], y2 = c[1] - a[1];
            cw = x1 * y2 - x2 * y1, cw = (cw > 0) - (cw < 0);
            if (cw == 0) {
                if (is_on_line({a[0], a[1], b[0], b[1]}, c)) return cw;
                cw = 1;
            }
            return cw;
        };
        x = is_intersect_ccw(l1.v, l1.u, l2.v) * is_intersect_ccw(l1.v, l1.u, l2.u);
        y = is_intersect_ccw(l2.v, l2.u, l1.v) * is_intersect_ccw(l2.v, l2.u, l1.u);
        if ((x == -1 && y == -1) || !x || !y) return 1;
        return 0;
    }
    static bool is_on_line_without(line li, array<line_t, 2> c)
    {  // point-point : X
        line_t x1, y1, x2, y2;
        x1 = min(li.v[0], li.u[0]), y1 = min(li.v[1], li.u[1]), x2 = max(li.v[0], li.u[0]), y2 = max(li.v[1], li.u[1]);
        if (c[0] > x1 && c[0] < x2 && c[1] > y1 && c[1] < y2) return 1;
        return 0;
    };
    static bool is_intersect_without(line l1, line l2)
    {  // line-point, point-point : X
        line_t x1, y1, x2, y2;
        auto is_intersect_ccw = [](array<line_t, 2> a, array<line_t, 2> b, array<line_t, 2> c) {
            line_t x1, y1, x2, y2, cw;
            x1 = b[0] - a[0], y1 = b[1] - a[1], x2 = c[0] - a[0], y2 = c[1] - a[1];
            cw = x1 * y2 - x2 * y1, cw = (cw > 0) - (cw < 0);
            return cw;
        };
        x1 = is_intersect_ccw(l1.v, l1.u, l2.v), y1 = is_intersect_ccw(l1.v, l1.u, l2.u);
        x2 = is_intersect_ccw(l2.v, l2.u, l1.v), y2 = is_intersect_ccw(l2.v, l2.u, l1.u);
        if (x1 == 0 && y1 == 0 && (is_on_line_without(l1, l2.u) || is_on_line_without(l1, l2.u) || is_on_line_without(l2, l1.u) || is_on_line_without(l2, l1.v)))
            return 1;
        if (x1 * y1 == -1 && x2 * y2 == -1) return 1;
        return 0;
    }
};
