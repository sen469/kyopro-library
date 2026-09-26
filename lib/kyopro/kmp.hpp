#ifndef KYOPRO_KMP_HPP
#define KYOPRO_KMP_HPP

#include <vector>

namespace kyopro {

template <class Container>
std::vector<int> prefix_function(const Container& pattern) {
    int n = (int)pattern.size();
    std::vector<int> prefix(n);
    for (int i = 1; i < n; i++) {
        int j = prefix[i - 1];
        while (j > 0 && !(pattern[i] == pattern[j])) {
            j = prefix[j - 1];
        }
        if (pattern[i] == pattern[j]) j++;
        prefix[i] = j;
    }
    return prefix;
}

template <class Text, class Pattern>
std::vector<int> kmp_search(const Text& text, const Pattern& pattern) {
    int n = (int)text.size();
    int m = (int)pattern.size();
    std::vector<int> result;

    if (m == 0) {
        result.reserve(n + 1);
        for (int i = 0; i <= n; i++) result.push_back(i);
        return result;
    }

    std::vector<int> prefix = prefix_function(pattern);
    int j = 0;
    for (int i = 0; i < n; i++) {
        while (j > 0 && !(text[i] == pattern[j])) {
            j = prefix[j - 1];
        }
        if (text[i] == pattern[j]) j++;
        if (j == m) {
            result.push_back(i - m + 1);
            j = prefix[j - 1];
        }
    }
    return result;
}

template <class Text, class Pattern>
int kmp_find(const Text& text, const Pattern& pattern) {
    int n = (int)text.size();
    int m = (int)pattern.size();
    if (m == 0) return 0;

    std::vector<int> prefix = prefix_function(pattern);
    int j = 0;
    for (int i = 0; i < n; i++) {
        while (j > 0 && !(text[i] == pattern[j])) {
            j = prefix[j - 1];
        }
        if (text[i] == pattern[j]) j++;
        if (j == m) return i - m + 1;
    }
    return -1;
}

}  // namespace kyopro

#endif  // KYOPRO_KMP_HPP
