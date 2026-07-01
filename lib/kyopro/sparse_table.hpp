#ifndef KYOPRO_SPARSE_TABLE_HPP
#define KYOPRO_SPARSE_TABLE_HPP

#include <cassert>
#include <vector>

namespace kyopro {

template <class S, S (*op)(S, S), S (*e)()>
class sparse_table {
private:
    int n_;
    std::vector<int> log_;
    std::vector<std::vector<S>> data_;

public:
    sparse_table() : n_(0), log_(1, 0) {}

    explicit sparse_table(int n) : sparse_table(std::vector<S>(n, e())) {}

    explicit sparse_table(const std::vector<S>& v) { build(v); }

    void build(const std::vector<S>& v) {
        n_ = (int)v.size();
        log_.assign(n_ + 1, 0);
        for (int i = 2; i <= n_; i++) {
            log_[i] = log_[i >> 1] + 1;
        }

        data_.clear();
        if (n_ == 0) return;
        data_.push_back(v);
        for (int k = 1; (1 << k) <= n_; k++) {
            int len = 1 << k;
            int half = len >> 1;
            data_.push_back(std::vector<S>(n_ - len + 1));
            for (int i = 0; i + len <= n_; i++) {
                data_[k][i] = op(data_[k - 1][i], data_[k - 1][i + half]);
            }
        }
    }

    int size() const { return n_; }

    bool empty() const { return n_ == 0; }

    S get(int p) const {
        assert(0 <= p && p < n_);
        return data_[0][p];
    }

    S operator[](int p) const { return get(p); }

    S prod(int l, int r) const {
        assert(0 <= l && l <= r && r <= n_);
        if (l == r) return e();
        int k = log_[r - l];
        return op(data_[k][l], data_[k][r - (1 << k)]);
    }

    S all_prod() const { return prod(0, n_); }
};

}  // namespace kyopro

#endif  // KYOPRO_SPARSE_TABLE_HPP
