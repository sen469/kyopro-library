#ifndef KYOPRO_LIS_HPP
#define KYOPRO_LIS_HPP

#include <algorithm>
#include <cassert>
#include <functional>
#include <limits>
#include <vector>

namespace kyopro {

template <class T>
struct lis_result {
    std::vector<int> indices;
    std::vector<T> values;

    int length() const { return static_cast<int>(indices.size()); }
};

namespace internal {

template <class T, class Compare>
std::vector<int> lis_tail_indices(const std::vector<T>& a, bool strict,
                                  Compare compare, std::vector<int>* previous) {
    assert(a.size() <= static_cast<std::size_t>(std::numeric_limits<int>::max()));
    std::vector<int> tails;
    for (int i = 0; i < static_cast<int>(a.size()); i++) {
        int left = 0, right = static_cast<int>(tails.size());
        while (left < right) {
            int middle = left + (right - left) / 2;
            // lower_bound for strict, upper_bound for non-strict sequences.
            bool before = strict ? compare(a[tails[middle]], a[i])
                                 : !compare(a[i], a[tails[middle]]);
            if (before) left = middle + 1;
            else right = middle;
        }
        if (previous != nullptr && left > 0) (*previous)[i] = tails[left - 1];
        if (left == static_cast<int>(tails.size())) tails.push_back(i);
        else tails[left] = i;
    }
    return tails;
}

}  // namespace internal

template <class T, class Compare = std::less<T>>
int lis_length(const std::vector<T>& a, bool strict = true,
               Compare compare = Compare()) {
    return static_cast<int>(internal::lis_tail_indices(a, strict, compare, nullptr).size());
}

template <class T, class Compare = std::less<T>>
lis_result<T> lis(const std::vector<T>& a, bool strict = true,
                  Compare compare = Compare()) {
    std::vector<int> previous(a.size(), -1);
    auto tails = internal::lis_tail_indices(a, strict, compare, &previous);
    lis_result<T> result;
    if (tails.empty()) return result;

    for (int i = tails.back(); i != -1; i = previous[i]) {
        result.indices.push_back(i);
    }
    std::reverse(result.indices.begin(), result.indices.end());
    result.values.reserve(result.indices.size());
    for (int i : result.indices) result.values.push_back(a[i]);
    return result;
}

}  // namespace kyopro

#endif  // KYOPRO_LIS_HPP
