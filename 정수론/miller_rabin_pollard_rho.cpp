namespace miller_rabin_pollard_rho {
bool miller_rabin(long long va, vector<long long> pri)
{
    long long D, s, t, d, x;
    uint64_t mod = va, imod = va, t128 = -__uint128_t(va) % va, m1;
    s = __builtin_ctzll(va - 1);
    D = va - 1 >> s;
    for (int i = 0; i < 5; i++) imod *= 2 - mod * imod;
    imod = -imod;
    m1 = (t128 + __uint128_t(t128 * imod) * mod) >> 64;
    if (m1 >= va) m1 -= va;
    for (auto a : pri) {
        if (va <= a) return 1;
        a = (__uint128_t(a) * t128 + __uint128_t(a * t128 * imod) * mod) >> 64;
        if (a >= mod) a -= mod;
        x = m1;
        for (d = D; d > 0; d >>= 1) {
            if (d & 1) {
                x = (__uint128_t(a) * x + __uint128_t(a * x * imod) * mod) >> 64;
                if (x >= mod) x -= mod;
            }
            a = (__uint128_t(a) * a + __uint128_t(a * a * imod) * mod) >> 64;
            if (a >= mod) a -= mod;
        }
        if (x != m1) {
            for (t = 0; t < s; t++) {
                if (x == mod - m1) break;
                x = (__uint128_t(x) * x + __uint128_t(x * x * imod) * mod) >> 64;
                if (x >= mod) x -= mod;
            }
            if (t == s) return 0;
        }
    }
    return 1;
}
bool is_prime(long long va)
{
    if (va < 2 || (va > 2 && va % 2 == 0))
        return 0;
    else if (va < 4759123141LL)
        return miller_rabin(va, {2, 7, 61});
    else
        return miller_rabin(va, {2, 325, 9375, 28178, 450775, 9780504, 1795265022});
}
long long find_prime_factor(long long va)
{
    if (va % 2 == 0) return 2;
    uint64_t mod = va, imod = va;
    for (int i = 0; i < 5; i++) imod *= 2 - mod * imod;
    imod = -imod;
    long long m = pow(va, 0.15) + 1, y, g, q, r, k, x, ys, d, c = 1;
    auto f = [&](long long v) {
        v = (__uint128_t(v) * v + __uint128_t(v * v * imod) * mod) >> 64;
        if (v >= mod) v -= mod;
        v += c;
        if (v >= mod) v -= mod;
        return v;
    };
    while (++c) {
        y = k = 0, g = r = 1;
        while (g == 1) {
            x = y;
            while (k * 4 < 3 * r)
                y = f(y), k++;
            while (k < r && g == 1) {
                ys = y, q = 1;
                for (int i = 0; i < min(m, r - k); i++) {
                    y = f(y), d = x > y ? x - y : y - x, q = (__uint128_t(q) * d + __uint128_t(q * d * imod) * mod) >> 64;
                    if (q >= mod) q -= mod;
                }
                g = gcd(q, va), k += m;
            }
            k = r, r *= 2;
        }
        if (g == va) {
            g = 1, y = ys;
            while (g == 1)
                y = f(y), g = gcd(x > y ? x - y : y - x, va);
        }
        if (g == va)
            continue;
        else if (is_prime(g))
            return g;
        else if (is_prime(va / g))
            return va / g;
        else
            return find_prime_factor(g);
    }
    return 0;
}
vector<long long> factorize(long long va)
{
    vector<long long> ret;
    long long n;
    while (!is_prime(va) && va > 1) {
        n = find_prime_factor(va);
        while (va % n == 0)
            va /= n, ret.push_back(n);
    }
    if (va > 1)
        ret.push_back(va);
    sort(ret.begin(), ret.end());
    return ret;
}
}  // namespace miller_rabin_pollard_rho
