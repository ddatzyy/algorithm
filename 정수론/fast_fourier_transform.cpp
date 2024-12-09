namespace fast_fourier_transform {
using cpx = complex<dll>;
void FFT(vector<cpx> &a, bool inv_fft = false)
{
    int N = a.size();
    vector<cpx> root(N / 2);  // root[0]=1
    for (int i = 1, j = 0; i < N; i++) {
        int bit = N / 2;
        while (j >= bit) j -= bit, bit >>= 1;
        if (i < (j += bit)) swap(a[i], a[j]);
    }
    long double ang = 2 * acosl(-1) / N * (inv_fft ? -1 : 1);
    for (int i = 0; i < N / 2; i++) root[i] = cpx(cosl(ang * i), sinl(ang * i));
    for (int i = 2; i <= N; i <<= 1) {
        int step = N / i;
        for (int j = 0; j < N; j += i)
            for (int k = 0; k < i / 2; k++) {
                cpx u = a[j + k], v = a[j + k + i / 2] * root[step * k];
                a[j + k] = u + v;
                a[j + k + i / 2] = u - v;
            }
    }
    if (inv_fft)
        for (int i = 0; i < N; i++) a[i] /= N;
}
vector<ll> Mul(const vector<ll> &_a, const vector<ll> &_b)
{
    vector<cpx> a(all(_a)), b(all(_b));  // (NTT) 2^19 700ms
    int N = 2;
    while (N < a.size() + b.size()) N <<= 1;
    a.resize(N);
    b.resize(N);
    FFT(a);
    FFT(b);
    for (int i = 0; i < N; i++) a[i] *= b[i];  // mod?
    vector<ll> ret(N);
    FFT(a, 1);  // NTT : just return a
    for (int i = 0; i < N; i++) ret[i] = llround(a[i].real());
    while (ret.size() > 1 && ret.back() == 0) ret.pop_back();
    return ret;
}
vector<ll> MulMod(const vector<ll> &a, const vector<ll> &b, const unsigned long long MOD = (119 << 23) + 1)
{  // (FFT) 2^19 1000ms
    int N = 2;
    while (N < a.size() + b.size()) N <<= 1;
    vector<cpx> v1(N), v2(N), r1(N), r2(N);
    for (int i = 0; i < a.size(); i++) v1[i] = cpx(a[i] >> 15, a[i] & 32767);
    for (int i = 0; i < b.size(); i++) v2[i] = cpx(b[i] >> 15, b[i] & 32767);
    FFT(v1);
    FFT(v2);
    for (int i = 0; i < N; i++) {
        int j = i ? N - i : i;
        cpx ans1 = (v1[i] + conj(v1[j])) * cpx(0.5, 0);
        cpx ans2 = (v1[i] - conj(v1[j])) * cpx(0, -0.5);
        cpx ans3 = (v2[i] + conj(v2[j])) * cpx(0.5, 0);
        cpx ans4 = (v2[i] - conj(v2[j])) * cpx(0, -0.5);
        r1[i] = (ans1 * ans3) + (ans1 * ans4) * cpx(0, 1);
        r2[i] = (ans2 * ans3) + (ans2 * ans4) * cpx(0, 1);
    }
    vector<ll> ret(N);
    FFT(r1, true);
    FFT(r2, true);
    for (int i = 0; i < N; i++) {
        ll av = llround(r1[i].real()) % MOD;
        ll bv = (llround(r1[i].imag()) + llround(r2[i].real())) % MOD;
        ll cv = llround(r2[i].imag()) % MOD;
        ret[i] = (av << 30) + (bv << 15) + cv;
        ret[i] %= MOD;
        ret[i] += MOD;
        ret[i] %= MOD;
    }
    while (ret.size() > 1 && ret.back() == 0) ret.pop_back();
    return ret;
}
template <char op>
void FWHT(vector<ll> &a, bool inv_fft, ll MOD)
{
    ll ta, tb, inv;
    int N = a.size();
    for (int i = 1, j = 0; i < N; i++) {
        int bit = N / 2;
        while (j >= bit) j -= bit, bit /= 2;
        if (i < (j += bit)) swap(a[i], a[j]);
    }
    for (int i = 2; i <= N; i *= 2) {
        int step = N / i;
        for (int j = 0; j < N; j += i) {
            for (int k = 0; k < i / 2; k++) {
                ll u = a[j + k], v = a[j + k + i / 2];
                if constexpr (op == '^')
                    a[j + k] = u + v, a[j + k + i / 2] = u - v;
                else if (op == '|')
                    a[j + k + i / 2] += inv_fft ? -u : u;
                else if (op == '&')
                    a[j + k] += inv_fft ? -v : v;
                if (a[j + k] >= MOD)
                    a[j + k] -= MOD;
                else if (a[j + k] < 0)
                    a[j + k] += MOD;
                if (a[j + k + i / 2] >= MOD)
                    a[j + k + i / 2] -= MOD;
                else if (a[j + k + i / 2] < 0)
                    a[j + k + i / 2] += MOD;
            }
        }
    }
    if (op == '^' && inv_fft) {
        inv = 1;
        ta = N, tb = MOD - 2;
        for (; tb; tb >>= 1, ta = ta * ta % MOD)
            if (tb & 1) inv = inv * ta % MOD;
        for (int i = 0; i < N; i++) a[i] = a[i] * inv % MOD;
    }
}
template <char op>
vector<ll> FWHT_Conv(vector<ll> a, vector<ll> b, ll MOD = (119 << 23) + 1)
{
    int n = max({(int)a.size(), (int)b.size() - 1, 1});  // 2^20 700ms
    if (__builtin_popcount(n) != 1) n = 1 << (__lg(n) + 1);
    a.resize(n);
    b.resize(n);
    FWHT<op>(a, false, MOD);
    FWHT<op>(b, false, MOD);
    for (int i = 0; i < n; i++) a[i] = a[i] * b[i] % MOD;
    FWHT<op>(a, true, MOD);
    return a;
}
// subset: C[k] = sum_{i and j = 0, i or j = k} A[i] * B[j]
vector<ll> SubsetConvolution(vector<ll> p, vector<ll> q, ll MOD = (119 << 23) + 1)
{  // Nlog2N
    int n = max({(int)p.size(), (int)q.size() - 1, 1}), w = __lg(n);
    if (__builtin_popcount(n) != 1) n = 1 << (w + 1);  // 2^20 4s
    p.resize(n);
    q.resize(n);
    vector<ll> res(n);  // SOS DP: 2.5s
    vector<vector<ll>> a(w + 1, vector<ll>(n)), b(a);
    for (int i = 0; i < n; i++) a[__builtin_popcount(i)][i] = p[i];
    for (int i = 0; i < n; i++) b[__builtin_popcount(i)][i] = q[i];
    for (int bit = 0; bit <= w; bit++) FWHT<'|'>(a[bit], false, MOD), FWHT<'|'>(b[bit], false, MOD);
    for (int bit = 0; bit <= w; bit++) {
        vector<ll> c(n);  // Warning : MOD
        for (int i = 0; i <= bit; i++)
            for (int j = 0; j < n; j++) c[j] += a[i][j] * b[bit - i][j] % MOD;
        for (auto &i : c) i %= MOD;
        FWHT<'|'>(c, true, MOD);
        for (int i = 0; i < n; i++)
            if (__builtin_popcount(i) == bit) res[i] = c[i];
    }
    return res;
}
}  // namespace fast_fourier_transform
