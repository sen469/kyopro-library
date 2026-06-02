#include <bits/stdc++.h>

#include "kyopro/trie.hpp"

using namespace std;

int main() {
    {
        kyopro::trie<> trie;
        trie.insert("app");
        trie.insert("apple");
        trie.insert("apply");
        trie.insert("bat");
        trie.insert("app");

        assert(trie.size() == 5);
        assert(trie.count("app") == 2);
        assert(trie.count("apple") == 1);
        assert(trie.count("ap") == 0);
        assert(trie.contains("apply"));
        assert(!trie.contains("banana"));
        assert(trie.starts_with("app"));
        assert(trie.starts_with("ba"));
        assert(!trie.starts_with("cat"));
        assert(trie.prefix_count("app") == 4);
        assert(trie.prefix_count("a") == 4);
        assert(trie.prefix_count("bat") == 1);
        assert(trie.prefix_count("") == 5);

        assert(trie.erase("app"));
        assert(trie.count("app") == 1);
        assert(trie.prefix_count("app") == 3);
        assert(trie.size() == 4);

        assert(trie.erase("app"));
        assert(!trie.contains("app"));
        assert(trie.starts_with("app"));
        assert(trie.prefix_count("app") == 2);
        assert(!trie.erase("app"));
    }

    {
        kyopro::trie<10, '0'> trie;
        trie.insert("012");
        trie.insert("019");
        trie.insert("9");

        assert(trie.contains("012"));
        assert(!trie.contains("01"));
        assert(trie.starts_with("01"));
        assert(trie.prefix_count("01") == 2);
        assert(trie.prefix_count("9") == 1);
    }

    {
        kyopro::trie<> trie;
        trie.insert("");
        trie.insert("a");

        assert(trie.size() == 2);
        assert(trie.contains(""));
        assert(trie.count("") == 1);
        assert(trie.prefix_count("") == 2);
        assert(trie.erase(""));
        assert(!trie.contains(""));
        assert(trie.prefix_count("") == 1);
    }

    return 0;
}
