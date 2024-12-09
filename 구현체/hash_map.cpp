#include <bits/extc++.h>

struct splitmix64_hash { // do not use class as hash map
    static uint64_t splitmix64(uint64_t va)
    {
        va += 0x9e3779b97f4a7c15;
        va = (va ^ (va >> 30)) * 0xbf58476d1ce4e5b9;
        va = (va ^ (va >> 27)) * 0x94d049bb133111eb;
        return va ^ (va >> 31);
    }

    template <typename T>
    typename enable_if<is_arithmetic<T>::value, size_t>::type
    operator()(T va) const
    {
        static const uint64_t FIXED_RANDOM_1 = splitmix64_hash_random(splitmix64_hash_seed);
        return splitmix64(static_cast<uint64_t>(va) + FIXED_RANDOM_1);
    }

    template <typename T>
    typename enable_if<!is_arithmetic<T>::value, size_t>::type
    operator()(const T &va) const
    {
        static const uint64_t FIXED_RANDOM_2 = splitmix64_hash_random(splitmix64_hash_seed);
        uint64_t hash = FIXED_RANDOM_2;
        for (const auto &ne : va)
            hash ^= splitmix64(static_cast<uint64_t>(ne) + hash);
        return hash;
    }

    static inline mt19937 splitmix64_hash_seed{static_cast<unsigned int>(chrono::steady_clock::now().time_since_epoch().count())};
    static inline uniform_int_distribution<uint64_t> splitmix64_hash_random{static_cast<uint64_t>(1e8), static_cast<uint64_t>(1e12)};
};
template <typename type1, typename type2, typename hash_t = splitmix64_hash>
using hash_map = __gnu_pbds::gp_hash_table<type1, type2, hash_t>;
