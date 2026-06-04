#ifndef KYOPRO_INTERVAL_HEAP_HPP
#define KYOPRO_INTERVAL_HEAP_HPP

#include <cassert>
#include <functional>
#include <initializer_list>
#include <set>
#include <utility>
#include <vector>

namespace kyopro {

template <class T, class Compare = std::less<T>>
class interval_heap {
   private:
    std::multiset<T, Compare> data_;

   public:
    interval_heap() = default;

    explicit interval_heap(const Compare& comp) : data_(comp) {}

    explicit interval_heap(const std::vector<T>& v, const Compare& comp = Compare()) : data_(comp) {
        for (const T& x : v) push(x);
    }

    interval_heap(std::initializer_list<T> init, const Compare& comp = Compare()) : data_(comp) {
        for (const T& x : init) push(x);
    }

    int size() const {
        return (int)data_.size();
    }

    bool empty() const {
        return data_.empty();
    }

    void clear() {
        data_.clear();
    }

    const T& min() const {
        assert(!empty());
        return *data_.begin();
    }

    const T& max() const {
        assert(!empty());
        return *data_.rbegin();
    }

    const T& top_min() const {
        return min();
    }

    const T& top_max() const {
        return max();
    }

    void push(const T& x) {
        data_.insert(x);
    }

    void push(T&& x) {
        data_.insert(std::move(x));
    }

    template <class... Args>
    void emplace(Args&&... args) {
        data_.emplace(std::forward<Args>(args)...);
    }

    void pop_min() {
        assert(!empty());
        data_.erase(data_.begin());
    }

    void pop_max() {
        assert(!empty());
        auto it = data_.end();
        --it;
        data_.erase(it);
    }
};

}  // namespace kyopro

#endif  // KYOPRO_INTERVAL_HEAP_HPP
