#ifndef KYOPRO_DEBUG_STRUCT_HPP
#define KYOPRO_DEBUG_STRUCT_HPP

#include "debug/debug.hpp"

namespace debug_internal {

template <class Tuple>
std::ostream& print_fields(std::ostream& os, const char* field_names, const Tuple& fields) {
    using ::operator<<;
    auto names = split_names(field_names);
    std::size_t index = 0;
    os << "{";
    std::apply([&](const auto&... members) {
        auto print_one = [&](const auto& member) {
            if (index != 0) os << ", ";
            os << names[index++] << ": " << member;
        };
        (print_one(members), ...);
        (void)print_one;
    }, fields);
    return os << "}";
}

}  // namespace debug_internal

template <class T>
auto operator<<(std::ostream& os, const T& value)
    -> std::enable_if_t<debug_internal::has_registered_fields<T>::value, std::ostream&> {
    return debug_internal::print_fields(os, T::debug_field_names(), value.debug_fields());
}

// Place in the public section. List lvalue members in the desired print order.
#define DG_FIELDS(...) \
    auto debug_fields() const { return std::tie(__VA_ARGS__); } \
    static constexpr const char* debug_field_names() { return #__VA_ARGS__; }

#endif  // KYOPRO_DEBUG_STRUCT_HPP
