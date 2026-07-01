#ifndef KYOPRO_PERSISTENT_UNION_FIND_HPP
#define KYOPRO_PERSISTENT_UNION_FIND_HPP

#include <algorithm>
#include <cassert>
#include <memory>
#include <utility>
#include <vector>

namespace kyopro {

class persistent_union_find {
private:
    class persistent_array {
    private:
        struct node {
            int value;
            int left;
            int right;

            node(int value, int left, int right)
                : value(value), left(left), right(right) {}
        };

        int n_;
        int root_;
        int default_value_;
        std::shared_ptr<std::vector<node>> nodes_;

        persistent_array(int n, int root, int default_value,
                         const std::shared_ptr<std::vector<node>>& nodes)
            : n_(n), root_(root), default_value_(default_value), nodes_(nodes) {}

        int make_node(int value, int left = 0, int right = 0) const {
            nodes_->emplace_back(value, left, right);
            return (int)nodes_->size() - 1;
        }

        int get(int t, int l, int r, int p) const {
            if (t == 0) return default_value_;
            if (r - l == 1) return (*nodes_)[t].value;
            int m = (l + r) / 2;
            if (p < m) return get((*nodes_)[t].left, l, m, p);
            return get((*nodes_)[t].right, m, r, p);
        }

        int set(int t, int l, int r, int p, int value) const {
            if (r - l == 1) return make_node(value);
            int m = (l + r) / 2;
            int left = t == 0 ? 0 : (*nodes_)[t].left;
            int right = t == 0 ? 0 : (*nodes_)[t].right;
            if (p < m) {
                left = set(left, l, m, p, value);
            } else {
                right = set(right, m, r, p, value);
            }
            return make_node(default_value_, left, right);
        }

    public:
        persistent_array()
            : n_(0),
              root_(0),
              default_value_(0),
              nodes_(std::make_shared<std::vector<node>>()) {
            nodes_->emplace_back(default_value_, 0, 0);
        }

        persistent_array(int n, int default_value)
            : n_(n),
              root_(0),
              default_value_(default_value),
              nodes_(std::make_shared<std::vector<node>>()) {
            assert(0 <= n);
            nodes_->emplace_back(default_value_, 0, 0);
        }

        int size() const { return n_; }

        int node_count() const { return (int)nodes_->size() - 1; }

        int get(int p) const {
            assert(0 <= p && p < n_);
            return get(root_, 0, n_, p);
        }

        persistent_array set(int p, int value) const {
            assert(0 <= p && p < n_);
            int new_root = set(root_, 0, n_, p, value);
            return persistent_array(n_, new_root, default_value_, nodes_);
        }
    };

    int n_;
    int group_count_;
    persistent_array parent_or_size_;

    persistent_union_find(int n, int group_count,
                          const persistent_array& parent_or_size)
        : n_(n),
          group_count_(group_count),
          parent_or_size_(parent_or_size) {}

public:
    persistent_union_find()
        : n_(0), group_count_(0), parent_or_size_(0, -1) {}

    explicit persistent_union_find(int n)
        : n_(n), group_count_(n), parent_or_size_(n, -1) {
        assert(0 <= n);
    }

    int size() const { return n_; }

    bool empty() const { return n_ == 0; }

    int group_count() const { return group_count_; }

    int node_count() const { return parent_or_size_.node_count(); }

    int leader(int a) const {
        assert(0 <= a && a < n_);
        int p = parent_or_size_.get(a);
        if (p < 0) return a;
        return leader(p);
    }

    bool same(int a, int b) const {
        assert(0 <= a && a < n_);
        assert(0 <= b && b < n_);
        return leader(a) == leader(b);
    }

    int component_size(int a) const {
        assert(0 <= a && a < n_);
        return -parent_or_size_.get(leader(a));
    }

    int size(int a) const {
        return component_size(a);
    }

    std::pair<persistent_union_find, bool> merge(int a, int b) const {
        assert(0 <= a && a < n_);
        assert(0 <= b && b < n_);
        int x = leader(a);
        int y = leader(b);
        if (x == y) return {*this, false};

        int sx = -parent_or_size_.get(x);
        int sy = -parent_or_size_.get(y);
        if (sx < sy) {
            std::swap(x, y);
            std::swap(sx, sy);
        }

        persistent_array next_parent =
            parent_or_size_.set(x, -(sx + sy)).set(y, x);
        return {persistent_union_find(n_, group_count_ - 1, next_parent), true};
    }

    persistent_union_find unite(int a, int b) const {
        return merge(a, b).first;
    }

    std::vector<std::vector<int>> groups() const {
        std::vector<int> leader_buf(n_), group_size(n_);
        for (int i = 0; i < n_; i++) {
            leader_buf[i] = leader(i);
            group_size[leader_buf[i]]++;
        }
        std::vector<std::vector<int>> result(n_);
        for (int i = 0; i < n_; i++) {
            result[i].reserve(group_size[i]);
        }
        for (int i = 0; i < n_; i++) {
            result[leader_buf[i]].push_back(i);
        }
        result.erase(std::remove_if(result.begin(), result.end(),
                                    [](const std::vector<int>& v) {
                                        return v.empty();
                                    }),
                     result.end());
        return result;
    }
};

}  // namespace kyopro

#endif  // KYOPRO_PERSISTENT_UNION_FIND_HPP
