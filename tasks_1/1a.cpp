#include <cmath>
#include <iostream>

int main() {
    int n;
    std::cin >> n;
    int64_t fib_prev = 0, fib_cur = 1;
    for (int i = 2; i <= n; i++) {
        int64_t tmp = fib_cur;
        fib_cur = fib_prev + fib_cur;
        fib_prev = tmp;
    }
    std::cout << fib_cur << "\n";
}