#ifndef KYOPRO_MATRIX_HPP
#define KYOPRO_MATRIX_HPP

#include <cassert>
#include <limits>
#include <optional>
#include <type_traits>
#include <utility>
#include <vector>

namespace kyopro {

template <class T>
struct linear_equation_result {
    bool exists;
    std::vector<T> solution;
    std::vector<std::vector<T>> basis;
};

template <class T>
class matrix {
   public:
    matrix() = default;

    matrix(int h, int w, T value = T()) : width_(w) {
        assert(0 <= h && 0 <= w);
        data_.assign(h, std::vector<T>(w, value));
    }

    explicit matrix(const std::vector<std::vector<T>>& data)
        : data_(data), width_(data.empty() ? 0 : (int)data[0].size()) {
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
        return width_;
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

    T determinant() const {
        assert(height() == width());
        matrix work = *this;
        T result = T(1);
        int r = work.eliminate(nullptr, &result);
        return r == height() ? result : T(0);
    }

    std::optional<matrix> inverse() const {
        assert(height() == width());
        matrix work = *this;
        matrix result = identity(height());
        if (work.eliminate(&result, nullptr) != height()) return std::nullopt;
        return result;
    }

    int rank() const {
        matrix work = *this;
        return work.eliminate(nullptr, nullptr);
    }

    linear_equation_result<T> solve(const std::vector<T>& b) const {
        assert((int)b.size() == height());
        matrix work = *this;
        matrix rhs(height(), 1);
        for (int i = 0; i < height(); i++) rhs[i][0] = b[i];
        std::vector<int> pivots;
        int r = work.eliminate(&rhs, nullptr, &pivots);
        for (int i = r; i < height(); i++) {
            if (!(rhs[i][0] == T(0))) return {false, {}, {}};
        }

        linear_equation_result<T> result{true, std::vector<T>(width(), T(0)), {}};
        std::vector<bool> is_pivot(width(), false);
        for (int i = 0; i < r; i++) {
            is_pivot[pivots[i]] = true;
            result.solution[pivots[i]] = rhs[i][0];
        }
        result.basis.reserve(width() - r);
        // Set one free variable to 1 and the others to 0 for each basis vector.
        for (int col = 0; col < width(); col++) {
            if (is_pivot[col]) continue;
            std::vector<T> v(width(), T(0));
            v[col] = T(1);
            for (int i = 0; i < r; i++) v[pivots[i]] = T(0) - work[i][col];
            result.basis.push_back(std::move(v));
        }
        return result;
    }

   private:
    int eliminate(matrix* companion, T* determinant, std::vector<int>* pivots = nullptr) {
        static_assert(!std::numeric_limits<T>::is_integer && !std::is_floating_point<T>::value,
                      "matrix elimination requires an exact field, such as modint with a prime modulus");
        int r = 0;
        for (int col = 0; col < width() && r < height(); col++) {
            int pivot = r;
            while (pivot < height() && data_[pivot][col] == T(0)) pivot++;
            if (pivot == height()) continue;
            if (pivot != r) {
                std::swap(data_[pivot], data_[r]);
                if (companion) std::swap((*companion)[pivot], (*companion)[r]);
                if (determinant) *determinant = T(0) - *determinant;
            }
            if (determinant) *determinant *= data_[r][col];
            T inv = T(1) / data_[r][col];
            for (int j = col + 1; j < width(); j++) data_[r][j] *= inv;
            data_[r][col] = T(1);
            if (companion) {
                for (int j = 0; j < companion->width(); j++) (*companion)[r][j] *= inv;
            }

            // Apply the same row operations to the companion (identity or right-hand side).
            for (int i = companion ? 0 : r + 1; i < height(); i++) {
                if (i == r || data_[i][col] == T(0)) continue;
                T factor = data_[i][col];
                for (int j = col + 1; j < width(); j++) data_[i][j] -= factor * data_[r][j];
                data_[i][col] = T(0);
                if (companion) {
                    for (int j = 0; j < companion->width(); j++) {
                        (*companion)[i][j] -= factor * (*companion)[r][j];
                    }
                }
            }
            if (pivots) pivots->push_back(col);
            r++;
        }
        return r;
    }

    std::vector<std::vector<T>> data_;
    int width_ = 0;
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
