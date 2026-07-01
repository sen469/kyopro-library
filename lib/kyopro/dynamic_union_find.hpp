#ifndef KYOPRO_DYNAMIC_UNION_FIND_HPP
#define KYOPRO_DYNAMIC_UNION_FIND_HPP

#include <algorithm>
#include <cassert>
#include <functional>
#include <unordered_map>
#include <vector>

namespace kyopro {

template <class Key = long long, class Hash = std::hash<Key>,
          class Pred = std::equal_to<Key>>
class dynamic_union_find {
private:
    std::unordered_map<Key, int, Hash, Pred> id_;
    std::vector<Key> keys_;
    std::vector<int> parent_or_size_;
    int group_count_;

    int leader_index(int a) {
        assert(0 <= a && a < (int)keys_.size());
        if (parent_or_size_[a] < 0) return a;
        return parent_or_size_[a] = leader_index(parent_or_size_[a]);
    }

public:
    dynamic_union_find() : group_count_(0) {}

    explicit dynamic_union_find(int reserve_size) : dynamic_union_find() {
        reserve(reserve_size);
    }

    void reserve(int n) {
        assert(0 <= n);
        id_.reserve(n);
        keys_.reserve(n);
        parent_or_size_.reserve(n);
    }

    bool contains(const Key& x) const {
        return id_.find(x) != id_.end();
    }

    int add(const Key& x) {
        auto it = id_.find(x);
        if (it != id_.end()) return it->second;
        int idx = (int)keys_.size();
        id_.emplace(x, idx);
        keys_.push_back(x);
        parent_or_size_.push_back(-1);
        group_count_++;
        return idx;
    }

    int index(const Key& x) const {
        auto it = id_.find(x);
        assert(it != id_.end());
        return it->second;
    }

    const Key& key(int i) const {
        assert(0 <= i && i < (int)keys_.size());
        return keys_[i];
    }

    bool merge(const Key& a, const Key& b) {
        int x = leader_index(add(a));
        int y = leader_index(add(b));
        if (x == y) return false;
        if (-parent_or_size_[x] < -parent_or_size_[y]) {
            std::swap(x, y);
        }
        parent_or_size_[x] += parent_or_size_[y];
        parent_or_size_[y] = x;
        group_count_--;
        return true;
    }

    bool same(const Key& a, const Key& b) {
        return leader_index(add(a)) == leader_index(add(b));
    }

    Key leader(const Key& a) {
        return keys_[leader_index(add(a))];
    }

    int size() const {
        return (int)keys_.size();
    }

    int size(const Key& a) {
        return -parent_or_size_[leader_index(add(a))];
    }

    int group_count() const {
        return group_count_;
    }

    std::vector<std::vector<Key>> groups() {
        int n = (int)keys_.size();
        std::vector<int> leader_buf(n), group_size(n);
        for (int i = 0; i < n; i++) {
            leader_buf[i] = leader_index(i);
            group_size[leader_buf[i]]++;
        }
        std::vector<std::vector<Key>> result(n);
        for (int i = 0; i < n; i++) {
            result[i].reserve(group_size[i]);
        }
        for (int i = 0; i < n; i++) {
            result[leader_buf[i]].push_back(keys_[i]);
        }
        result.erase(std::remove_if(result.begin(), result.end(),
                                    [](const std::vector<Key>& v) {
                                        return v.empty();
                                    }),
                     result.end());
        return result;
    }
};

}  // namespace kyopro

#endif  // KYOPRO_DYNAMIC_UNION_FIND_HPP
