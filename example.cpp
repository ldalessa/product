#include "outer_product_iterator.hpp"
#include <cstdio>

int main() 
{
    {
        std::array min = {1, 2, 0};
        std::array max = {3, 4, 2};
        for (auto [i, j, k] : dense(min, max)) {
            printf("1: %d %d %d\n", i, j, k);
        }
    }

    {
        std::array bounds = {
            std::array<int, 2>{1, 5},
            std::array<int, 2>{2, 7}
        };

        for (auto [i, j] : dense(bounds)) {
            printf("2: %d %d\n", i, j);
        }
    }

    {
        std::array I = {1, 5};
        std::array J = {2, 7};
        for (auto [i, j] : outer_product(I, J)) {
            printf("3: %d %d\n", i, j);
        }
    }
}
