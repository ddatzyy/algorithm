/*
 * Author: ddatzyy
 * Date: 2024-12-14
 */
struct eertree {
    struct node {
        node()
            : l(-1), r(-1), par(-1), suf(-1), qsuf(-1), dif(-1), srl(-1), dp{-1, -1} {}
        node(int _l, int _r)
            : l(_l), r(_r), par(-1), suf(-1), qsuf(-1), dif(-1), srl(-1), dp{-1, -1} {}
        node(int _l, int _r, int _par, int _suf, int _qsuf)
            : l(_l), r(_r), par(_par), suf(_suf), qsuf(_qsuf), dif(-1), srl(-1), dp{-1, -1} {}

        int len() const { return r - l; }

        int l, r, par, suf, qsuf, dif, srl;
        array<int, 2> dp;
        map<int, int> nxt;
    };

    eertree()
    {
        arr.push_back({1, 0, -1, 0, 0});
        arr.push_back({1, 1, -1, 0, 0});
        idx.push_back(1);
        str.push_back(-inf_length);
        plen.push_back({0, inf_length});
        irev.push_back({-1, -1});
        cur_length = 0;
    }

    // update eertree and k-factorization
    void push(int va)
    {
        // eertree
        cur_length++;
        str.push_back(va);
        int lo = idx.back();
        if (str[cur_length - 1 - arr[lo].len()] != va) {
            while (lo > 1 && str[cur_length - 1 - arr[arr[lo].suf].len()] != va)
                lo = arr[lo].qsuf;
            lo = arr[lo].suf;
        }
        if (arr[lo].nxt.count(va) > 0) {
            idx.push_back(arr[lo].nxt[va]);
            pop_ok.push_back(0);
        }
        else {
            arr.push_back({cur_length - 1 - arr[lo].len(), cur_length + 1});
            arr.back().par = lo;
            arr[lo].nxt[va] = arr.size() - 1;
            if (arr.back().len() == 1)
                arr.back().suf = arr.back().qsuf = 1;
            else {
                lo = arr[lo].suf;
                if (str[cur_length - 1 - arr[lo].len()] != va) {
                    while (lo > 1 && str[cur_length - 1 - arr[arr[lo].suf].len()] != va)
                        lo = arr[lo].qsuf;
                    lo = arr[lo].suf;
                }
                lo = arr[lo].nxt[va];
                arr.back().suf = lo;
                if (arr[lo].suf > 1 && str[arr.back().r - 1 - arr[lo].len()] == str[arr[lo].r - 1 - arr[arr[lo].suf].len()])
                    arr.back().qsuf = arr[lo].qsuf;
                else
                    arr.back().qsuf = lo;
            }
            idx.push_back(arr.size() - 1);
            pop_ok.push_back(1);
        }

        // k-factorization
        arr.back().dif = arr.back().len() - arr[arr.back().suf].len();
        if (arr.back().dif == arr[arr.back().suf].dif)
            arr.back().srl = arr[arr.back().suf].srl;
        else
            arr.back().srl = arr.back().suf;
        plen.push_back({inf_length, inf_length});
        irev.push_back({-1, -1});
        lo = idx.back();
        pop_rb.push_back({});
        while (arr[lo].len() > 0) {
            int nt = arr[arr[lo].srl].len() + arr[lo].dif;
            for (int i = 0; i < 2; i++) {
                int tm = nt;
                if (arr[lo].dif == arr[arr[lo].suf].dif && plen[cur_length - tm][!i] > plen[cur_length - arr[arr[lo].suf].dp[!i] - arr[lo].dif][!i])
                    tm = arr[arr[lo].suf].dp[!i] + arr[lo].dif;
                pop_rb.back().push_back({lo, !i, arr[lo].dp[!i]});
                arr[lo].dp[!i] = tm;
                if (plen[cur_length - tm][!i] + 1 < plen[cur_length][i])
                    plen[cur_length][i] = plen[cur_length - tm][!i] + 1, irev[cur_length][i] = tm;
            }
            lo = arr[lo].srl;
        }
    }

    // roll back eertree
    void pop()
    {
        if (!cur_length)
            return;
        cur_length--;
        int va = str.back();
        str.pop_back();
        int lo = idx.back();
        idx.pop_back();
        if (pop_ok.back()) {
            arr[arr[lo].par].nxt.erase(va);
            arr.pop_back();
        }
        pop_ok.pop_back();
        plen.pop_back(), irev.pop_back();
        for (auto [x, y, z] : pop_rb.back())
            if (x < arr.size())
                arr[x].dp[y] = z;
        pop_rb.pop_back();
    }

    int palindromic_length() { return min(plen.back()[0], plen.back()[1]); }

    bool k_factorization(int k = -1)
    {
        if (k < 0) k = palindromic_length();
        if (k > cur_length || k < plen[cur_length][k & 1]) return false;
        return true;
    }

    vector<int> get_k_factorization(int k = -1)
    {
        if (k < 0) k = palindromic_length();
        vector<int> ret;
        if (!k_factorization(k))
            return ret;
        int n = cur_length;
        int m = k & 1;
        vector<int> tem;
        while (n > 0)
            tem.push_back(irev[n][m]), n -= irev[n][m], m = !m;
        reverse(tem.begin(), tem.end());
        k -= tem.size();
        for (int i = 0; i < tem.size(); i++)
            if (k > 0 && tem[i] > 2) {
                int va = min(k >> 1, (tem[i] - 1) >> 1);
                for (int p = 0; p < va; p++)
                    ret.push_back(1);
                ret.push_back(tem[i] - va * 2);
                for (int p = 0; p < va; p++)
                    ret.push_back(1);
                k -= va * 2;
            }
            else
                ret.push_back(tem[i]);
        if (k > 0) {
            tem.swap(ret);
            ret.clear();
            for (int i = 0; i < tem.size(); i++)
                if (k > 0 && tem[i] > 1) {
                    ret.push_back(1), ret.push_back(1);
                    k--;
                }
                else
                    ret.push_back(tem[i]);
        }
        return ret;
    }

    void print_k_factorization(int k = -1)
    {
        if (k < 0) k = palindromic_length();
        vector<int> ret = get_k_factorization(k);
        int n = 1;
        for (int i = 0; i < ret.size(); i++) {
            int m = n + ret[i];
            for (int p = n; p < m; p++)
                cout << char(str[p]);
            cout << " ";
            n = m;
        }
    }

    int cur_length;
    vector<int> str, idx;
    vector<array<int, 2>> plen, irev;
    vector<node> arr;

   private:
    constexpr static int inf_length = numeric_limits<int>::max() / 2;

    vector<bool> pop_ok;
    vector<vector<array<int, 3>>> pop_rb;
};
