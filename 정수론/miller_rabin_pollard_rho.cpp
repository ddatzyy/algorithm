namespace miller_rabin_pollard_rho {
using uil = __uint128_t;
bool miller_rabin(long long va, vector<long long> pri)
{
    long long d, s, t, x;
    uint64_t mod = va, imod = va, tm = -uil(va) % va;
    s = __builtin_ctzll(va - 1);
    d = va - 1 >> s;
    for (int i = 0; i < 5; i++) imod *= 2 - mod * imod;
    imod = -imod;
    uint64_t vm = (tm + uil(tm * imod) * mod) >> 64;
    if (vm >= va) vm -= va;
    for (auto ne : pri) {
        if (va <= ne) return 1;
        ne = (uil(ne) * tm + uil(ne * tm * imod) * mod) >> 64;
        if (ne >= mod) ne -= mod;
        x = vm;
        for (int i = d; i > 0; i >>= 1) {
            if (i & 1) {
                x = (uil(ne) * x + uil(ne * x * imod) * mod) >> 64;
                if (x >= mod) x -= mod;
            }
            ne = (uil(ne) * ne + uil(ne * ne * imod) * mod) >> 64;
            if (ne >= mod) ne -= mod;
        }
        if (x != vm) {
            for (t = 0; t < s; t++) {
                if (x == mod - vm) break;
                x = (uil(x) * x + uil(x * x * imod) * mod) >> 64;
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
    auto fx = [&](long long v) {
        v = (uil(v) * v + uil(v * v * imod) * mod) >> 64;
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
                y = fx(y), k++;
            while (k < r && g == 1) {
                ys = y, q = 1;
                for (int i = 0; i < min(m, r - k); i++) {
                    y = fx(y), d = x > y ? x - y : y - x;
                    q = (uil(q) * d + uil(q * d * imod) * mod) >> 64;
                    if (q >= mod) q -= mod;
                }
                g = gcd(q, va), k += m;
            }
            k = r, r *= 2;
        }
        if (g == va) {
            g = 1, y = ys;
            while (g == 1)
                y = fx(y), g = gcd(x > y ? x - y : y - x, va);
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
