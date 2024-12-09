template <int64_t MOD>
struct mod_int {
    using mod_t = mod_int;

    constexpr mod_int() : N(0) {}
    constexpr mod_int(const int64_t &_N) : N((_N % MOD + MOD) % MOD) {};

    constexpr mod_t &operator+=(const mod_t &va)
    {
        N = (N + va.N) % MOD;
        return *this;
    }

    constexpr mod_t &operator-=(const mod_t &va)
    {
        N = (N + MOD - va.N) % MOD;
        return *this;
    }

    constexpr mod_t &operator*=(const mod_t &va)
    {
        N = (N * va.N) % MOD;
        return *this;
    }

    constexpr mod_t &operator/=(const mod_t &va)
    {
        N = (N * va.inverse()) % MOD;
        return *this;
    }

    constexpr mod_t operator+(const mod_t &va) const { return mod_t(*this) += va; }
    constexpr mod_t operator-(const mod_t &va) const { return mod_t(*this) -= va; }
    constexpr mod_t operator*(const mod_t &va) const { return mod_t(*this) *= va; }
    constexpr mod_t operator/(const mod_t &va) const { return mod_t(*this) /= va; }
    constexpr bool operator==(const mod_t &va) const { return N == va.N; }
    constexpr bool operator!=(const mod_t &va) const { return N != va.N; }
    constexpr mod_t operator-() const { return mod_t() - mod_t(*this); }
    constexpr mod_t operator+() const { return mod_t(*this); }

    friend ostream &operator<<(ostream &os, const mod_t &va)
    {
        return os << va.N;
    }

    friend istream &operator>>(istream &is, mod_t &va)
    {
        int64_t t;
        is >> t;
        va = mod_int<MOD>(t);
        return is;
    }

    constexpr mod_t pow(int64_t va) const
    {
        mod_t ret(1), mul(*this);
        while (va > 0) {
            if (va & 1) ret *= mul;
            mul *= mul;
            va >>= 1;
        }
        return ret;
    }

    constexpr mod_t inverse() const
    {  // exgcd
        int x1, y1, x2, y2;
        x1 = N, y1 = MOD, x2 = 1, y2 = 0;
        assert(gcd(x1, y1) == 1);
        while (y1 > 0) {
            x2 -= x1 / y1 * y2, x1 %= y1;
            swap(x1, y1), swap(x2, y2);
        }
        return mod_t(x2);
    }

    static constexpr int64_t get_mod() { return MOD; }

    int64_t N;

    static_assert(MOD <= int64_t(3'037'000'499) && MOD > 0, "invalid, mod is not in (0, 3'037'000'499]");
};
