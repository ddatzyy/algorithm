struct point {
    using point_t = int64_t;

    point_t x, y, z;

    point() : x(0), y(0), z(0) {}
    point(point_t _x, point_t _y) : x(_x), y(_y), z(0) {}
    point(point_t _x, point_t _y, point_t _z) : x(_x), y(_y), z(_z) {}

    // distance ^ 2
    constexpr point_t hypot(point &p) const noexcept
    {
        point_t dx = p.x - x, dy = p.y - y;
        return dx * dx + dy * dy;
    }

    point operator+(const point &p) const noexcept { return point(x + p.x, y + p.y); }
    point operator-(const point &p) const noexcept { return point(x - p.x, y - p.y); }
    point &operator+=(const point &p) noexcept
    {
        x += p.x, y += p.y;
        return *this;
    }
    point &operator-=(const point &p) noexcept
    {
        x -= p.x, y -= p.y;
        return *this;
    }
    constexpr bool operator<(const point &p) const noexcept { return (x < p.x) || (x == p.x && y < p.y); }
    constexpr bool operator<=(const point &p) const noexcept { return !(p < *this); }
    constexpr bool operator>(const point &p) const noexcept { return p < *this; }
    constexpr bool operator>=(const point &p) const noexcept { return !(*this < p); }
    constexpr bool operator==(const point &p) const noexcept { return x == p.x && y == p.y; }
    constexpr bool operator!=(const point &p) const noexcept { return !(*this == p); }
    constexpr point_t operator*(const point &p) const noexcept { return x * p.x + y * p.y; }  // dot
    constexpr point_t operator/(const point &p) const noexcept { return x * p.y - p.x * y; }  // cross
};
