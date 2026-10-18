#include <cassert>
#include <deque>
#include <functional>
#include <memory_resource>
#include <set>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

#include "debug/debug.hpp"

template <class T>
std::string render(const T& value) {
    std::ostringstream out;
    out << value;
    return out.str();
}

std::string strip_colors(const std::string& text) {
    std::string result;
    for (std::size_t i = 0; i < text.size(); i++) {
        if (text[i] == '\033' && i + 1 < text.size() && text[i + 1] == '[') {
            while (i < text.size() && text[i] != 'm') i++;
        } else {
            result += text[i];
        }
    }
    return result;
}

int main() {
    assert(render(std::deque<int>{}) == "{}");
    assert(render(std::deque<int>{7}) == "{7}");
    assert(render(std::deque<int>{3, 1, 2}) == "{3, 1, 2}");
    assert(render(std::multiset<int>{}) == "{}");
    assert(render(std::multiset<int>{7}) == "{7}");
    assert(render(std::multiset<int>{3, 1, 1, 2}) == "{1, 1, 2, 3}");
    assert(render(std::multiset<int, std::greater<int>>{3, 1, 1, 2}) ==
           "{3, 2, 1, 1}");

    std::pmr::monotonic_buffer_resource resource;
    std::pmr::deque<int> custom_deque(&resource);
    custom_deque.push_back(1);
    custom_deque.push_back(2);
    assert(render(custom_deque) == "{1, 2}");
    std::pmr::multiset<int, std::greater<int>> custom_multiset(&resource);
    custom_multiset.insert({1, 3, 1});
    assert(render(custom_multiset) == "{3, 1, 1}");

    __int128 large = __int128(1) << 100;
    __int128 maximum = ((__int128(1) << 126) - 1) + (__int128(1) << 126);
    __int128 minimum = -maximum - 1;
    assert(render(__int128(0)) == "0");
    assert(render(__int128(42)) == "42");
    assert(render(__int128(-42)) == "-42");
    assert(render(large) == "1267650600228229401496703205376");
    assert(render(-large) == "-1267650600228229401496703205376");
    assert(render(maximum) == "170141183460469231731687303715884105727");
    assert(render(minimum) == "-170141183460469231731687303715884105728");

    std::deque<std::multiset<__int128>> nested = {{large, -1, -1}, {}};
    assert(render(nested) == "{{-1, -1, 1267650600228229401496703205376}, {}}");
    std::vector<std::deque<__int128>> vectors = {{minimum, maximum}, {}};
    assert(render(vectors) ==
           "{{-170141183460469231731687303715884105728, "
           "170141183460469231731687303715884105727}, {}}");
    assert(render(std::make_pair(std::deque<int>{1, 2}, large)) ==
           "({1, 2}, 1267650600228229401496703205376)");
    assert(render(std::make_tuple(large, std::multiset<int>{2, 1, 1})) ==
           "(1267650600228229401496703205376, {1, 1, 2})");
    assert(render(std::map<int, std::deque<int>>{{1, {2, 3}}}) == "{1: {2, 3}}");
    assert(render(std::set<int>{2, 1}) == "{1, 2}");

    std::queue<__int128> queue;
    queue.push(large);
    queue.push(-1);
    assert(render(queue) == "{1267650600228229401496703205376, -1}");
    std::stack<__int128> stack;
    stack.push(large);
    stack.push(-1);
    assert(render(stack) == "{1267650600228229401496703205376, -1}");
    std::priority_queue<__int128> priority_queue;
    priority_queue.push(-1);
    priority_queue.push(large);
    assert(render(priority_queue) == "{1267650600228229401496703205376, -1}");

    auto original = nested;
    std::ostringstream captured;
    auto* previous = std::cerr.rdbuf(captured.rdbuf());
    dg(nested, minimum);
    std::cerr.rdbuf(previous);
    assert(nested == original);
    auto output = strip_colors(captured.str());
    assert(output.find("Line ") == 0);
    assert(output.find("[nested]") != std::string::npos);
    assert(output.find(" = {{-1, -1, 1267650600228229401496703205376}, {}}") !=
           std::string::npos);
    assert(output.find("[minimum]") != std::string::npos);
    assert(output.find(" = -170141183460469231731687303715884105728") !=
           std::string::npos);
}
