#include <bits/stdc++.h>

#include "kyopro/implicit_treap.hpp"

using namespace std;

struct S {
    long long sum;
    int size;
};

S op(S a, S b) { return {a.sum + b.sum, a.size + b.size}; }
S e() { return {0, 0}; }

struct F {
    long long add;
};

S mapping(F f, S x) { return {x.sum + f.add * x.size, x.size}; }
F composition(F f, F g) { return {f.add + g.add}; }
F id() { return {0}; }

using treap = kyopro::implicit_treap<S, op, e, F, mapping, composition, id>;

string op_string(string a, string b) { return a + b; }
string e_string() { return ""; }
string mapping_string(int, string x) { return x; }
int composition_string(int, int) { return 0; }
int id_string() { return 0; }

using string_treap =
    kyopro::implicit_treap<string, op_string, e_string, int, mapping_string,
                           composition_string, id_string>;

long long sum_vector(const vector<long long>& v, int l, int r) {
    long long res = 0;
    for (int i = l; i < r; i++) res += v[i];
    return res;
}

void check(treap& t, const vector<long long>& v) {
    assert(t.size() == (int)v.size());
    assert(t.all_prod().sum == sum_vector(v, 0, (int)v.size()));
    vector<S> got = t.to_vector();
    assert(got.size() == v.size());
    for (int i = 0; i < (int)v.size(); i++) {
        assert(got[i].sum == v[i]);
        assert(got[i].size == 1);
        assert(t.get(i).sum == v[i]);
    }
}

int main() {
    treap t;
    vector<long long> v;
    mt19937 rng(0);

    for (int step = 0; step < 20000; step++) {
        int n = (int)v.size();
        int kind = uniform_int_distribution<int>(0, 7)(rng);
        if (n == 0) kind = 0;

        if (kind == 0) {
            int p = uniform_int_distribution<int>(0, n)(rng);
            long long x = uniform_int_distribution<int>(-100, 100)(rng);
            t.insert(p, {x, 1});
            v.insert(v.begin() + p, x);
        } else if (kind == 1) {
            int p = uniform_int_distribution<int>(0, n - 1)(rng);
            S erased = t.erase(p);
            assert(erased.sum == v[p]);
            v.erase(v.begin() + p);
        } else if (kind == 2) {
            int l = uniform_int_distribution<int>(0, n)(rng);
            int r = uniform_int_distribution<int>(l, n)(rng);
            reverse(v.begin() + l, v.begin() + r);
            t.reverse(l, r);
        } else if (kind == 3) {
            int l = uniform_int_distribution<int>(0, n)(rng);
            int r = uniform_int_distribution<int>(l, n)(rng);
            long long add = uniform_int_distribution<int>(-20, 20)(rng);
            for (int i = l; i < r; i++) v[i] += add;
            t.apply(l, r, {add});
        } else if (kind == 4) {
            int l = uniform_int_distribution<int>(0, n)(rng);
            int r = uniform_int_distribution<int>(l, n)(rng);
            assert(t.prod(l, r).sum == sum_vector(v, l, r));
        } else if (kind == 5) {
            int p = uniform_int_distribution<int>(0, n - 1)(rng);
            long long x = uniform_int_distribution<int>(-100, 100)(rng);
            t.set(p, {x, 1});
            v[p] = x;
        } else if (kind == 6) {
            int l = uniform_int_distribution<int>(0, n)(rng);
            int r = uniform_int_distribution<int>(l, n)(rng);
            t.erase(l, r);
            v.erase(v.begin() + l, v.begin() + r);
        } else {
            int p = uniform_int_distribution<int>(0, n - 1)(rng);
            assert(t[p].sum == v[p]);
        }

        check(t, v);
    }

    treap copied = t;
    check(copied, v);
    copied.push_front({123, 1});
    assert(copied.size() == t.size() + 1);

    string_treap st(vector<string>{"a", "b", "c", "d"});
    assert(st.prod(0, 4) == "abcd");
    st.reverse(1, 4);
    assert(st.prod(0, 4) == "adcb");
    st.reverse(0, 4);
    assert(st.prod(0, 4) == "bcda");

    return 0;
}
