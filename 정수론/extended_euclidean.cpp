pair<int64_t, int64_t> exgcd(int64_t a, int64_t b)
{  // a * x + b * y = 1 -> return {x, y}
    pair<int64_t, int64_t> ret;
    if (b == 0) return {1, 0};
    ret = exgcd(b, a % b);
    return {ret.second, ret.first - a / b * ret.second};
}
