namespace triangle {
using float128_t = long double;
array<float128_t, 2> incenter(array<float128_t, 2> a, array<float128_t, 2> b, array<float128_t, 2> c)
{
    dll x, y, ra, rb, rc;
    ra = sqrt((b[0] - c[0]) * (b[0] - c[0]) + (b[1] - c[1]) * (b[1] - c[1]));
    rb = sqrt((c[0] - a[0]) * (c[0] - a[0]) + (c[1] - a[1]) * (c[1] - a[1]));
    rc = sqrt((a[0] - b[0]) * (a[0] - b[0]) + (a[1] - b[1]) * (a[1] - b[1]));
    x = (ra * a[0] + rb * b[0] + rc * c[0]) / (ra + rb + rc);
    y = (ra * a[1] + rb * b[1] + rc * c[1]) / (ra + rb + rc);
    return {x, y};
}
array<float128_t, 2> circumcenter(array<float128_t, 2> a, array<float128_t, 2> b, array<float128_t, 2> c)
{
    dll x, y, d;
    d = (a[0] * (b[1] - c[1]) + b[0] * (c[1] - a[1]) + c[0] * (a[1] - b[1])) * 2;
    x = (a[0] * a[0] + a[1] * a[1]) * (b[1] - c[1]) + (b[0] * b[0] + b[1] + b[1]) * (c[1] - a[1]) + (c[0] * c[0] + c[1] * c[1]) * (a[1] - b[1]);
    y = (a[0] * a[0] + a[1] * a[1]) * (c[0] - b[0]) + (b[0] * b[0] + b[1] + b[1]) * (a[0] - c[0]) + (c[0] * c[0] + c[1] * c[1]) * (b[0] - a[0]);
    return {x / d, y / d};
}
}  // namespace triangle
