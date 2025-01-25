// need miller_rabin_pollard_rho
namespace fibonacci {
// fib[1] = 1, fib[2] = 1, ...
int64_t get_fibonacci(int64_t va, int64_t md)
{
    if (va < 2) return va % md;
    int64_t x, y, tm, nx, ny;
    x = 0, y = 1;
    for (int64_t i = 1LL << (63 - __builtin_clzll(va) - 1); i > 0; i /= 2) {
        nx = (__int128(x) * x % md + __int128(y) * y % md) % md;
        ny = __int128(2LL * x + y) * y % md;
        x = nx, y = ny;
        if (va & i) {
            tm = x + y;
            if (tm >= md) tm -= md;
            x = y, y = tm;
        }
    }
    return y;
}
int64_t pisano_period_prime(int64_t N)
{
    if (N == 2) return 3;
    if (N == 5) return 20;
    auto fac = miller_rabin_pollard_rho::factorize((N % 5 == 1 || N % 5 == 4) ? N - 1 : 2 * N + 2);
    map<int64_t, int> mp;
    for (auto ne : fac)
        mp[ne]++;
    vector<int64_t> tem;
    tem.push_back(1);
    for (auto [ne, cn] : mp) {
        int64_t va = 1;
        int sz = tem.size();
        for (int i = 0; i < cn; i++) {
            va *= ne;
            for (int p = 0; p < sz; p++)
                tem.push_back(tem[p] * va);
        }
    }
    sort(tem.begin(), tem.end());
    for (auto ne : tem)
        if (get_fibonacci(ne, N) == 0 and get_fibonacci(ne + 1, N) == 1)
            return ne;
    assert(0);
    return -1;
}
int64_t pisano_period(int64_t N)
{
    auto fac = miller_rabin_pollard_rho::factorize(N);
    map<int64_t, int> mp;
    for (auto ne : fac)
        mp[ne]++;
    int64_t ret = 1;
    for (auto [ne, cn] : mp) {
        int64_t va = pisano_period_prime(ne);
        for (int i = 1; i < cn; i++)
            va *= ne;
        ret = ret / __gcd(ret, va) * va;
    }
    return ret;
}
}  // namespace fibonacci