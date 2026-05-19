#ifndef DEBUG_HPP
# define DEBUG_HPP

# include <bits/stdc++.h>
# include <cxxabi.h>

template <typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &p) {
    return os << "(" << p.first << ", " << p.second << ")";
}

template <typename T>
std::ostream &operator<<(std::ostream &os, const std::vector<T> &v) {
    os << "{";
    for (int i = 0; i < (int)v.size(); i++) {
        os << v[i] << (i == (int)v.size() - 1 ? "" : ", ");
    }
    return os << "}";
}

template <typename T>
std::ostream &operator<<(std::ostream &os, const std::set<T> &s) {
    os << "{";
    for (auto it = s.begin(); it != s.end(); it++) {
        os << *it << (std::next(it) == s.end() ? "" : ", ");
    }
    return os << "}";
}

template <typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::map<T, U> &m) {
    os << "{";
    for (auto it = m.begin(); it != m.end(); it++) {
        os << it->first << ": " << it->second << (std::next(it) == m.end() ? "" : ", ");
    }
    return os << "}";
}

template <typename T, typename Container, typename Compare>
std::ostream &operator<<(std::ostream &os, std::priority_queue<T, Container, Compare> pq) {
    os << "{";
    while (!pq.empty()) {
        os << pq.top() << (pq.size() == 1 ? "" : ", ");
        pq.pop();
    }
    return os << "}";
}

template <typename T, typename Container>
std::ostream &operator<<(std::ostream &os, std::stack<T, Container> s) {
    os << "{";
    std::vector<T> v;
    while (!s.empty()) {
        v.push_back(s.top());
        s.pop();
    }
    std::reverse(v.begin(), v.end());
    for (int i = 0; i < (int)v.size(); i++) {
        os << v[i] << (i == (int)v.size() - 1 ? "" : ", ");
    }
    return os << "}";
}

template <typename T, typename Container>
std::ostream &operator<<(std::ostream &os, std::queue<T, Container> q) {
    os << "{";
    while (!q.empty()) {
        os << q.front() << (q.size() == 1 ? "" : ", ");
        q.pop();
    }
    return os << "}";
}

namespace debug_internal {
    template <typename T>
    std::string type_name() {
        int status;
        char* demangled = abi::__cxa_demangle(typeid(T).name(), 0, 0, &status);
        std::string res = (status == 0) ? demangled : typeid(T).name();
        free(demangled);
        size_t pos;
        while ((pos = res.find("std::__cxx11::")) != std::string::npos) res.erase(pos, 14);
        while ((pos = res.find(", std::allocator<")) != std::string::npos) {
            int depth = 1;
            size_t end = pos + 17;
            while (end < res.size() && depth > 0) {
                if (res[end] == '<') depth++;
                if (res[end] == '>') depth--;
                end++;
            }
            res.erase(pos, end - pos);
        }
        // Simplify string
        while ((pos = res.find("basic_string<char, std::char_traits<char>>")) != std::string::npos) {
            res.replace(pos, 42, "string");
        }
        while ((pos = res.find("basic_string<char, std::char_traits<char> >")) != std::string::npos) {
            res.replace(pos, 43, "string");
        }
        // Remove std::
        while ((pos = res.find("std::")) != std::string::npos) res.erase(pos, 5);
        // Remove spaces before > to avoid "vector<int >"
        while ((pos = res.find(" >")) != std::string::npos) res.erase(pos, 1);
        return res;
    }

    inline std::vector<std::string> split_names(std::string s) {
        std::vector<std::string> res;
        int bracket = 0;
        std::string cur = "";
        for (char c : s) {
            if (c == ',' && bracket == 0) {
                while (!cur.empty() && cur.back() == ' ') cur.pop_back();
                res.push_back(cur);
                cur = "";
            } else {
                if (c == '(' || c == '[' || c == '{' || c == '<') bracket++;
                if (c == ')' || c == ']' || c == '}' || c == '>') bracket--;
                if (cur.size() || c != ' ') cur += c;
            }
        }
        while (!cur.empty() && cur.back() == ' ') cur.pop_back();
        res.push_back(cur);
        return res;
    }

    inline void debug_out(std::vector<std::string> names, int index) {
        (void)names; (void)index;
        std::cerr << "\033[0m" << std::endl;
    }

    template <typename Head, typename... Tail>
    inline void debug_out(std::vector<std::string> names, int index, Head H, Tail... T) {
        if (index > 0) std::cerr << ", ";
        std::cerr << "\033[36m[" << names[index] << "]\033[0m"
                  << "\033[33m(" << type_name<Head>() << ")\033[0m"
                  << " = " << H;
        debug_out(names, index + 1, T...);
    }
}

#define dg(...) \
    std::cerr << "\033[32m" << "Line " << __LINE__ << ": " << "\033[0m", \
    debug_internal::debug_out(debug_internal::split_names(#__VA_ARGS__), 0, __VA_ARGS__)

#endif
