#ifndef KYOPRO_CUMULATIVE_SUM_HPP
#define KYOPRO_CUMULATIVE_SUM_HPP

#include <cassert>
#include <vector>

namespace kyopro {

template <class T>
class cumulative_sum {
private:
    std::vector<T> data_;

public:
    cumulative_sum() : data_(1, T(0)) {}

    explicit cumulative_sum(int n) : data_(n + 1, T(0)) {
        assert(0 <= n);
    }

    explicit cumulative_sum(const std::vector<T>& v) { build(v); }

    void build(const std::vector<T>& v) {
        int n = (int)v.size();
        data_.assign(n + 1, T(0));
        for (int i = 0; i < n; i++) {
            data_[i + 1] = data_[i] + v[i];
        }
    }

    int size() const { return (int)data_.size() - 1; }

    bool empty() const { return size() == 0; }

    T prefix_sum(int r) const {
        assert(0 <= r && r <= size());
        return data_[r];
    }

    T sum(int l, int r) const {
        assert(0 <= l && l <= r && r <= size());
        return data_[r] - data_[l];
    }

    T all_sum() const { return data_.back(); }
};

template <class T>
class cumulative_sum_2d {
private:
    int h_;
    int w_;
    std::vector<std::vector<T>> data_;

public:
    cumulative_sum_2d() : h_(0), w_(0), data_(1, std::vector<T>(1, T(0))) {}

    cumulative_sum_2d(int h, int w)
        : h_(h), w_(w), data_(h + 1, std::vector<T>(w + 1, T(0))) {
        assert(0 <= h && 0 <= w);
    }

    explicit cumulative_sum_2d(const std::vector<std::vector<T>>& v) {
        build(v);
    }

    void build(const std::vector<std::vector<T>>& v) {
        h_ = (int)v.size();
        w_ = h_ == 0 ? 0 : (int)v[0].size();
        for (int y = 0; y < h_; y++) {
            assert((int)v[y].size() == w_);
        }
        data_.assign(h_ + 1, std::vector<T>(w_ + 1, T(0)));
        for (int y = 0; y < h_; y++) {
            for (int x = 0; x < w_; x++) {
                data_[y + 1][x + 1] = data_[y + 1][x] + data_[y][x + 1] -
                                      data_[y][x] + v[y][x];
            }
        }
    }

    int height() const { return h_; }

    int width() const { return w_; }

    bool empty() const { return h_ == 0 || w_ == 0; }

    T prefix_sum(int y, int x) const {
        assert(0 <= y && y <= h_);
        assert(0 <= x && x <= w_);
        return data_[y][x];
    }

    T sum(int y1, int x1, int y2, int x2) const {
        assert(0 <= y1 && y1 <= y2 && y2 <= h_);
        assert(0 <= x1 && x1 <= x2 && x2 <= w_);
        return data_[y2][x2] - data_[y1][x2] - data_[y2][x1] +
               data_[y1][x1];
    }

    T all_sum() const { return data_[h_][w_]; }
};

template <class T>
class cumulative_sum_3d {
private:
    int d_;
    int h_;
    int w_;
    std::vector<std::vector<std::vector<T>>> data_;

public:
    cumulative_sum_3d()
        : d_(0),
          h_(0),
          w_(0),
          data_(1, std::vector<std::vector<T>>(1, std::vector<T>(1, T(0)))) {}

    cumulative_sum_3d(int d, int h, int w)
        : d_(d),
          h_(h),
          w_(w),
          data_(d + 1, std::vector<std::vector<T>>(
                           h + 1, std::vector<T>(w + 1, T(0)))) {
        assert(0 <= d && 0 <= h && 0 <= w);
    }

    explicit cumulative_sum_3d(
        const std::vector<std::vector<std::vector<T>>>& v) {
        build(v);
    }

    void build(const std::vector<std::vector<std::vector<T>>>& v) {
        d_ = (int)v.size();
        h_ = d_ == 0 ? 0 : (int)v[0].size();
        w_ = h_ == 0 ? 0 : (int)v[0][0].size();
        for (int z = 0; z < d_; z++) {
            assert((int)v[z].size() == h_);
            for (int y = 0; y < h_; y++) {
                assert((int)v[z][y].size() == w_);
            }
        }
        data_.assign(d_ + 1, std::vector<std::vector<T>>(
                                 h_ + 1, std::vector<T>(w_ + 1, T(0))));
        for (int z = 0; z < d_; z++) {
            for (int y = 0; y < h_; y++) {
                for (int x = 0; x < w_; x++) {
                    data_[z + 1][y + 1][x + 1] =
                        data_[z][y + 1][x + 1] +
                        data_[z + 1][y][x + 1] +
                        data_[z + 1][y + 1][x] -
                        data_[z][y][x + 1] -
                        data_[z][y + 1][x] -
                        data_[z + 1][y][x] +
                        data_[z][y][x] + v[z][y][x];
                }
            }
        }
    }

    int depth() const { return d_; }

    int height() const { return h_; }

    int width() const { return w_; }

    bool empty() const { return d_ == 0 || h_ == 0 || w_ == 0; }

    T prefix_sum(int z, int y, int x) const {
        assert(0 <= z && z <= d_);
        assert(0 <= y && y <= h_);
        assert(0 <= x && x <= w_);
        return data_[z][y][x];
    }

    T sum(int z1, int y1, int x1, int z2, int y2, int x2) const {
        assert(0 <= z1 && z1 <= z2 && z2 <= d_);
        assert(0 <= y1 && y1 <= y2 && y2 <= h_);
        assert(0 <= x1 && x1 <= x2 && x2 <= w_);
        return data_[z2][y2][x2] - data_[z1][y2][x2] -
               data_[z2][y1][x2] - data_[z2][y2][x1] +
               data_[z1][y1][x2] + data_[z1][y2][x1] +
               data_[z2][y1][x1] - data_[z1][y1][x1];
    }

    T all_sum() const { return data_[d_][h_][w_]; }
};

}  // namespace kyopro

#endif  // KYOPRO_CUMULATIVE_SUM_HPP
