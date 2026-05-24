#ifndef KYOPRO_WAVELET_MATRIX_HPP
#define KYOPRO_WAVELET_MATRIX_HPP

#include <algorithm>
#include <cassert>
#include <limits>
#include <vector>

namespace kyopro {

template <class T>
class wavelet_matrix {
   private:
    struct bit_vector {
        std::vector<int> sum;

        bit_vector() = default;

        explicit bit_vector(const std::vector<int>& bits) : sum(bits.size() + 1) {
            for (int i = 0; i < (int)bits.size(); i++) {
                sum[i + 1] = sum[i] + bits[i];
            }
        }

        int rank1(int r) const {
            return sum[r];
        }

        int rank0(int r) const {
            return r - sum[r];
        }
    };

   public:
    wavelet_matrix() = default;

    explicit wavelet_matrix(const std::vector<T>& a) {
        build(a);
    }

    void build(const std::vector<T>& a) {
        n = (int)a.size();
        values = a;
        std::sort(values.begin(), values.end());
        values.erase(std::unique(values.begin(), values.end()), values.end());

        int sigma = (int)values.size();
        lg = 0;
        while ((1 << lg) < sigma) lg++;

        matrix.clear();
        mid.clear();
        matrix.reserve(lg);
        mid.reserve(lg);

        std::vector<int> cur(n), next(n);
        for (int i = 0; i < n; i++) {
            cur[i] = (int)(std::lower_bound(values.begin(), values.end(), a[i]) - values.begin());
        }

        for (int level = lg - 1; level >= 0; level--) {
            std::vector<int> bits(n);
            int zero_count = 0;
            for (int x : cur) {
                if (((x >> level) & 1) == 0) zero_count++;
            }

            int zero_pos = 0;
            int one_pos = zero_count;
            for (int i = 0; i < n; i++) {
                bits[i] = (cur[i] >> level) & 1;
                if (bits[i] == 0) {
                    next[zero_pos++] = cur[i];
                } else {
                    next[one_pos++] = cur[i];
                }
            }

            matrix.push_back(bit_vector(bits));
            mid.push_back(zero_count);
            cur.swap(next);
        }
    }

    int size() const {
        return n;
    }

    int distinct_size() const {
        return (int)values.size();
    }

    bool empty() const {
        return n == 0;
    }

    const std::vector<T>& sorted_values() const {
        return values;
    }

    T access(int i) const {
        assert(0 <= i && i < n);

        int code = 0;
        for (int depth = 0; depth < lg; depth++) {
            int level = lg - 1 - depth;
            int bit = matrix[depth].rank1(i + 1) - matrix[depth].rank1(i);
            if (bit == 0) {
                i = matrix[depth].rank0(i);
            } else {
                code |= 1 << level;
                i = mid[depth] + matrix[depth].rank1(i);
            }
        }
        return values[code];
    }

    T operator[](int i) const {
        return access(i);
    }

    T kth_smallest(int l, int r, int k) const {
        assert(0 <= l && l <= r && r <= n);
        assert(0 <= k && k < r - l);

        int code = 0;
        for (int depth = 0; depth < lg; depth++) {
            int level = lg - 1 - depth;
            int one_l = matrix[depth].rank1(l);
            int one_r = matrix[depth].rank1(r);
            int zero_l = matrix[depth].rank0(l);
            int zero_r = matrix[depth].rank0(r);
            int zero_count = zero_r - zero_l;

            if (k < zero_count) {
                l = zero_l;
                r = zero_r;
            } else {
                code |= 1 << level;
                k -= zero_count;
                l = mid[depth] + one_l;
                r = mid[depth] + one_r;
            }
        }

        return values[code];
    }

    T kth_largest(int l, int r, int k) const {
        assert(0 <= l && l <= r && r <= n);
        assert(0 <= k && k < r - l);
        return kth_smallest(l, r, r - l - 1 - k);
    }

    int range_freq(int l, int r, const T& upper) const {
        assert(0 <= l && l <= r && r <= n);
        int code = (int)(std::lower_bound(values.begin(), values.end(), upper) - values.begin());
        return count_less(l, r, code);
    }

    int range_freq(int l, int r, const T& lower, const T& upper) const {
        assert(0 <= l && l <= r && r <= n);
        if (!(lower < upper)) return 0;
        return range_freq(l, r, upper) - range_freq(l, r, lower);
    }

    int count(int l, int r, const T& x) const {
        assert(0 <= l && l <= r && r <= n);
        int code = (int)(std::lower_bound(values.begin(), values.end(), x) - values.begin());
        if (code == (int)values.size() || values[code] != x) return 0;
        return count_less(l, r, code + 1) - count_less(l, r, code);
    }

    T prev_value(int l, int r, const T& upper) const {
        int cnt = range_freq(l, r, upper);
        assert(cnt > 0);
        return kth_smallest(l, r, cnt - 1);
    }

    T next_value(int l, int r, const T& lower) const {
        int cnt = range_freq(l, r, lower);
        assert(cnt < r - l);
        return kth_smallest(l, r, cnt);
    }

   private:
    int n = 0;
    int lg = 0;
    std::vector<T> values;
    std::vector<bit_vector> matrix;
    std::vector<int> mid;

    int count_less(int l, int r, int code) const {
        if (code <= 0) return 0;
        if (code >= (int)values.size()) return r - l;

        int res = 0;
        for (int depth = 0; depth < lg; depth++) {
            int level = lg - 1 - depth;
            int one_l = matrix[depth].rank1(l);
            int one_r = matrix[depth].rank1(r);
            int zero_l = matrix[depth].rank0(l);
            int zero_r = matrix[depth].rank0(r);

            if ((code >> level) & 1) {
                res += zero_r - zero_l;
                l = mid[depth] + one_l;
                r = mid[depth] + one_r;
            } else {
                l = zero_l;
                r = zero_r;
            }
        }
        return res;
    }
};

}  // namespace kyopro

#endif  // KYOPRO_WAVELET_MATRIX_HPP
