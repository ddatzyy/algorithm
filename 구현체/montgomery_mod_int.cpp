template <uint32_t MOD>
struct montgomery_mod_int {
    using mod_t = montgomery_mod_int;

    constexpr montgomery_mod_int() : N(0) {}
    constexpr montgomery_mod_int(const int64_t &_N) : N(reduce(uint64_t(_N % MOD + MOD) * M64)) {};

    constexpr mod_t &operator+=(const mod_t &va)
    {
        if (int(N += va.N - 2 * MOD) < 0) N += 2 * MOD;
        return *this;
    }

    constexpr mod_t &operator-=(const mod_t &va)
    {
        if (int(N -= va.N) < 0) N += 2 * MOD;
        return *this;
    }

    constexpr mod_t &operator*=(const mod_t &va)
    {
        N = reduce(uint64_t(N) * va.N);
        return *this;
    }

    constexpr mod_t &operator/=(const mod_t &va)
    {
        *this *= va.inverse();
        return *this;
    }

    constexpr mod_t operator+(const mod_t &va) const { return mod_t(*this) += va; }
    constexpr mod_t operator-(const mod_t &va) const { return mod_t(*this) -= va; }
    constexpr mod_t operator*(const mod_t &va) const { return mod_t(*this) *= va; }
    constexpr mod_t operator/(const mod_t &va) const { return mod_t(*this) /= va; }
    constexpr friend mod_t operator+(const int64_t iva, const mod_t &va) { return mod_t(iva) + va; }
    constexpr friend mod_t operator-(const int64_t iva, const mod_t &va) { return mod_t(iva) - va; }
    constexpr friend mod_t operator*(const int64_t iva, const mod_t &va) { return mod_t(iva) * va; }
    constexpr friend mod_t operator/(const int64_t iva, const mod_t &va) { return mod_t(iva) / va; }
    constexpr bool operator==(const mod_t &va) const { return (N >= MOD ? N - MOD : N) == (va.N >= MOD ? va.N - MOD : va.N); }
    constexpr bool operator!=(const mod_t &va) const { return (N >= MOD ? N - MOD : N) != (va.N >= MOD ? va.N - MOD : va.N); }
    constexpr mod_t operator-() const { return mod_t() - mod_t(*this); }
    constexpr mod_t operator+() const { return mod_t(*this); }

    friend ostream &operator<<(ostream &os, const mod_t &va)
    {
        return os << va.get();
    }

    friend istream &operator>>(istream &is, mod_t &va)
    {
        int64_t t;
        is >> t;
        va = montgomery_mod_int<MOD>(t);
        return is;
    }

    static constexpr uint32_t reduce(const uint64_t &va) { return (va + uint64_t(uint32_t(va) * uint32_t(-R)) * MOD) >> 32; }

    constexpr mod_t pow(uint64_t va) const
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
        x1 = get(), y1 = MOD, x2 = 1, y2 = 0;
        while (y1 > 0) {
            x2 -= x1 / y1 * y2, x1 %= y1;
            swap(x1, y1), swap(x2, y2);
        }
        return mod_t(x2);
    }

    constexpr uint32_t get() const
    {
        uint32_t ret = reduce(N);
        return ret >= MOD ? ret - MOD : ret;
    }

    static constexpr uint32_t get_mod() { return MOD; }

    /*
    MOD * R = 1 mod (1 << 32)
    {f(R) = 1 / R - MOD -> 0}, {f'(R) = - 1 / R^2}
    R_{n+1} = R_{n} - f(R_{n}) / f'(R_{n}) = R_{n} * (2 - MOD * R_{n})
    */
    static constexpr uint32_t get_R()
    {
        uint32_t ret = MOD;
        for (int i = 0; i < 4; ++i) ret *= 2 - MOD * ret;
        return ret;
    }

    uint32_t N;
    static constexpr uint32_t R = get_R();
    static constexpr uint32_t M64 = -uint64_t(MOD) % MOD;
    static_assert(MOD < (1 << 30), "invalid, mod >= 2 ^ 30");
    static_assert((MOD & 1) == 1, "invalid, mod % 2 == 0");
    static_assert(R * MOD == 1, "invalid, MOD * R != 1 mod (1 << 32)");
};
