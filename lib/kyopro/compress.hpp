#ifndef KYOPRO_COMPRESS_HPP
#define KYOPRO_COMPRESS_HPP

#include <algorithm>
#include <cassert>
#include <vector>

namespace kyopro {

template <class T>
class compressor {
   public:
    compressor() = default;

    explicit compressor(const std::vector<T>& values) : xs(values) {
        build();
    }

    template <class Iterator>
    compressor(Iterator first, Iterator last) : xs(first, last) {
        build();
    }

    void add(const T& x) {
        xs.push_back(x);
        built = false;
    }

    void build() {
        std::sort(xs.begin(), xs.end());
        xs.erase(std::unique(xs.begin(), xs.end()), xs.end());
        built = true;
    }

    int size() const {
        return (int)xs.size();
    }

    bool empty() const {
        return xs.empty();
    }

    int get(const T& x) const {
        assert(built);
        return (int)(std::lower_bound(xs.begin(), xs.end(), x) - xs.begin());
    }

    int lower_bound(const T& x) const {
        return get(x);
    }

    int upper_bound(const T& x) const {
        assert(built);
        return (int)(std::upper_bound(xs.begin(), xs.end(), x) - xs.begin());
    }

    bool contains(const T& x) const {
        int i = get(x);
        return i < size() && xs[i] == x;
    }

    const T& operator[](int i) const {
        assert(built);
        return xs[i];
    }

    const std::vector<T>& values() const {
        assert(built);
        return xs;
    }

   private:
    std::vector<T> xs;
    bool built = false;
};

template <class T>
struct compress_result {
    std::vector<int> compressed;
    std::vector<T> values;

    int size() const {
        return (int)values.size();
    }
};

template <class T>
compress_result<T> compress(const std::vector<T>& a) {
    compressor<T> comp(a);

    std::vector<int> compressed;
    compressed.reserve(a.size());
    for (const T& x : a) {
        compressed.push_back(comp.get(x));
    }

    return {compressed, comp.values()};
}

}  // namespace kyopro

#endif  // KYOPRO_COMPRESS_HPP
