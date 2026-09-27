#ifndef KYOPRO_MO_HPP
#define KYOPRO_MO_HPP

#include <algorithm>
#include <cassert>
#include <cmath>
#include <numeric>
#include <utility>
#include <vector>

namespace kyopro {

class mo {
private:
    struct query {
        int left;
        int right;
    };

    int n_;
    std::vector<query> queries_;

public:
    mo() : n_(0) {}

    explicit mo(int n) : n_(n) { assert(0 <= n); }

    int add_query(int left, int right) {
        assert(0 <= left && left <= right && right <= n_);
        int id = (int)queries_.size();
        queries_.push_back({left, right});
        return id;
    }

    int element_count() const { return n_; }

    int query_count() const { return (int)queries_.size(); }

    template <class AddLeft, class AddRight, class EraseLeft, class EraseRight, class Answer>
    void run(AddLeft&& add_left, AddRight&& add_right, EraseLeft&& erase_left,
             EraseRight&& erase_right, Answer&& answer) const {
        int q = query_count();
        if (q == 0) return;

        int block_size = std::max(1, (int)(n_ / std::sqrt((double)q)));
        std::vector<int> order(q);
        std::iota(order.begin(), order.end(), 0);
        std::sort(order.begin(), order.end(), [&](int a, int b) {
            int block_a = queries_[a].left / block_size;
            int block_b = queries_[b].left / block_size;
            if (block_a != block_b) return block_a < block_b;
            if (block_a & 1) return queries_[a].right > queries_[b].right;
            return queries_[a].right < queries_[b].right;
        });

        int left = 0;
        int right = 0;
        for (int id : order) {
            int query_left = queries_[id].left;
            int query_right = queries_[id].right;
            while (query_left < left) add_left(--left);
            while (right < query_right) add_right(right++);
            while (left < query_left) erase_left(left++);
            while (query_right < right) erase_right(--right);
            answer(id);
        }
    }

    template <class Add, class Erase, class Answer>
    void run(Add&& add, Erase&& erase, Answer&& answer) const {
        run(add, add, erase, erase, answer);
    }
};

}  // namespace kyopro

#endif  // KYOPRO_MO_HPP
