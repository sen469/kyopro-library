// Verify registration works even if the core/container printers are included first.
#include "debug/debug.hpp"
#include "debug/interval_heap.hpp"
#include "debug/struct.hpp"

#include <cassert>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

namespace sample {

struct node {
    long long sum;
    int length;
    DG_FIELDS(sum, length)
};

struct nested {
    node root;
    std::vector<node> children;
    DG_FIELDS(root, children)
};

template <class T>
struct box {
    T value;
    DG_FIELDS(value)
};

struct empty {
    DG_FIELDS()
};

struct selected {
    int first, hidden, last;
    DG_FIELDS(last, first)
};

struct with_val {
    int value;
    int val() const { return value; }
    DG_FIELDS(value)
};

struct val_only {
    int value;
    int val() const { return value; }
};

struct custom_output {
    int value;
    DG_FIELDS(value)
    friend std::ostream& operator<<(std::ostream& os, const custom_output&) {
        return os << "custom";
    }
};

class private_fields {
    int value_;
public:
    explicit private_fields(int value) : value_(value) {}
    DG_FIELDS(value_)
};

struct noncopyable {
    int value;
    explicit noncopyable(int v) : value(v) {}
    noncopyable(const noncopyable&) = delete;
    friend std::ostream& operator<<(std::ostream& os, const noncopyable& x) {
        return os << x.value;
    }
};

struct no_member_copy {
    noncopyable value;
    DG_FIELDS(value)
};

}  // namespace sample

template <class T>
std::string render(const T& value) {
    std::ostringstream out;
    out << value;
    return out.str();
}

int main() {
    static_assert(std::is_aggregate<sample::node>::value, "registration preserves aggregate initialization");
    const sample::node node{10, 3};
    assert(render(node) == "{sum: 10, length: 3}");
    assert(render(sample::empty{}) == "{}");
    assert(render(sample::selected{1, 2, 3}) == "{last: 3, first: 1}");
    assert(render(sample::private_fields(7)) == "{value_: 7}");
    assert(render(sample::box<std::vector<int>>{{1, 2}}) == "{value: {1, 2}}");
    assert(render(sample::with_val{5}) == "{value: 5}");
    assert(render(sample::val_only{5}) == "5");
    assert(render(sample::custom_output{5}) == "custom");
    sample::no_member_copy noncopyable{sample::noncopyable(42)};
    assert(render(noncopyable) == "{value: 42}");

    std::vector<sample::node> nodes = {{10, 3}, {2, 1}};
    const std::string expected = "{{sum: 10, length: 3}, {sum: 2, length: 1}}";
    assert(render(nodes) == expected);
    assert(render(sample::nested{node, nodes}) ==
           "{root: {sum: 10, length: 3}, children: " + expected + "}");
    assert(render(std::make_pair(node, 7)) == "({sum: 10, length: 3}, 7)");
    assert(render(std::make_tuple(node, nodes)) == "({sum: 10, length: 3}, " + expected + ")");
    assert(render(std::map<int, sample::node>{{1, node}}) == "{1: {sum: 10, length: 3}}");
    assert(render(std::deque<sample::node>{node}) == "{{sum: 10, length: 3}}");

    __int128 large = __int128(1) << 100;
    assert(render(sample::box<__int128>{large}) == "{value: 1267650600228229401496703205376}");
    sample::box<kyopro::interval_heap<int>> heap{{3, 1, 1}};
    assert(render(heap) == "{value: {1, 1, 3}}");
    auto compare = [](const sample::node& a, const sample::node& b) { return a.sum < b.sum; };
    kyopro::interval_heap<sample::node, decltype(compare)> heap_of_structs(nodes, compare);
    assert(render(heap_of_structs) == "{{sum: 2, length: 1}, {sum: 10, length: 3}}");

    std::ostringstream out;
    auto* previous = std::cerr.rdbuf(out.rdbuf());
    dg(node, nodes, heap_of_structs);
    std::cerr.rdbuf(previous);
    assert(out.str().find("{sum: 10, length: 3}") != std::string::npos);
    assert(out.str().find(expected) != std::string::npos);
    assert(node.sum == 10 && node.length == 3);
    assert(heap_of_structs.size() == 2);
}
