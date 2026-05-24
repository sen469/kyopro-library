#ifndef KYOPRO_MATRIX_HPP
#define KYOPRO_MATRIX_HPP

#include <cassert>
#include <vector>

namespace kyopro {

template <class T>
class matrix {
   public:
    matrix() = default;

    matrix(int h, int w, T value = T()) : data_(h, std::vector<T>(w, value)) {}

    explicit matrix(const std::vector<std::vector<T>>& data) : data_(data) {
        if (!data_.empty()) {
            int w = (int)data_[0].size();
            for (const auto& row : data_) {
                assert((int)row.size() == w);
            }
        }
    }

    int height() const {
        return (int)data_.size();
    }

    int width() const {
        return data_.empty() ? 0 : (int)data_[0].size();
    }

    bool empty() const {
        return data_.empty();
    }

    std::vector<T>& operator[](int i) {
        return data_[i];
    }

    const std::vector<T>& operator[](int i) const {
        return data_[i];
    }

    const std::vector<std::vector<T>>& data() const {
        return data_;
    }

    static matrix identity(int n) {
        matrix res(n, n);
        for (int i = 0; i < n; i++) {
            res[i][i] = T(1);
        }
        return res;
    }

    matrix& operator+=(const matrix& rhs) {
        assert(height() == rhs.height());
        assert(width() == rhs.width());

        for (int i = 0; i < height(); i++) {
            for (int j = 0; j < width(); j++) {
                data_[i][j] += rhs[i][j];
            }
        }
        return *this;
    }

    matrix& operator-=(const matrix& rhs) {
        assert(height() == rhs.height());
        assert(width() == rhs.width());

        for (int i = 0; i < height(); i++) {
            for (int j = 0; j < width(); j++) {
                data_[i][j] -= rhs[i][j];
            }
        }
        return *this;
    }

    matrix& operator*=(const matrix& rhs) {
        return *this = *this * rhs;
    }

    matrix pow(long long e) const {
        assert(0 <= e);
        assert(height() == width());

        matrix res = identity(height());
        matrix base = *this;
        while (e > 0) {
            if (e & 1) res *= base;
            base *= base;
            e >>= 1;
        }
        return res;
    }

   private:
    std::vector<std::vector<T>> data_;
};

template <class T>
matrix<T> operator+(matrix<T> lhs, const matrix<T>& rhs) {
    lhs += rhs;
    return lhs;
}

template <class T>
matrix<T> operator-(matrix<T> lhs, const matrix<T>& rhs) {
    lhs -= rhs;
    return lhs;
}

template <class T>
matrix<T> operator*(const matrix<T>& lhs, const matrix<T>& rhs) {
    assert(lhs.width() == rhs.height());

    matrix<T> res(lhs.height(), rhs.width());
    for (int i = 0; i < lhs.height(); i++) {
        for (int k = 0; k < lhs.width(); k++) {
            for (int j = 0; j < rhs.width(); j++) {
                res[i][j] += lhs[i][k] * rhs[k][j];
            }
        }
    }
    return res;
}

template <class T>
matrix<T> matrix_power(const matrix<T>& a, long long e) {
    return a.pow(e);
}

}  // namespace kyopro

#endif  // KYOPRO_MATRIX_HPP
