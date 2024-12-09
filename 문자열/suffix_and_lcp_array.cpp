struct suffix_lcp {
    suffix_lcp(string& _str)
    {
        str = _str;
        n = str.length();
        count.resize(n + 1);
        temp.resize(n);
        _get_suffix_array();
        _get_lcp_array();
    }
    vector<int> get_suffix_array() { return suffix_arr; }
    vector<int> get_lcp_array() { return lcp_arr; }

   private:
    int n;
    string str;
    vector<int> suffix_arr, suffix_rank, lcp_arr, count, temp;
    inline bool rank_is_same(int loc1, int loc2, int len)
    {
        return suffix_rank[min(n, loc1 + len)] == suffix_rank[min(n, loc2 + len)];
    }
    inline void cal_base_rank()
    {
        string str_copy = str;
        sort(str_copy.begin(), str_copy.end());
        str_copy.erase(unique(str_copy.begin(), str_copy.end()), str_copy.end());
        for (int i = 0; i < n; i++) {
            suffix_arr[i] = i;
            suffix_rank[i] = upper_bound(str_copy.begin(), str_copy.end(), str[i]) - str_copy.begin();
        }
    }
    void counting_sort(int len)
    {
        fill(count.begin(), count.end(), 0);
        for (int i = 0; i < n; i++)
            count[suffix_rank[min(n, i + len)]]++;
        for (int i = 1; i <= n; i++)
            count[i] += count[i - 1];
        for (int i = n - 1; i >= 0; i--)
            temp[--count[suffix_rank[min(n, suffix_arr[i] + len)]]] = suffix_arr[i];
        suffix_arr = temp;
    }
    void _get_suffix_array()
    {
        suffix_arr.resize(n);
        suffix_rank.resize(n + 1);
        cal_base_rank();
        for (int len = 1; len < n; len *= 2) {
            counting_sort(len), counting_sort(0);
            temp[suffix_arr[0]] = 1;
            for (int i = 1; i < n; i++) {
                temp[suffix_arr[i]] = temp[suffix_arr[i - 1]];
                if (!rank_is_same(suffix_arr[i], suffix_arr[i - 1], 0) || !rank_is_same(suffix_arr[i], suffix_arr[i - 1], len))
                    temp[suffix_arr[i]]++;
            }
            suffix_rank = temp;
            if (suffix_rank[suffix_arr[n - 1]] == n)
                break;
        }
    }
    void _get_lcp_array()
    {
        int len;
        lcp_arr.resize(n);
        vector<int> inv_index(n);
        for (int i = 0; i < n; i++)
            inv_index[suffix_arr[i]] = i;
        len = 0;
        for (int i = 0; i < n; i++)
            if (inv_index[i] > 0) {
                while (max(i, suffix_arr[inv_index[i] - 1]) + len < n && str[i + len] == str[suffix_arr[inv_index[i] - 1] + len])
                    len++;
                lcp_arr[inv_index[i]] = len;
                len = max(0, len - 1);
            }
    }
};
