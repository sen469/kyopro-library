#include <bits/stdc++.h>

#include "kyopro/doubling.hpp"

using namespace std;

long long add_ll(long long a, long long b) {
    return a + b;
}

long long zero_ll() {
    return 0;
}

string concat_string(string a, string b) {
    return a + b;
}

string empty_string() {
    return "";
}

int main() {
    {
        vector<int> next = {1, 2, 0};
        kyopro::doubling db(next, 100);

        assert(db.size() == 3);
        assert(db.jump(0, 0) == 0);
        assert(db.jump(0, 1) == 1);
        assert(db.jump(0, 2) == 2);
        assert(db.jump(0, 3) == 0);
        assert(db(1, 5) == 0);
        assert(db.next(0, 1) == 2);
    }

    {
        kyopro::doubling db(5, 20);
        db.set_next(0, 1);
        db.set_next(1, 2);
        db.set_next(2, 3);
        db.set_next(3, -1);
        db.set_next(4, 4);
        db.build();

        assert(db.jump(0, 3) == 3);
        assert(db.jump(0, 4) == -1);
        assert(db.jump(3, 1) == -1);
        assert(db.jump(4, 20) == 4);
    }

    {
        vector<int> next = {1, 2, 3, -1};
        vector<long long> weight = {5, 7, 11, 100};
        kyopro::doubling_monoid<long long, add_ll, zero_ll> db(next, weight, 20);

        assert(db.jump(0, 0) == make_pair(0, 0LL));
        assert(db.jump(0, 1) == make_pair(1, 5LL));
        assert(db.jump(0, 2) == make_pair(2, 12LL));
        assert(db.jump(0, 3) == make_pair(3, 23LL));
        assert(db.jump(0, 4) == make_pair(-1, 23LL));
        assert(db.value(0, 1) == 12);
    }

    {
        vector<int> next = {1, 2, 0};
        vector<string> value = {"a", "b", "c"};
        kyopro::doubling_monoid<string, concat_string, empty_string> db(next, value, 10);

        assert(db.jump(0, 3) == make_pair(0, string("abc")));
        assert(db.jump(1, 4) == make_pair(2, string("bcab")));
    }

    return 0;
}
