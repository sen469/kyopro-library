#ifndef KYOPRO_AHO_CORASICK_HPP
#define KYOPRO_AHO_CORASICK_HPP

#include <array>
#include <cassert>
#include <limits>
#include <string>
#include <utility>
#include <vector>

namespace kyopro {

template <int CHAR_SIZE = 26, char BASE = 'a'>
class aho_corasick {
    static_assert(0 < CHAR_SIZE && CHAR_SIZE <= 256, "invalid alphabet size");
    static_assert((int)(unsigned char)BASE + CHAR_SIZE <= 256, "alphabet exceeds the byte range");

    struct node {
        std::array<int, CHAR_SIZE> child;
        std::array<int, CHAR_SIZE> go;
        std::vector<int> ids;
        int failure = 0;
        int output_link = -1;
        long long match_count = 0;

        node() {
            child.fill(-1);
            go.fill(0);
        }
    };

    std::vector<node> nodes_;
    std::vector<int> terminal_;
    std::vector<int> length_;
    std::vector<int> order_;
    bool built_ = false;

    static int char_id(char c) {
        int x = (int)(unsigned char)c - (int)(unsigned char)BASE;
        assert(0 <= x && x < CHAR_SIZE);
        return x;
    }

    void check_state(int state) const {
        assert(built_);
        assert(0 <= state && state < node_count());
        (void)state;
    }

    template <class F>
    void for_each_match(int state, F f) const {
        for (int v = state; v != -1; v = nodes_[v].output_link) {
            for (int id : nodes_[v].ids) f(id);
        }
    }

   public:
    aho_corasick() : nodes_(1) {}

    explicit aho_corasick(const std::vector<std::string>& patterns) : aho_corasick() {
        for (const auto& pattern : patterns) insert(pattern);
    }

    int insert(const std::string& pattern) {
        assert(pattern.size() <= (std::size_t)std::numeric_limits<int>::max());
        assert(size() < std::numeric_limits<int>::max());
        built_ = false;
        int v = 0;
        for (char c : pattern) {
            int x = char_id(c);
            int to = nodes_[v].child[x];
            if (to == -1) {
                assert(node_count() < std::numeric_limits<int>::max());
                to = node_count();
                nodes_[v].child[x] = to;
                nodes_.emplace_back();
            }
            v = to;
        }
        int id = size();
        nodes_[v].ids.push_back(id);
        terminal_.push_back(v);
        length_.push_back((int)pattern.size());
        return id;
    }

    void build() {
        order_.clear();
        order_.reserve(nodes_.size());
        order_.push_back(0);
        nodes_[0].failure = 0;
        nodes_[0].output_link = -1;
        nodes_[0].match_count = (long long)nodes_[0].ids.size();
        for (std::size_t i = 0; i < order_.size(); i++) {
            int v = order_[i];
            for (int c = 0; c < CHAR_SIZE; c++) {
                int to = nodes_[v].child[c];
                if (to == -1) {
                    nodes_[v].go[c] = v == 0 ? 0 : nodes_[nodes_[v].failure].go[c];
                    continue;
                }
                nodes_[v].go[c] = to;
                int f = v == 0 ? 0 : nodes_[nodes_[v].failure].go[c];
                nodes_[to].failure = f;
                // Link only terminal states; copying all suffix IDs can use quadratic space.
                nodes_[to].output_link = nodes_[f].ids.empty() ? nodes_[f].output_link : f;
                nodes_[to].match_count = (long long)nodes_[to].ids.size() + nodes_[f].match_count;
                order_.push_back(to);
            }
        }
        built_ = true;
    }

    int size() const { return (int)terminal_.size(); }
    int node_count() const { return (int)nodes_.size(); }

    int terminal(int id) const {
        assert(0 <= id && id < size());
        return terminal_[id];
    }

    int next(int state, char c) const {
        check_state(state);
        return nodes_[state].go[char_id(c)];
    }

    int failure(int state) const {
        check_state(state);
        return nodes_[state].failure;
    }

    long long match_count(int state) const {
        check_state(state);
        return nodes_[state].match_count;
    }

    std::vector<int> match_ids(int state) const {
        check_state(state);
        std::vector<int> result;
        for_each_match(state, [&](int id) { result.push_back(id); });
        return result;
    }

    std::vector<long long> count(const std::string& text) const {
        assert(built_);
        assert(text.size() <= (std::size_t)std::numeric_limits<int>::max());
        std::vector<long long> visits(node_count(), 0);
        int state = 0;
        visits[0] = 1;
        for (char c : text) {
            state = next(state, c);
            visits[state]++;
        }
        // A state visit also matches each suffix on its failure chain.
        for (int i = node_count() - 1; i > 0; i--) {
            int v = order_[i];
            visits[nodes_[v].failure] += visits[v];
        }
        std::vector<long long> result(size());
        for (int id = 0; id < size(); id++) result[id] = visits[terminal_[id]];
        return result;
    }

    long long count_matches(const std::string& text) const {
        assert(built_);
        assert(text.size() <= (std::size_t)std::numeric_limits<int>::max());
        int state = 0;
        long long result = match_count(0);
        for (char c : text) {
            state = next(state, c);
            result += match_count(state);
        }
        return result;
    }

    std::vector<std::pair<int, int>> find_all(const std::string& text) const {
        assert(built_);
        assert(text.size() <= (std::size_t)std::numeric_limits<int>::max());
        std::vector<std::pair<int, int>> result;
        int state = 0;
        for_each_match(0, [&](int id) { result.emplace_back(0, id); });
        for (int i = 0; i < (int)text.size(); i++) {
            state = next(state, text[i]);
            for_each_match(state, [&](int id) { result.emplace_back(i + 1 - length_[id], id); });
        }
        return result;
    }
};

}  // namespace kyopro

#endif  // KYOPRO_AHO_CORASICK_HPP
