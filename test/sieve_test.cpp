#include <bits/stdc++.h>

#include "kyopro/sieve.hpp"

using namespace std;

int main() {
    kyopro::sieve<> sv(100);

    assert(sv.max_n() == 100);

    vector<int> expected_primes = {
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
        31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
        73, 79, 83, 89, 97
    };
    assert(sv.primes() == expected_primes);

    for (int i = 0; i <= 100; i++) {
        bool expected = i >= 2;
        for (int d = 2; d * d <= i; d++) {
            if (i % d == 0) expected = false;
        }
        assert(sv.is_prime(i) == expected);
    }

    assert(sv.spf(2) == 2);
    assert(sv.spf(60) == 2);
    assert(sv.spf(97) == 97);

    assert(sv.factorize(1).empty());
    vector<pair<int, int>> factor_2 = {{2, 1}};
    vector<pair<int, int>> factor_60 = {{2, 2}, {3, 1}, {5, 1}};
    vector<pair<int, int>> factor_72 = {{2, 3}, {3, 2}};
    vector<pair<int, int>> factor_97 = {{97, 1}};
    assert(sv.factorize(2) == factor_2);
    assert(sv.factorize(60) == factor_60);
    assert(sv.factorize(72) == factor_72);
    assert(sv.factorize(97) == factor_97);

    vector<int> divisors_60 = {1, 2, 3, 4, 5, 6, 10, 12, 15, 20, 30, 60};
    assert(sv.divisors(60) == divisors_60);
    vector<int> divisors_1 = {1};
    assert(sv.divisors(1) == divisors_1);


    kyopro::sieve<long long> sv_ll(100);
    vector<pair<long long, int>> factor_ll = {{2LL, 2}, {3LL, 1}, {5LL, 1}};
    vector<long long> divisors_ll = {1, 2, 3, 4, 5, 6, 10, 12, 15, 20, 30, 60};
    assert(sv_ll.factorize(60LL) == factor_ll);
    assert(sv_ll.divisors(60LL) == divisors_ll);
    assert(sv_ll.is_prime(97LL));

    vector<pair<long long, int>> factor_large = {{101LL, 1}, {103LL, 1}};
    assert(sv_ll.factorize(101LL * 103LL) == factor_large);

    kyopro::sieve<long long> sv_large(1000000);
    long long big = 999983LL * 999979LL;
    vector<pair<long long, int>> factor_big = {{999979LL, 1}, {999983LL, 1}};
    assert(sv_large.factorize(big) == factor_big);

    long long near_1e18 = 1000000007LL * 1000000009LL;
    vector<pair<long long, int>> factor_near_1e18 = {{1000000007LL, 1}, {1000000009LL, 1}};
    assert(sv_ll.factorize(near_1e18) == factor_near_1e18);

    long long pow2_60 = 1LL << 60;
    vector<pair<long long, int>> factor_pow2_60 = {{2LL, 60}};
    assert(sv_ll.factorize(pow2_60) == factor_pow2_60);

    long long ten18 = 1000000000000000000LL;
    vector<pair<long long, int>> factor_ten18 = {{2LL, 18}, {5LL, 18}};
    assert(sv_ll.factorize(ten18) == factor_ten18);
    assert(kyopro::factorize(ten18) == factor_ten18);

    unsigned long long ten19 = 10000000000000000000ULL;
    vector<pair<unsigned long long, int>> factor_ten19 = {{2ULL, 19}, {5ULL, 19}};
    assert(kyopro::factorize(ten19) == factor_ten19);

    assert(kyopro::prime_count_less(0) == 0);
    assert(kyopro::prime_count_less(1) == 0);
    assert(kyopro::prime_count_less(2) == 0);
    assert(kyopro::prime_count_less(3) == 1);
    assert(kyopro::prime_count_less(100) == 25);
    assert(kyopro::prime_count_less(101) == 25);
    assert(kyopro::prime_count_less(102) == 26);
    assert(kyopro::prime_count_less(1000000) == 78498);
    assert(kyopro::prime_count_less(100000000000LL) == 4118054813LL);

    assert(sv_ll.is_prime(1000000007LL));
    assert(!sv_ll.is_prime(near_1e18));

    sv.build(10);
    assert(sv.max_n() == 10);
    vector<int> primes_10 = {2, 3, 5, 7};
    vector<pair<int, int>> factor_10 = {{2, 1}, {5, 1}};
    assert(sv.primes() == primes_10);
    assert(sv.factorize(10) == factor_10);

    return 0;
}
