template <typename T>
struct matrix {
    matrix() : row(0), col(0) {}
    matrix(int _row, int _col) : row(_row), col(_col) { arr.resize(row, vector<T>(col)); }
    matrix(int _row, int _col, T _va) : row(_row), col(_col) { arr.resize(row, vector<T>(col, _va)); }
    matrix(vector<vector<T>> _arr) : row(_arr.size()), col(_arr.begin()->size()), arr(_arr) {}
    matrix(initializer_list<initializer_list<T>> _arr) : arr(_arr.begin(), _arr.end()), row(_arr.size()), col(_arr.begin()->size()) {}

    void identity()
    {
        assert(row == col);
        clear(0);
        for (int i = 0; i < row; i++)
            arr[i][i] = 1;
    }
    void clear(T va)
    {
        for (int i = 0; i < row; i++)
            for (int p = 0; p < col; p++)
                arr[i][p] = va;
    }
    matrix pow(int64_t va)
    {
        assert(row == col);
        matrix pw(arr), ret(row, col);
        ret.identity();
        while (va > 0) {
            if (va & 1)
                ret *= pw;
            pw *= pw;
            va >>= 1;
        }
        return ret;
    }

    struct proxy_mt {
        vector<T> &R;

        proxy_mt(vector<T> &_R) : R(_R) {}

        T &operator[](int c) { return R[c]; }
    };

    proxy_mt operator[](int rr)
    {
        return proxy_mt(arr[rr]);
    }

    constexpr matrix &operator+=(const matrix &va)
    {
        assert(row == va.row && col == va.col);
        for (int i = 0; i < row; i++)
            for (int p = 0; p < col; p++)
                arr[i][p] += va.arr[i][p];
        return *this;
    }
    constexpr matrix &operator-=(const matrix &va)
    {
        assert(row == va.row && col == va.col);
        for (int i = 0; i < row; i++)
            for (int p = 0; p < col; p++)
                arr[i][p] -= va.arr[i][p];
        return *this;
    }
    constexpr matrix &operator*=(const matrix &va)
    {
        assert(col == va.row);
        matrix tem(row, va.col);
        for (int i = 0; i < col; i++)
            for (int p = 0; p < row; p++) {
                T tm = arr[p][i];
                for (int q = 0; q < va.col; q++)
                    tem.arr[p][q] += tm * va.arr[i][q];
            }
        arr.swap(tem.arr), col = tem.col, row = tem.row;
        return *this;
    }

    constexpr matrix operator+(const matrix &va) const { return matrix(*this) += va; }
    constexpr matrix operator-(const matrix &va) const { return matrix(*this) -= va; }
    constexpr matrix operator*(const matrix &va) const { return matrix(*this) *= va; }
    constexpr bool operator==(const matrix &va) const { return arr == va.arr; }
    constexpr bool operator!=(const matrix &va) const { return arr != va.arr; }

    friend ostream &operator<<(ostream &os, const matrix &va)
    {
        for (int i = 0; i < va.row; i++) {
            for (int p = 0; p < va.col; p++)
                os << va.arr[i][p] << " ";
            os << "\n";
        }
        return os;
    }

    int row, col;
    vector<vector<T>> arr;
};