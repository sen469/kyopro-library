#include <bits/stdc++.h>

#include "kyopro/aho_corasick.hpp"

using namespace std;

template <int CHAR_SIZE, char BASE>
void check(const kyopro::aho_corasick<CHAR_SIZE, BASE>& ac,
           const vector<string>& patterns, const string& text) {
    vector<long long> counts(patterns.size(), 0);
    vector<pair<int, int>> matches;
    for (int id = 0; id < (int)patterns.size(); id++) {
        for (int pos = 0; pos + (int)patterns[id].size() <= (int)text.size(); pos++) {
            if (text.compare(pos, patterns[id].size(), patterns[id]) == 0) {
                counts[id]++;
                matches.emplace_back(pos, id);
            }
        }
    }
    assert(ac.size() == (int)patterns.size());
    assert(ac.count(text) == counts);
    assert(ac.count_matches(text) == accumulate(counts.begin(), counts.end(), 0LL));
    sort(matches.begin(), matches.end(), [&](auto a, auto b) {
        int end_a = a.first + (int)patterns[a.second].size();
        int end_b = b.first + (int)patterns[b.second].size();
        if (end_a != end_b) return end_a < end_b;
        if (patterns[a.second].size() != patterns[b.second].size()) {
            return patterns[a.second].size() > patterns[b.second].size();
        }
        return a.second < b.second;
    });
    assert(ac.find_all(text) == matches);

    int state = 0;
    for (int end = 0; end <= (int)text.size(); end++) {
        vector<int> expected;
        for (int id = 0; id < (int)patterns.size(); id++) {
            int length = (int)patterns[id].size();
            if (length <= end && text.compare(end - length, length, patterns[id]) == 0) {
                expected.push_back(id);
            }
        }
        auto actual = ac.match_ids(state);
        assert(ac.match_count(state) == (long long)expected.size());
        sort(actual.begin(), actual.end());
        assert(actual == expected);
        if (end < (int)text.size()) state = ac.next(state, text[end]);
    }
}

int main() {
    {
        vector<string> patterns = {"he", "she", "his", "hers"};
        kyopro::aho_corasick<> ac(patterns);
        ac.build();
        check(ac, patterns, "ushers");
        assert(ac.count("ushers") == vector<long long>({1, 1, 0, 1}));
        assert(ac.failure(ac.terminal(1)) == ac.terminal(0));
        ac.build();
        check(ac, patterns, "shershehishers");
        int old_terminal = ac.terminal(3);
        assert(ac.insert("s") == 4);
        patterns.push_back("s");
        ac.build();
        assert(ac.terminal(3) == old_terminal);
        assert(ac.failure(old_terminal) == ac.terminal(4));
        check(ac, patterns, "ushershishers");
    }
    {
        vector<string> patterns = {"", "a", "aa", "a", ""};
        kyopro::aho_corasick<> ac(patterns);
        ac.build();
        assert(ac.terminal(0) == 0 && ac.terminal(4) == 0);
        assert(ac.terminal(1) == ac.terminal(3));
        check(ac, patterns, "aaaa");
        check(ac, patterns, "");
        ac.insert("aaa");
        patterns.push_back("aaa");
        ac.build();
        check(ac, patterns, "aaaa");
    }
    {
        kyopro::aho_corasick<> ac;
        assert(ac.size() == 0 && ac.node_count() == 1);
        ac.build();
        check(ac, {}, "abc");
        check(ac, {}, "");
        assert(ac.failure(0) == 0);
        ac.insert("");
        ac.build();
        check(ac, {""}, "abc");
        ac.build();
        check(ac, {""}, "abc");
    }
    {
        vector<string> patterns = {"0", "01", "101", "01"};
        kyopro::aho_corasick<10, '0'> ac(patterns);
        ac.build();
        check(ac, patterns, "0101019");
        string bytes;
        bytes.push_back('\0');
        bytes.push_back((char)255);
        bytes.push_back('\0');
        vector<string> byte_patterns = {string(1, '\0'), string(1, (char)255), bytes, ""};
        kyopro::aho_corasick<256, '\0'> byte_ac(byte_patterns);
        byte_ac.build();
        check(byte_ac, byte_patterns, bytes + bytes);
    }
    {
        mt19937 rng(469);
        for (int iteration = 0; iteration < 500; iteration++) {
            vector<string> patterns;
            kyopro::aho_corasick<3> ac;
            for (int phase = 0; phase < 3; phase++) {
                for (int k = (int)(rng() % 6); k > 0; k--) {
                    string pattern(rng() % 8, 'a');
                    for (char& c : pattern) c += rng() % 3;
                    assert(ac.insert(pattern) == (int)patterns.size());
                    patterns.push_back(pattern);
                }
                ac.build();
                string text(rng() % 50, 'a');
                for (char& c : text) c += rng() % 3;
                check(ac, patterns, text);
                ac.build();
                check(ac, patterns, text);
            }
        }
    }
    {
        kyopro::aho_corasick<1> ac;
        for (int length = 1; length <= 1000; length++) ac.insert(string(length, 'a'));
        ac.build();
        auto counts = ac.count(string(20000, 'a'));
        long long total = 0;
        for (int id = 0; id < 1000; id++) {
            assert(counts[id] == 20000 - id);
            total += counts[id];
        }
        assert(ac.count_matches(string(20000, 'a')) == total);
        assert(ac.node_count() == 1001);
    }
    {
        kyopro::aho_corasick<1> ac;
        ac.insert(string(100000, 'a'));
        ac.build();
        assert(ac.count(string(100001, 'a')) == vector<long long>({2}));
        ac.build();
        assert(ac.count_matches(string(100001, 'a')) == 2);
    }
}
