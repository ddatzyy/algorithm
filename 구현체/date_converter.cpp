// 0000-01-01 = day 1
namespace date_converter {
bool leap_year = true;
int one_year = 365;
// 0 : not leap year, 1 : leap year
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
};  // namespace date_converter
