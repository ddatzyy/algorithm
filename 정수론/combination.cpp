struct combination {
    combination(int64_t _N, int64_t _md) : N(_N), md(_md)
    {
        fac.resize(N);
        ifac.resize(N);
        fac[0] = ifac[0] = 1;
        for (int i = 1; i < fac.size(); i++)
            fac[i] = fac[i - 1] * i % md;
        ifac.back() = power(fac.back(), md - 2);
        for (int i = ifac.size() - 2; i > 0; i--)
            ifac[i] = ifac[i + 1] * (i + 1) % md;
    }
    int64_t power(int64_t a, int64_t b)
    {
        int64_t ret = 1;
        while (b) {
            if (b & 1) ret = ret * a % md;
            a = a * a % md, b >>= 1;
        }
        return ret;
    }
    int64_t inverse(int64_t a)
    {
        if (a > 0 && a < N) return ifac[a] * fac[a - 1] % md;
        return power(a, md - 2);
    }
    int64_t nCr(int64_t a, int64_t b)
    {
        assert(max(a, b) < N);
        if (a < b) return 0;
        return fac[a] * ifac[b] % md * ifac[a - b] % md;
    }
    int64_t nHr(int64_t a, int64_t b) { return nCr(a + b - 1, b); }
    int64_t factorial(int64_t a) { return fac[a]; };
    int64_t inverse_factorial(int64_t a) { return ifac[a]; }

   private:
    int64_t N, md;
    vector<int64_t> fac, ifac;
};