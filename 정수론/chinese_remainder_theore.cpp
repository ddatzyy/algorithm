array<ll, 3> crt(ll md1, ll va1, ll md2, ll va2)
{  // ? = (va1 mod md1) and (va2 mod md2)
    ll n, x, y;
    pil va;
    n = ((va2 - va1) % md2 + md2) % md2;
    if (n % gcd(md1, md2) > 0)
        return {0, 0, 0};
    va = exgcd(md1 / gcd(md1, md2), md2 / gcd(md1, md2));
    va.first *= n / gcd(md1, md2);
    va.second = md2 / gcd(md1, md2);
    va.first = (va.first % va.second + va.second) % va.second;
    x = va.second * md1, y = (va.first * md1 + va1) % x;
    return {1, x, y};  // answer = y mod x
}
