namespace time_converter {

// 0000-01-01 = day 1
constexpr int one_year = 365;
// 0 : not leap year, 1 : leap year
bool leap_year = true;
vector<vector<int>> month({{0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
                           {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}});
vector<vector<int>> month_sum({{0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365},
                               {0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366}});
bool is_leap(int y)
{
    assert(y >= 0);
    return !(y % 400) || (!(y % 4) && (y % 100));
}
int count_leap_year(int y)
{
    if (y < 0) return 0;
    return y / 400 + 1 + y / 4 - y / 100;
}
int year_to_day(int y, int m, int d)
{
    assert(y >= 0 && m > 0 && m < 13 && d > 0 && d <= month[is_leap(y)][m]);
    if (!leap_year) return y * one_year + month_sum[0][m - 1] + d;
    return y * one_year + count_leap_year(y - 1) + month_sum[is_leap(y)][m - 1] + d;
}
array<int, 3> day_to_year(int d)
{
    int st, en, md;
    array<int, 3> ret({-1, -1, -1});
    assert(d > 0);
    if (!leap_year) {
        ret[0] = (d - 1) / one_year, d = (d - 1) % one_year + 1;
        for (int i = 1; i <= 12; i++)
            if (month_sum[0][i] >= d) {
                ret[1] = i, ret[2] = d - month_sum[0][i - 1];
                break;
            }
    }
    else {
        st = (d - 1) / (one_year + 1), en = (d - 1) / one_year + 1;
        while (st + 1 < en) {
            md = (st + en) >> 1;
            if (year_to_day(md, 1, 1) > d)
                en = md;
            else
                st = md;
        }
        ret[0] = md;
        for (int i = 1; i <= 12; i++)
            if (year_to_day(md, i, month[is_leap(md)][i]) >= d) {
                ret[1] = i, ret[2] = month[is_leap(md)][i] - (year_to_day(md, i, month[is_leap(md)][i]) - d);
                break;
            }
    }
    assert(ret[0] >= 0);
    return ret;
}

// hour : minute : second
constexpr int one_hour = 60, one_minute = 60;
struct time_od {
    time_od() : H(0), M(0), S(0) {}
    time_od(int h, int m, int s) : H(h), M(m), S(s) { normalize(); }
    time_od(array<int, 3> va) : H(va[0]), M(va[1]), S(va[2]) { normalize(); }
    static int time_to_s(int h, int m, int s)
    {
        assert(m >= 0 && m < one_hour && s >= 0 && s < one_minute);
        return (h * one_hour + m) * one_minute + s;
    }
    static int time_to_s(time_od va)
    {
        assert(va.M >= 0 && va.M < one_hour && va.S >= 0 && va.S < one_minute);
        return (va.H * one_hour + va.M) * one_minute + va.S;
    }
    static int time_to_s(array<int, 3> va)
    {
        assert(va[1] >= 0 && va[1] < one_hour && va[2] >= 0 && va[2] < one_minute);
        return (va[0] * one_hour + va[1]) * one_minute + va[2];
    }

    static time_od s_to_time(int s)
    {
        assert(s >= 0);
        return {s / (one_minute * one_hour), s % (one_minute * one_hour) / one_hour, s % one_minute};
    }

    time_od &operator+=(const time_od &va) { return *this = s_to_time(time_to_s(*this) + time_to_s(va)); }
    time_od &operator-=(const time_od &va) { return *this = s_to_time(time_to_s(*this) - time_to_s(va)); }
    time_od operator+(const time_od &va) const { return time_od(*this) += va; }
    time_od operator-(const time_od &va) const { return time_od(*this) -= va; }
    bool operator==(const time_od &va) const { return array<int, 3>({H, M, S}) == array<int, 3>({va.H, va.M, va.S}); }
    bool operator!=(const time_od &va) const { return !(*this == va); }

    friend ostream &operator<<(ostream &os, const time_od &va)
    {
        return os << "(" << va.H << ":" << va.M << ":" << va.S << ")";
    }

    void normalize()
    {
        M += S / one_minute, S %= one_minute;
        H += M / one_hour, M %= one_hour;
    }

    int H, M, S;
};

};  // namespace time_converter