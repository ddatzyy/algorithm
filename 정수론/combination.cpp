
template <typename comb_t>
struct combination {
    combination(int _N) : N(_N)
    {
        fac.resize(N);
        ifac.resize(N);
        fac[0] = ifac[0] = 1;
        for (int i = 1; i < fac.size(); i++)
            fac[i] = fac[i - 1] * i;
        ifac.back() = 1 / fac.back();
        for (int i = ifac.size() - 2; i > 0; i--)
            ifac[i] = ifac[i + 1] * (i + 1);
    }
    comb_t power(int64_t a, int64_t b)
    {
        comb_t ret = 1;
        while (b) {
            if (b & 1) ret = ret * a;
            a = a * a, b >>= 1;
        }
        return ret;
    }
    comb_t nCr(int a, int b)
    {
        assert(max(a, b) < N);
        if (a < b) return 0;
        return fac[a] * ifac[b] * ifac[a - b];
    }
    comb_t nHr(int a, int b) { return nCr(a + b - 1, b); }
    comb_t factorial(int a) { return fac[a]; };
    comb_t inverse_factorial(int a) { return ifac[a]; }

   private:
    int N;
    vector<comb_t> fac, ifac;
};
