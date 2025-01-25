// need pollard_rho
namespace euler_phi_function {
long long get_phi(long long va)
{  // if gcd(a, b) = 1 -> a^k mod b = a^(k mod 𝜙(b)) mod b
    if (va == 1)
        return 1;
    vector<long long> fac = miller_rabin_pollard_rho::factorize(va);
    long long ret = 1, be = 0;
    for (auto ne : fac) {
        if (ne != be)
            ret *= ne - 1, be = ne;
        else
            ret *= ne;
    }
    return ret;
}
struct phi_array {
    phi_array(int _N) : N(_N)
    {
        phi.resize(N + 1);
        iota(phi.begin(), phi.end(), 0);
        for (int i = 2; i <= N; i++) {
            if (phi[i] != i) continue;
            for (int p = i; p <= N; p += i)
                phi[p] -= phi[p] / i;
        }
    }

   private:
    vector<int> phi;
    int N;
};
}  // namespace euler_phi_function