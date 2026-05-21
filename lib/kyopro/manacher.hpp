#ifndef KYOPRO_MANACHER_HPP
#define KYOPRO_MANACHER_HPP

#include <algorithm>
#include <cassert>
#include <vector>

namespace kyopro {

template <class Container>
std::vector<int> manacher(const Container& s) {
    int n = (int)s.size();
    std::vector<int> odd(n);
    int left = 0;
    int right = -1;

    for (int i = 0; i < n; i++) {
        int k = (i > right) ? 1 : std::min(odd[left + right - i], right - i + 1);
        while (0 <= i - k && i + k < n && s[i - k] == s[i + k]) k++;
        odd[i] = k;
        if (i + k - 1 > right) {
            left = i - k + 1;
            right = i + k - 1;
        }
    }

    return odd;
}

template <class Container>
std::vector<int> manacher_even(const Container& s) {
    int n = (int)s.size();
    std::vector<int> even(n);
    int left = 0;
    int right = -1;

    for (int i = 0; i < n; i++) {
        int k = (i > right) ? 0 : std::min(even[left + right - i + 1], right - i + 1);
        while (0 <= i - k - 1 && i + k < n && s[i - k - 1] == s[i + k]) k++;
        even[i] = k;
        if (i + k - 1 > right) {
            left = i - k;
            right = i + k - 1;
        }
    }

    return even;
}

struct palindrome_radii_result {
    std::vector<int> odd;
    std::vector<int> even;

    bool is_palindrome(int l, int r) const {
        assert(0 <= l && l <= r && r <= (int)odd.size());
        int len = r - l;
        if (len == 0) return true;
        if (len & 1) {
            int center = (l + r - 1) / 2;
            return odd[center] >= len / 2 + 1;
        } else {
            int center = (l + r) / 2;
            return even[center] >= len / 2;
        }
    }
};

template <class Container>
palindrome_radii_result palindrome_radii(const Container& s) {
    return {manacher(s), manacher_even(s)};
}

}  // namespace kyopro

#endif  // KYOPRO_MANACHER_HPP
