#include <algorithm>
#include <cassert>
#include <random>
#include <string>
#include <vector>

#include "atcoder/lazysegtree.hpp"
#include "debug/debug.hpp"
#include "kyopro/lazy_segtree.hpp"

struct S {
    long long sum;
    int length;
};
bool operator==(S a, S b) { return a.sum == b.sum && a.length == b.length; }
S op(S a, S b) { return {a.sum + b.sum, a.length + b.length}; }
S e() { return {0, 0}; }
struct F { long long multiply, add; };
int mapping_calls = 0;
S mapping(F f, S x) {
    mapping_calls++;
    return {f.multiply * x.sum + f.add * x.length, x.length};
}
F composition(F f, F g) {
    return {f.multiply * g.multiply, f.multiply * g.add + f.add};
}
F id() { return {1, 0}; }
bool small_sum(S x) { return x.sum <= 7; }

using tree_type = kyopro::lazy_segtree<S, op, e, F, mapping, composition, id>;
using acl_type = atcoder::lazy_segtree<S, op, e, F, mapping, composition, id>;

S fold(const std::vector<S>& a, int l, int r) {
    S result = e();
    for (int i = l; i < r; i++) result = op(result, a[i]);
    return result;
}

void affine_test() {
    const tree_type const_empty;
    assert(const_empty.size() == 0);
    assert(const_empty.all_prod() == e());
    tree_type empty;
    empty.apply(0, 0, {2, 3});
    assert(empty.to_vec().empty());
    assert(empty.prod(0, 0) == e());
    assert(empty.max_right<small_sum>(0) == 0);
    assert(empty.min_left<small_sum>(0) == 0);
    tree_type initialized(5);
    assert(initialized.to_vec() == std::vector<S>(5, e()));

    std::mt19937 rng(469);
    for (int n : {0, 1, 2, 3, 7, 8, 9, 31, 32, 33, 100}) {
        std::vector<S> a(n);
        for (auto& value : a) value = {static_cast<long long>(rng() % 10), 1};
        tree_type tree(a);
        acl_type acl(a);
        for (int step = 0; step < 500; step++) {
            int l = rng() % (n + 1), r = rng() % (n + 1);
            if (r < l) std::swap(l, r);
            int kind = rng() % 6;
            F action = {static_cast<long long>(rng() % 2), static_cast<long long>(rng() % 5)};
            if (kind == 0) {
                tree.apply(l, r, action);
                acl.apply(l, r, action);
                for (int i = l; i < r; i++) a[i] = mapping(action, a[i]);
            } else if (kind == 1 && n > 0) {
                int p = rng() % n;
                tree.apply(p, action);
                acl.apply(p, action);
                a[p] = mapping(action, a[p]);
            } else if (kind == 2 && n > 0) {
                int p = rng() % n;
                a[p] = {static_cast<long long>(rng() % 10), 1};
                tree.set(p, a[p]);
                acl.set(p, a[p]);
            } else if (kind == 3) {
                assert(tree.prod(l, r) == fold(a, l, r));
                assert(tree.prod(l, r) == acl.prod(l, r));
            } else if (kind == 4 && n > 0) {
                int p = rng() % n;
                assert(tree.get(p) == a[p]);
                assert(tree.get(p) == acl.get(p));
            } else if (kind == 5) {
                assert(tree.to_vec() == a);
                assert(tree.to_vec() == a);
            }
            const auto& view = tree;
            assert(view.all_prod() == fold(a, 0, n));
            assert(view.all_prod() == acl.all_prod());
            long long limit = rng() % (view.all_prod().sum + 1);
            auto predicate = [=](S value) { return value.sum <= limit; };
            int right = l;
            S aggregate = e();
            while (right < n && predicate(op(aggregate, a[right]))) {
                aggregate = op(aggregate, a[right++]);
            }
            assert(tree.max_right(l, predicate) == right);
            assert(tree.max_right(l, predicate) == acl.max_right(l, predicate));
            int left = r;
            aggregate = e();
            while (left > 0 && predicate(op(a[left - 1], aggregate))) {
                aggregate = op(a[--left], aggregate);
            }
            assert(tree.min_left(r, predicate) == left);
            assert(tree.min_left(r, predicate) == acl.min_left(r, predicate));
            assert(tree.max_right<small_sum>(l) == acl.max_right<small_sum>(l));
            assert(tree.min_left<small_sum>(r) == acl.min_left<small_sum>(r));
        }
        assert(tree.to_vec() == a);
    }

    // Noncommuting actions, pending tags at multiple depths, then set/flush/update.
    std::vector<S> a(9, {1, 1});
    tree_type tree(a);
    for (F action : {F{2, 3}, F{3, 1}}) {
        tree.apply(0, 9, action);
        for (auto& value : a) value = mapping(action, value);
    }
    tree.apply(2, 7, {0, 5});
    for (int i = 2; i < 7; i++) a[i] = {5, 1};
    auto pending_copy = tree;
    tree.set(4, {99, 1});
    a[4] = {99, 1};
    assert(pending_copy.get(4) == (S{5, 1}));
    assert(tree.to_vec() == a);
    auto snapshot = tree.to_vec();
    snapshot[0] = {-1, 1};
    assert(tree.get(0) == a[0]);
    tree.apply(0, 9, {1, 2});
    for (auto& value : a) value.sum += 2;
    assert(tree.prod(1, 8) == fold(a, 1, 8));
    assert(tree.to_vec() == a);

    // A linear flush visits each internal node once, not each root-to-leaf path.
    tree_type large(std::vector<S>(1024, {1, 1}));
    large.apply(0, 1024, {2, 3});
    mapping_calls = 0;
    auto values = large.to_vec();
    assert(mapping_calls == 2 * (1024 - 1));
    assert(values == std::vector<S>(1024, {5, 1}));
    assert(large.all_prod() == (S{5120, 1024}));
}

std::string concat(std::string a, std::string b) { return a + b; }
std::string empty_string() { return ""; }
std::string shift(int f, std::string s) {
    for (char& c : s) c = 'a' + (c - 'a' + f) % 26;
    return s;
}
int compose_shift(int f, int g) { return (f + g) % 26; }
int identity_shift() { return 0; }

void noncommutative_test() {
    using string_tree = kyopro::lazy_segtree<std::string, concat, empty_string,
                                            int, shift, compose_shift, identity_shift>;
    using acl_string_tree = atcoder::lazy_segtree<std::string, concat, empty_string,
                                                int, shift, compose_shift, identity_shift>;
    std::vector<std::string> a = {"a", "b", "c", "a", "b", "c", "d"};
    string_tree tree(a);
    acl_string_tree acl(a);
    tree.apply(0, 7, 1);
    acl.apply(0, 7, 1);
    tree.apply(2, 6, 2);
    acl.apply(2, 6, 2);
    for (int i = 0; i < 7; i++) a[i] = shift(i >= 2 && i < 6 ? 3 : 1, a[i]);
    for (int l = 0; l <= 7; l++) {
        for (int r = l; r <= 7; r++) {
            std::string expected;
            for (int i = l; i < r; i++) expected += a[i];
            assert(tree.prod(l, r) == expected);
            assert(tree.prod(l, r) == acl.prod(l, r));
            auto prefix = [&](std::string s) { return expected.compare(0, s.size(), s) == 0; };
            auto suffix = [&](std::string s) {
                return s.size() <= expected.size() &&
                       expected.compare(expected.size() - s.size(), s.size(), s) == 0;
            };
            assert(tree.max_right(l, prefix) == r);
            assert(tree.max_right(l, prefix) == acl.max_right(l, prefix));
            assert(tree.min_left(r, suffix) == l);
            assert(tree.min_left(r, suffix) == acl.min_left(r, suffix));
        }
    }
    assert(tree.to_vec() == a);
    tree.apply(3, 1);
    a[3] = shift(1, a[3]);
    assert(tree.to_vec() == a);

    std::ostringstream out;
    auto* previous = std::cerr.rdbuf(out.rdbuf());
    dg(tree.to_vec());
    std::cerr.rdbuf(previous);
    assert(out.str().find("{b, c, f, e, e, f, e}") != std::string::npos);
}

int main() {
    affine_test();
    noncommutative_test();
}
