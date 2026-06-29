#ifndef KYOPRO_DOUBLING_HPP
#define KYOPRO_DOUBLING_HPP

#include <cassert>
#include <utility>
#include <vector>

namespace kyopro {

class doubling {
private:
    int n_;
    int log_;
    unsigned long long max_step_;
    std::vector<std::vector<int>> next_;
    bool built_;

    void init(int n, unsigned long long max_step) {
        assert(0 <= n);
        n_ = n;
        max_step_ = max_step;
        log_ = 1;
        while (log_ < 63 && (max_step_ >> log_) != 0) log_++;
        next_.assign(log_, std::vector<int>(n_, -1));
        built_ = false;
    }

public:
    doubling() : n_(0), log_(1), max_step_(0), next_(1), built_(false) {}

    explicit doubling(int n, long long max_step = (1LL << 60)) {
        assert(0 <= max_step);
        init(n, (unsigned long long)max_step);
    }

    explicit doubling(const std::vector<int>& next, long long max_step = (1LL << 60)) {
        assert(0 <= max_step);
        init((int)next.size(), (unsigned long long)max_step);
        for (int v = 0; v < n_; v++) {
            set_next(v, next[v]);
        }
        build();
    }

    void set_next(int v, int to) {
        assert(0 <= v && v < n_);
        assert(to == -1 || (0 <= to && to < n_));
        next_[0][v] = to;
        built_ = false;
    }

    void build() {
        for (int k = 0; k + 1 < log_; k++) {
            for (int v = 0; v < n_; v++) {
                int to = next_[k][v];
                next_[k + 1][v] = (to == -1 ? -1 : next_[k][to]);
            }
        }
        built_ = true;
    }

    int jump(int v, long long step) const {
        assert(built_);
        assert(0 <= v && v < n_);
        assert(0 <= step);
        unsigned long long k = (unsigned long long)step;
        assert(k <= max_step_);

        for (int i = 0; i < log_ && v != -1; i++) {
            if ((k >> i) & 1ULL) v = next_[i][v];
        }
        return v;
    }

    int operator()(int v, long long step) const {
        return jump(v, step);
    }

    int next(int v, int power) const {
        assert(built_);
        assert(0 <= v && v < n_);
        assert(0 <= power && power < log_);
        return next_[power][v];
    }

    int size() const {
        return n_;
    }

    int log() const {
        return log_;
    }
};

template <class S, S (*op)(S, S), S (*e)()>
class doubling_monoid {
private:
    int n_;
    int log_;
    unsigned long long max_step_;
    std::vector<std::vector<int>> next_;
    std::vector<std::vector<S>> value_;
    bool built_;

    void init(int n, unsigned long long max_step) {
        assert(0 <= n);
        n_ = n;
        max_step_ = max_step;
        log_ = 1;
        while (log_ < 63 && (max_step_ >> log_) != 0) log_++;
        next_.assign(log_, std::vector<int>(n_, -1));
        value_.assign(log_, std::vector<S>(n_, e()));
        built_ = false;
    }

public:
    doubling_monoid() : n_(0), log_(1), max_step_(0), next_(1), value_(1), built_(false) {}

    explicit doubling_monoid(int n, long long max_step = (1LL << 60)) {
        assert(0 <= max_step);
        init(n, (unsigned long long)max_step);
    }

    doubling_monoid(const std::vector<int>& next, const std::vector<S>& value, long long max_step = (1LL << 60)) {
        assert(next.size() == value.size());
        assert(0 <= max_step);
        init((int)next.size(), (unsigned long long)max_step);
        for (int v = 0; v < n_; v++) {
            set_next(v, next[v], value[v]);
        }
        build();
    }

    void set_next(int v, int to, const S& value) {
        assert(0 <= v && v < n_);
        assert(to == -1 || (0 <= to && to < n_));
        next_[0][v] = to;
        value_[0][v] = (to == -1 ? e() : value);
        built_ = false;
    }

    void build() {
        for (int k = 0; k + 1 < log_; k++) {
            for (int v = 0; v < n_; v++) {
                int to = next_[k][v];
                if (to == -1) {
                    next_[k + 1][v] = -1;
                    value_[k + 1][v] = value_[k][v];
                } else {
                    next_[k + 1][v] = next_[k][to];
                    value_[k + 1][v] = op(value_[k][v], value_[k][to]);
                }
            }
        }
        built_ = true;
    }

    std::pair<int, S> jump(int v, long long step) const {
        assert(built_);
        assert(0 <= v && v < n_);
        assert(0 <= step);
        unsigned long long k = (unsigned long long)step;
        assert(k <= max_step_);

        S res = e();
        for (int i = 0; i < log_ && v != -1; i++) {
            if ((k >> i) & 1ULL) {
                res = op(res, value_[i][v]);
                v = next_[i][v];
            }
        }
        return {v, res};
    }

    std::pair<int, S> operator()(int v, long long step) const {
        return jump(v, step);
    }

    int next(int v, int power) const {
        assert(built_);
        assert(0 <= v && v < n_);
        assert(0 <= power && power < log_);
        return next_[power][v];
    }

    S value(int v, int power) const {
        assert(built_);
        assert(0 <= v && v < n_);
        assert(0 <= power && power < log_);
        return value_[power][v];
    }

    int size() const {
        return n_;
    }

    int log() const {
        return log_;
    }
};

}  // namespace kyopro

#endif  // KYOPRO_DOUBLING_HPP
