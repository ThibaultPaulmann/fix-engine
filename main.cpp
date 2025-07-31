#include <iostream>
#include <vector>
#include <ranges>


int main() {
    std::vector<int> v = {1, 2, 3, 4, 5};
    for (int x : v | std::views::filter([](int n){ return n % 2 == 1; }))
        std::cout << x << ' ';
}
