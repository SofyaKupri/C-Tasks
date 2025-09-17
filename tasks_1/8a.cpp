#include <iostream>

int main() {
    long n;
    std::cin >> n;
    int log_n = 0;
    while (n > 1) {
        ++log_n;
        n /= 2;
    }
    std::cout << log_n << "\n";
}