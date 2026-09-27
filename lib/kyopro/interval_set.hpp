#ifndef KYOPRO_INTERVAL_SET_HPP
#define KYOPRO_INTERVAL_SET_HPP

#include <algorithm>
#include <map>
#include <optional>
#include <type_traits>
#include <utility>
#include <vector>

namespace kyopro {

template <class T>
class interval_set {
    static_assert(std::is_integral<T>::value, "T must be an integral type");

private:
    std::map<T, T> intervals_;

public:
    interval_set() = default;

    void add(T left, T right) {
        if (left >= right) return;

        auto it = intervals_.lower_bound(left);
        if (it != intervals_.begin()) {
            auto previous = std::prev(it);
            if (previous->second >= left) {
                left = previous->first;
                right = std::max(right, previous->second);
                it = intervals_.erase(previous);
            }
        }

        while (it != intervals_.end() && it->first <= right) {
            right = std::max(right, it->second);
            it = intervals_.erase(it);
        }
        intervals_.emplace(left, right);
    }

    void erase(T left, T right) {
        if (left >= right) return;

        auto it = intervals_.lower_bound(left);
        if (it != intervals_.begin()) {
            auto previous = std::prev(it);
            if (previous->second > left) it = previous;
        }

        while (it != intervals_.end() && it->first < right) {
            T interval_left = it->first;
            T interval_right = it->second;
            if (interval_right <= left) {
                ++it;
                continue;
            }

            it = intervals_.erase(it);
            if (interval_left < left) intervals_.emplace(interval_left, left);
            if (right < interval_right) {
                intervals_.emplace(right, interval_right);
                break;
            }
        }
    }

    std::optional<std::pair<T, T>> find(T x) const {
        auto it = intervals_.upper_bound(x);
        if (it == intervals_.begin()) return std::nullopt;
        --it;
        if (x < it->second) return std::pair<T, T>{it->first, it->second};
        return std::nullopt;
    }

    bool contains(T x) const { return find(x).has_value(); }

    bool contains(T left, T right) const {
        if (left >= right) return true;
        auto interval = find(left);
        return interval.has_value() && right <= interval->second;
    }

    T mex(T x = T(0)) const {
        auto interval = find(x);
        return interval.has_value() ? interval->second : x;
    }

    int size() const { return (int)intervals_.size(); }

    int interval_count() const { return size(); }

    bool empty() const { return intervals_.empty(); }

    void clear() { intervals_.clear(); }

    std::vector<std::pair<T, T>> intervals() const {
        return std::vector<std::pair<T, T>>(intervals_.begin(), intervals_.end());
    }
};

}  // namespace kyopro

#endif  // KYOPRO_INTERVAL_SET_HPP
