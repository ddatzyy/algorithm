struct rational {
    rational() : x(1), y(0) {}
    rational(int64_t _x, int64_t _y) : x(_x), y(_y) {}  // (down : x, up : y) -> (y / x)

    void normalization()
    {
        if (y == 0)
            x = 1;
        else {
            int64_t tm = gcd(x, y);
            x /= tm, y /= tm;
            if (x < 0) x = -x, y = -y;
        }
    }

    rational& operator+=(const rational& va)
    {
        x *= va.x, y = y * va.x + va.y * x;
        return *this;
    }

    rational& operator-=(const rational& va)
    {
        x *= va.x, y = y * va.x - va.y * x;
        return *this;
    }

    rational& operator*=(const rational& va)
    {
        x *= va.x, y *= va.y;
        return *this;
    }

    rational& operator/=(const rational& va)
    {
        x *= va.y, y *= va.x;
        return *this;
    }

    rational operator+(const rational& va) const { return rational(*this) += va; }
    rational operator-(const rational& va) const { return rational(*this) -= va; }
    rational operator*(const rational& va) const { return rational(*this) *= va; }
    rational operator/(const rational& va) const { return rational(*this) /= va; }
    bool operator<(const rational& va) const { return y * va.x < va.y * x; }
    bool operator>(const rational& va) const { return va < *this; }
    bool operator<=(const rational& va) const { return !(va < *this); }
    bool operator>=(const rational& va) const { return !(*this < va); }
    bool operator==(const rational& va) const { return y * va.x == va.y * x; }
    bool operator!=(const rational& va) const { return !(*this == va); }

    friend ostream& operator<<(ostream& os, const rational& va)
    {
        os << "(" << va.x << ", " << va.y << ")";
        return os;
    }

    int64_t x, y;
};
