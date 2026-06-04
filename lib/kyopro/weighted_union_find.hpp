#ifndef KYOPRO_WEIGHTED_UNION_FIND_HPP
#define KYOPRO_WEIGHTED_UNION_FIND_HPP

#include <algorithm>
#include <cassert>
#include <vector>

namespace kyopro {

template <class T>
struct weighted_union_find {
  public:
    weighted_union_find() : _n(0) {}
    explicit weighted_union_find(int n) : _n(n), parent_or_size(n, -1), diff_weight(n, T(0)) {}

    bool merge(int a, int b, T w) {
        assert(0 <= a && a < _n);
        assert(0 <= b && b < _n);
        int x = leader(a), y = leader(b);
        T wa = potential(a), wb = potential(b);
        if (x == y) return wb - wa == w;

        T d = w + wa - wb;
        if (-parent_or_size[x] < -parent_or_size[y]) {
            std::swap(x, y);
            d = -d;
        }
        parent_or_size[x] += parent_or_size[y];
        parent_or_size[y] = x;
        diff_weight[y] = d;
        return true;
    }

    bool same(int a, int b) {
        assert(0 <= a && a < _n);
        assert(0 <= b && b < _n);
        return leader(a) == leader(b);
    }

    int leader(int a) {
        assert(0 <= a && a < _n);
        if (parent_or_size[a] < 0) return a;
        int p = parent_or_size[a];
        int r = leader(p);
        diff_weight[a] += diff_weight[p];
        return parent_or_size[a] = r;
    }

    int size(int a) {
        assert(0 <= a && a < _n);
        return -parent_or_size[leader(a)];
    }

    T potential(int a) {
        assert(0 <= a && a < _n);
        leader(a);
        return diff_weight[a];
    }

    T diff(int a, int b) {
        assert(0 <= a && a < _n);
        assert(0 <= b && b < _n);
        assert(same(a, b));
        return potential(b) - potential(a);
    }

    bool consistent(int a, int b, T w) {
        assert(0 <= a && a < _n);
        assert(0 <= b && b < _n);
        return !same(a, b) || diff(a, b) == w;
    }

    std::vector<std::vector<int>> groups() {
        std::vector<int> leader_buf(_n), group_size(_n);
        for (int i = 0; i < _n; i++) {
            leader_buf[i] = leader(i);
            group_size[leader_buf[i]]++;
        }
        std::vector<std::vector<int>> result(_n);
        for (int i = 0; i < _n; i++) {
            result[i].reserve(group_size[i]);
        }
        for (int i = 0; i < _n; i++) {
            result[leader_buf[i]].push_back(i);
        }
        result.erase(std::remove_if(result.begin(), result.end(), [](const std::vector<int>& v) { return v.empty(); }),
                     result.end());
        return result;
    }

  private:
    int _n;
    std::vector<int> parent_or_size;
    std::vector<T> diff_weight;
};

}  // namespace kyopro

#endif  // KYOPRO_WEIGHTED_UNION_FIND_HPP
