struct dynamic_bitset {
    dynamic_bitset(uint64_t _N) : N(_N)
    {
        arr.resize((N - 1) / 64 + 1, 0);
        if (N % 64 == 0)
            del_carry = max64;
        else
            del_carry = (1ULL << (N % 64)) - 1ULL;
    }

    struct proxy_bit {
        dynamic_bitset& bt;
        int idx;
        proxy_bit(dynamic_bitset& _bt, int _idx) : bt(_bt), idx(_idx) {}

        operator bool() const { return (bt.arr[idx / 64] >> (idx % 64)) & 1; }

        proxy_bit& operator=(bool va)
        {
            if (va)
                bt.set(idx);
            else
                bt.reset(idx);
            return *this;
        }

        proxy_bit& operator=(const proxy_bit& va) { return *this = static_cast<bool>(va); }
    };

    bool operator[](int lo) const
    {
        assert(lo < N);
        return (arr[lo / 64] >> (lo % 64)) & 1;
    }

    proxy_bit operator[](int lo)
    {
        assert(lo < N);
        return proxy_bit(*this, lo);
    }

    const dynamic_bitset& operator<<=(const int& va)
    {
        left(va);
        return *this;
    }

    const dynamic_bitset& operator>>=(const int& va)
    {
        right(va);
        return *this;
    }

    const dynamic_bitset& operator|=(const dynamic_bitset& va)
    {
        assert(va.N == N);
        for (int i = 0; i < arr.size(); i++)
            arr[i] |= va.arr[i];
        return *this;
    }

    const dynamic_bitset& operator&=(const dynamic_bitset& va)
    {
        assert(va.N == N);
        for (int i = 0; i < arr.size(); i++)
            arr[i] &= va.arr[i];
        return *this;
    }

    const dynamic_bitset& operator^=(const dynamic_bitset& va)
    {
        assert(va.N == N);
        for (int i = 0; i < arr.size(); i++)
            arr[i] ^= va.arr[i];
        return *this;
    }

    dynamic_bitset operator|(const dynamic_bitset& va) const { return dynamic_bitset(*this) |= va; }
    dynamic_bitset operator&(const dynamic_bitset& va) const { return dynamic_bitset(*this) &= va; }
    dynamic_bitset operator^(const dynamic_bitset& va) const { return dynamic_bitset(*this) ^= va; }
    dynamic_bitset operator<<(const int& va) const { return dynamic_bitset(*this) <<= va; }
    dynamic_bitset operator>>(const int& va) const { return dynamic_bitset(*this) >>= va; }
    bool operator==(const dynamic_bitset& va) const
    {
        assert(size() == va.size());
        return equal(arr.begin(), arr.end(), va.arr.begin());
    }
    bool operator!=(const dynamic_bitset& va) const { return !(*this == va); }

    friend ostream& operator<<(ostream& os, const dynamic_bitset& va)
    {
        for (int i = (va.N - 1) % 64; i >= 0; i--)
            os << char('0' + ((va.arr.back() >> i) & 1));
        for (int i = va.arr.size() - 2; i >= 0; i--)
            for (int p = 63; p >= 0; p--)
                os << char('0' + ((va.arr[i] >> p) & 1));
        return os;
    }

    void set(int lo = -1)
    {
        assert(lo < N);
        if (lo < 0) {
            fill(arr.begin(), arr.end(), max64);
            arr.back() = del_carry;
        }
        else
            arr[lo / 64] |= (1ULL << (lo % 64));
    }

    void reset(int lo = -1)
    {
        assert(lo < N);
        if (lo < 0)
            fill(arr.begin(), arr.end(), 0ULL);
        else
            arr[lo / 64] &= ~(1ULL << (lo % 64));
    }

    void flip(int lo = -1)
    {
        assert(lo < N);
        if (lo < 0) {
            for (int i = 0; i < arr.size(); i++) arr[i] ^= max64;
            arr.back() &= del_carry;
        }
        else
            arr[lo / 64] ^= (1ULL << (lo % 64));
    }

    int size() const { return N; }

    int count() const
    {
        int ret = 0;
        for (auto v : arr) ret += __builtin_popcountll(v);
        return ret;
    }

    bool all_bit() const
    {
        if (arr.back() < del_carry) return false;
        for (int i = 0; i < arr.size() - 1; i++)
            if (arr[i] < max64) return false;
        return true;
    }

    bool any_bit() const
    {
        for (auto v : arr)
            if (v > 0) return true;
        return false;
    }

    bool none_bit() const { return !any_bit(); }

   private:
    static constexpr uint64_t max64 = 0ULL - 1ULL;
    uint64_t N, del_carry;

    vector<uint64_t> arr;

    void left(int va)
    {
        int n = va / 64;
        int shift = va % 64;
        if (n > 0) {
            for (int i = arr.size() - 1; i >= 0; i--) {
                if (i - n >= 0)
                    arr[i] = arr[i - n];
                else
                    arr[i] = 0;
            }
        }
        if (shift > 0) {
            uint64_t carry = 0;
            for (int i = 0; i < arr.size(); i++) {
                uint64_t next_carry = arr[i] >> (64 - shift);
                arr[i] = (arr[i] << shift) | carry;
                carry = next_carry;
            }
        }
        arr.back() &= del_carry;
    }

    void right(int va)
    {
        int n = va / 64;
        int shift = va % 64;
        if (n > 0) {
            for (int i = 0; i < arr.size(); i++) {
                if (i + n < arr.size())
                    arr[i] = arr[i + n];
                else
                    arr[i] = 0;
            }
        }
        if (shift > 0) {
            uint64_t carry = 0;
            for (int i = arr.size() - 1; i >= 0; i--) {
                uint64_t next_carry = arr[i] << (64 - shift);
                arr[i] = (arr[i] >> shift) | carry;
                carry = next_carry;
            }
        }
    }
};
