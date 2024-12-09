// i must fix it later...
namespace line_pool_intersect {
struct l_point {
    int x, y;
    l_point(int _x, int _y) : x(_x), y(_y) {}
    bool operator==(l_point b) const { return x == b.x && y == b.y; }
    bool operator<(l_point b) const { return x < b.x || x == b.x && y < b.y; }
    l_point operator-(l_point b) const { return {x - b.x, y - b.y}; }
    ll operator|(l_point b) const { return ll(x) * b.y - ll(y) * b.x; }
};
bool lines_intersect_with_point(int N, vector<l_point> A)  // line(0, N), line(1, N + 1) ...
{
    for (int i = 0; i < N; i++)
        if (A[N + i] < A[i]) swap(A[i], A[N + i]);
    vector<int> I(N * 2);
    iota(I.begin(), I.end(), 0);
    sort(I.begin(), I.end(), [&](int i, int j) { return A[i] < A[j] || !(A[j] < A[i]) && i < j; });
    auto cmp = [&](int i, int j) {
        return (A[j] < A[i] || !(A[i] < A[j]) && j < i ? A[j] - A[i] | A[j] - A[j + N] : A[i] - A[i + N] | A[i] - A[j]) < 0;
    };
    auto chk = [&](int i, int j) {
        if ((A[i] - A[j + N] | A[i] - A[i + N]) <= 0 && (A[j] - A[j + N] | A[j] - A[i + N]) <= 0)
            return 1;
        return 0;
    };
    set<int, decltype(cmp)> S(cmp);
    for (int i : I)
        if (i < N) {
            auto [it, ok] = S.insert(i);
            if (!ok)
                return 1;
            if (it != S.begin())
                if (chk(*prev(it), i))
                    return 1;
            if (++it != S.end())
                if (chk(i, *it))
                    return 1;
        }
        else {
            auto it = S.erase(S.lower_bound(i));
            if (it != S.begin() && it != S.end())
                if (chk(*prev(it), *it))
                    return 1;
        }
    return 0;
}
bool lines_intersect_without_point(int N, vector<l_point> A)  // line(0, N), line(1, N + 1) ...
{
    for (int i = 0; i < N; i++)
        if (A[i + N] < A[i]) swap(A[i], A[i + N]);
    vector<int> I(N * 2);
    iota(I.begin(), I.end(), 0);
    sort(I.begin(), I.end(), [&](int i, int j) { return A[i] < A[j] || A[i] == A[j] && i > j; });
    auto cmp = [&](int i, int j) {
        return (A[j] - A[i] | A[j] - A[j + N]) < 0 || !(A[j] - A[i] | A[j] - A[j + N]) && (A[i] - A[i + N] | A[j] - A[j + N]) < 0;
    };
    auto chk = [&](int i, int j) {
        ll a = A[i] - A[i + N] | A[i] - A[j + N];
        if (a < 0) return 0;
        ll b = A[j] - A[j + N] | A[j] - A[i + N];
        if (b > 0) return 0;
        if (a || b ? !(A[i] == A[j]) : !(A[i] - A[i + N] | A[j] - A[j + N]))
            return 1;
        return 0;
    };
    multiset<int, decltype(cmp)> S(cmp);
    vector<decltype(S)::iterator> It(N);
    for (int i : I)
        if (i < N) {
            auto it = It[i] = S.insert(i);
            if (it != S.begin())
                if (chk(*prev(it), i))
                    return 1;
            if (++it != S.end())
                if (chk(i, *it))
                    return 1;
        }
        else {
            auto it = S.erase(It[i - N]);
            if (it != S.begin() && it != S.end())
                if (chk(*prev(it), *it))
                    return 1;
        }
    return 0;
}
}  // namespace line_pool_intersect
