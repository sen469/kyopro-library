#ifndef KYOPRO_TRIE_HPP
#define KYOPRO_TRIE_HPP

#include <array>
#include <string>
#include <vector>

namespace kyopro {

template <int CHAR_SIZE = 26, char BASE = 'a'>
class trie {
   public:
    struct node {
        std::array<int, CHAR_SIZE> next;
        int pass_count;
        int end_count;

        node() : pass_count(0), end_count(0) {
            next.fill(-1);
        }
    };

   private:
    std::vector<node> nodes_;

    static int char_id(char c) {
        return c - BASE;
    }

    int find_node(const std::string& s) const {
        int v = 0;
        for (char c : s) {
            int x = char_id(c);
            if (nodes_[v].next[x] == -1) return -1;
            v = nodes_[v].next[x];
        }
        return v;
    }

   public:
    trie() : nodes_(1) {}

    void insert(const std::string& s) {
        int v = 0;
        nodes_[v].pass_count++;
        for (char c : s) {
            int x = char_id(c);
            if (nodes_[v].next[x] == -1) {
                nodes_[v].next[x] = (int)nodes_.size();
                nodes_.push_back(node());
            }
            v = nodes_[v].next[x];
            nodes_[v].pass_count++;
        }
        nodes_[v].end_count++;
    }

    bool erase(const std::string& s) {
        if (!contains(s)) return false;

        int v = 0;
        nodes_[v].pass_count--;
        for (char c : s) {
            v = nodes_[v].next[char_id(c)];
            nodes_[v].pass_count--;
        }
        nodes_[v].end_count--;
        return true;
    }

    int count(const std::string& s) const {
        int v = find_node(s);
        if (v == -1) return 0;
        return nodes_[v].end_count;
    }

    bool contains(const std::string& s) const {
        return count(s) > 0;
    }

    bool starts_with(const std::string& prefix) const {
        return find_node(prefix) != -1;
    }

    int prefix_count(const std::string& prefix) const {
        int v = find_node(prefix);
        if (v == -1) return 0;
        return nodes_[v].pass_count;
    }

    int size() const {
        return nodes_[0].pass_count;
    }

    int node_count() const {
        return (int)nodes_.size();
    }

    const node& operator[](int i) const {
        return nodes_[i];
    }
};

}  // namespace kyopro

#endif  // KYOPRO_TRIE_HPP
