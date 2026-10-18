#include <algorithm>
#include <cassert>
#include <random>
#include <string>
#include <vector>

#include "atcoder/segtree.hpp"
#include "debug/debug.hpp"
#include "kyopro/segtree.hpp"

long long sum_op(long long a, long long b) { return a + b; }
long long sum_e() { return 0; }
bool at_most_seven(long long x) { return x <= 7; }
int min_op(int a, int b) { return std::min(a, b); }
int min_e() { return std::numeric_limits<int>::max(); }
std::string concat(std::string a, std::string b) { return a + b; }
std::string empty_string() { return ""; }

using sum_tree = kyopro::segtree<long long, sum_op, sum_e>;
using acl_sum_tree = atcoder::segtree<long long, sum_op, sum_e>;

int main() {
    {
        const sum_tree empty;
        assert(empty.size() == 0);
        assert(empty.to_vec().empty());
        assert(empty.prod(0, 0) == 0);
        assert(empty.all_prod() == 0);
        assert(empty.max_right<at_most_seven>(0) == 0);
        assert(empty.min_left<at_most_seven>(0) == 0);
        sum_tree initialized(5);
        assert(initialized.size() == 5);
        assert(initialized.to_vec() == std::vector<long long>(5, 0));
    }

    std::mt19937 rng(469);
    for (int n : {0, 1, 2, 3, 7, 8, 9, 31, 32, 33, 100}) {
        std::vector<long long> a(n);
        for (auto& x : a) x = rng() % 10;
        sum_tree tree(a);
        acl_sum_tree acl(a);
        for (int step = 0; step < 120; step++) {
            if (n > 0) {
                int p = rng() % n;
                a[p] = rng() % 10;
                tree.set(p, a[p]);
                acl.set(p, a[p]);
            }
            const auto& view = tree;
            assert(view.to_vec() == a);
            for (int i = 0; i < n; i++) assert(view.get(i) == a[i]);
            long long total = 0;
            for (auto x : a) total += x;
            assert(view.all_prod() == total);
            assert(view.all_prod() == acl.all_prod());

            int l = rng() % (n + 1), r = rng() % (n + 1);
            if (r < l) std::swap(l, r);
            long long expected = 0;
            for (int i = l; i < r; i++) expected += a[i];
            assert(view.prod(l, r) == expected);
            assert(view.prod(l, r) == acl.prod(l, r));
            assert(view.prod(l, l) == 0);

            for (long long limit : {0LL, 7LL, total, total + 1}) {
                auto predicate = [=](long long value) { return value <= limit; };
                int right = l;
                long long aggregate = 0;
                while (right < n && aggregate + a[right] <= limit) aggregate += a[right++];
                assert(view.max_right(l, predicate) == right);
                assert(view.max_right(l, predicate) == acl.max_right(l, predicate));
                int left = r;
                aggregate = 0;
                while (left > 0 && a[left - 1] + aggregate <= limit) aggregate += a[--left];
                assert(view.min_left(r, predicate) == left);
                assert(view.min_left(r, predicate) == acl.min_left(r, predicate));
            }
            assert(view.max_right<at_most_seven>(l) == acl.max_right<at_most_seven>(l));
            assert(view.min_left<at_most_seven>(r) == acl.min_left<at_most_seven>(r));
        }
        auto snapshot = tree.to_vec();
        if (n > 0) snapshot[0] = -1;
        assert(tree.to_vec() == a);
        auto copied = tree;
        if (n > 0) copied.set(0, -1);
        assert(tree.to_vec() == a);
    }

    {
        // Noncommutative operation: detect reversed products and search folds.
        std::vector<std::string> a = {"a", "b", "c", "a", "b", "c", "d"};
        kyopro::segtree<std::string, concat, empty_string> tree(a);
        atcoder::segtree<std::string, concat, empty_string> acl(a);
        for (int l = 0; l <= tree.size(); l++) {
            for (int r = l; r <= tree.size(); r++) {
                std::string expected;
                for (int i = l; i < r; i++) expected += a[i];
                assert(tree.prod(l, r) == expected);
            }
            for (const std::string target : {"", "abc", "bc", "abcabcd", "x"}) {
                auto prefix = [&](std::string s) { return target.compare(0, s.size(), s) == 0; };
                auto suffix = [&](std::string s) {
                    return s.size() <= target.size() &&
                           target.compare(target.size() - s.size(), s.size(), s) == 0;
                };
                assert(tree.max_right(l, prefix) == acl.max_right(l, prefix));
                assert(tree.min_left(l, suffix) == acl.min_left(l, suffix));
            }
        }
        tree.set(2, "XYZ");
        assert(tree.all_prod() == "abXYZabcd");
        assert(tree.to_vec()[2] == "XYZ");
    }

    {
        // Named function pointers fit ACL's C++17 auto template interface.
        struct add {
            long long operator()(long long a, long long b) const { return a + b; }
        };
        struct zero {
            long long operator()() const { return 0; }
        };
        kyopro::segtree<long long, &sum_op, &sum_e> tree(std::vector<long long>{1, 2});
        assert(tree.all_prod() == 3);
        // Named constexpr function pointers also fit auto template parameters.
        constexpr auto operation = sum_op;
        constexpr auto identity = sum_e;
        kyopro::segtree<long long, operation, identity> second(3);
        second.set(1, 4);
        assert(second.to_vec() == std::vector<long long>({0, 4, 0}));
#if __cplusplus >= 202002L
        kyopro::segtree<long long, add{}, zero{}> callable_tree(std::vector<long long>{3, 4});
        assert(callable_tree.all_prod() == 7);
#endif
    }

    {
        kyopro::segtree<int, min_op, min_e> initialized(5);
        assert(initialized.to_vec() == std::vector<int>(5, min_e()));
        assert(initialized.all_prod() == min_e());
        kyopro::segtree<int, min_op, min_e> tree(std::vector<int>{9, 4, 7, 2, 8});
        assert(tree.prod(1, 4) == 2);
        auto at_least_four = [](int x) { return x >= 4; };
        assert(tree.max_right(0, at_least_four) == 3);
        assert(tree.min_left(5, at_least_four) == 4);
        assert(tree.prod(2, 2) == min_e());
    }

    {
        sum_tree tree(std::vector<long long>{1, 2, 3});
        std::ostringstream out;
        auto* previous = std::cerr.rdbuf(out.rdbuf());
        dg(tree.to_vec());
        std::cerr.rdbuf(previous);
        assert(out.str().find("{1, 2, 3}") != std::string::npos);
    }
}
