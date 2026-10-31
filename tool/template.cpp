// #include <atcoder/all>
// using namespace atcoder;
// #include "all"
// using namespace kyopro;
// 998244353 1000000007 2147483647 4294967295 9223372036854775807 18446744073709551615
#ifdef SEN469
#include "debug/all"
#else
#define dg(...)
#endif
#include <bits/stdc++.h>
using namespace std;
template<class... T>
void input(T&... vars) { (cin >> ... >> vars); }
#define INT(...) int __VA_ARGS__; input(__VA_ARGS__)
#define LL(...) long long __VA_ARGS__; input(__VA_ARGS__)
#define CHR(...) char __VA_ARGS__; input(__VA_ARGS__)
#define STR(...) string __VA_ARGS__; input(__VA_ARGS__)
#define rep1(n) for (int __ = 0; __ < (int)(n); ++__)
#define rep2(i, n) for (int i = 0; i < (int)(n); ++i)
#define rep3(i, a, b) for (int i = (int)(a); i < (int)(b); ++i)
#define rep(...) GET_MACRO(__VA_ARGS__, rep3, rep2, rep1)(__VA_ARGS__)
#define rrep1(n) for (int __ = (int)(n) - 1; __ >= 0; --__)
#define rrep2(i, n) for (int i = (int)(n) - 1; i >= 0; --i)
#define rrep3(i, a, b) for (int i = (int)(b) - 1; i >= (int)(a); --i)
#define rrep(...) GET_MACRO(__VA_ARGS__, rrep3, rrep2, rrep1)(__VA_ARGS__)
#define GET_MACRO(_1, _2, _3, NAME, ...) NAME
#define all(x) x.begin(), x.end()
#define rall(x) x.rbegin(), x.rend()
#define Min(x) *min_element(all(x))
#define Max(x) *max_element(all(x))
#define vec vector
#define INF 1001001001
#define INFF 1001001001001001001LL
#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
using ll = long long;
using ull = unsigned long long;
using ld = long double;
using i128 = __int128_t;
template<typename T> using vv = vec<vec<T>>;
template<typename T> using vvv = vec<vec<vec<T>>>;
template<typename T> bool chmin(T& a, const T& b){ return a > b ? a = b, 1 : 0; }
template<typename T> bool chmax(T& a, const T& b){ return a < b ? a = b, 1 : 0; }
template<typename T, typename U> bool chmin(T& a, const U& b){ return a > b ? a = b, 1 : 0; }
template<typename T, typename U> bool chmax(T& a, const U& b){ return a < b ? a = b, 1 : 0; }
template<class T> void inc1(T& x) { if constexpr (is_arithmetic_v<T>) ++x; else for (auto& y : x) inc1(y); }
template<class T> void dec1(T& x) { if constexpr (is_arithmetic_v<T>) --x; else for (auto& y : x) dec1(y); }
template<class... T> void inc(T&... vars) { (inc1(vars), ...); }
template<class... T> void dec(T&... vars) { (dec1(vars), ...); }
void YN(bool b) { cout << (b ? "YES" : "NO") << endl; }
void Yn(bool b) { cout << (b ? "Yes" : "No") << endl; }
void yn(bool b) { cout << (b ? "yes" : "no") << endl; }
template <typename T>
auto make_vec(size_t n) { return (std::vector<T>(n)); }
template <typename T, typename... Args>
auto make_vec(size_t n, Args... args) { return (std::vector<decltype(make_vec<T>(args...))>(n, make_vec<T>(args...))); }
template<typename T>
istream &operator>>(istream &is, vec<T> &v) {
	for (int i = 0; i < (int)v.size(); i++)
		is >> v[i];
	return (is);
}
template<typename T>
ostream &operator<<(ostream &os, vec<T> &v) {
	for (int i = 0; i < (int)v.size(); i++)
		os << v[i] << (i == (int)v.size() - 1 ? "" : " ");
	return (os);
}
void solve();
int main()
{
	int t = 1;
	// cin >> t;
	while (t--)
	{
		solve();
	}
	return (0);
}

void solve()
{

}
