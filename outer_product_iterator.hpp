#ifndef PRODUCT_OUTER_PRODUCT_ITERATOR_HPP
#define PRODUCT_OUTER_PRODUCT_ITERATOR_HPP

#include <array>
#include <type_traits>

namespace ld {

template <class T, std::size_t N>
struct CarrySumIterator {
    std::array<T, N> min;
    std::array<T, N> max;
    std::array<T, N> i;

    constexpr CarrySumIterator() = default;

    constexpr CarrySumIterator(std::array<T, N> min, std::array<T, N> max)
        : min(min)
        , max(max)
        , i(min)
    {
    }

    constexpr auto& operator*() const { 
        return i;
    }

    constexpr auto& operator++() 
    {
        // reverse fill the array to make the outer index the outer loop
        for (std::size_t n = 0, m = N - n - 1; n < N; i[m] = min[m], ++n, m = N - n - 1) {
            if (++i[m] < max[m]) {
                return *this; // no carry
            }
        }
        i = max;
        return *this;
    }

    constexpr friend auto operator==(const CarrySumIterator& a, const CarrySumIterator& b) {
        return a.i == b.i;
    }

    constexpr friend auto operator<=>(const CarrySumIterator& a, const CarrySumIterator& b) {
        return a.i <=> b.i;
    }
};

template <class T, std::size_t N>
struct dense {
    std::array<T, N> min;
    std::array<T, N> max;
    
    constexpr dense(std::array<T, N> max)
        : max(max)
    {
    }

    constexpr dense(std::array<T, N> min, std::array<T, N> max)
        : min(min)
        , max(max)
    {
    }

    constexpr dense(std::array<std::array<T, 2>, N> aos) 
    {
        for (int n = 0; n < N; ++n) {
            min[n] = aos[n][0];
            max[n] = aos[n][1];
        }
    }

    constexpr auto begin() const { return CarrySumIterator(min, max); }
    constexpr auto   end() const { return CarrySumIterator(max, max); }
};

template <class... Bounds>
auto outer_product(Bounds&&... bounds) {
    using std::get;
    using T = std::common_type_t<std::decay_t<decltype(get<0>(bounds))>...>;
    constexpr std::size_t N = sizeof...(bounds);
    std::array<T, N> min;
    std::array<T, N> max;
    int n = 0;
    ((min[n] = get<0>(bounds), max[n] = get<1>(bounds), ++n), ...);
    return dense(min, max);
}

}
#endif
