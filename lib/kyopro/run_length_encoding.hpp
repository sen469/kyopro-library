#ifndef KYOPRO_RUN_LENGTH_ENCODING_HPP
#define KYOPRO_RUN_LENGTH_ENCODING_HPP

#include <iterator>
#include <utility>
#include <vector>

namespace kyopro {

template <class InputIt>
auto run_length_encoding(InputIt first, InputIt last)
    -> std::vector<std::pair<typename std::iterator_traits<InputIt>::value_type, int>> {
    using value_type = typename std::iterator_traits<InputIt>::value_type;

    std::vector<std::pair<value_type, int>> res;
    if (first == last) return res;

    value_type current = *first;
    int count = 1;
    ++first;

    for (; first != last; ++first) {
        if (*first == current) {
            count++;
        } else {
            res.emplace_back(current, count);
            current = *first;
            count = 1;
        }
    }

    res.emplace_back(current, count);
    return res;
}

template <class Container>
auto run_length_encoding(const Container& c)
    -> decltype(run_length_encoding(std::begin(c), std::end(c))) {
    return run_length_encoding(std::begin(c), std::end(c));
}

}  // namespace kyopro

#endif  // KYOPRO_RUN_LENGTH_ENCODING_HPP
