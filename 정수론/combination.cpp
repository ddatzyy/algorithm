struct combination {
    combination(ll _N, ll _md) : N(_N), md(_md)
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
    ll power(ll a, ll b)
    {
        ll ret = 1;
        while (b) {
            if (b & 1) ret = ret * a % md;
            a = a * a % md, b >>= 1;
        }
        return ret;
    }
    ll inverse(ll a)
    {
        if (a > 0 && a < N) return ifac[a] * fac[a - 1] % md;
        return power(a, md - 2);
    }
    ll nCr(ll a, ll b)
    {
        assert(max(a, b) < N);
        if (a < b) return 0;
        return fac[a] * ifac[b] % md * ifac[a - b] % md;
    }
    ll nHr(ll a, ll b) { return nCr(a + b - 1, b); }
    ll factorial(ll a) { return fac[a]; };
    ll inverse_factorial(ll a) { return ifac[a]; }

   private:
    ll N, md;
    vector<ll> fac, ifac;
};
