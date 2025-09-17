#include <iostream>

int main() {
    double n;
    std::cin >> n;
    int log_n = 0;
    double cur2 = 1;
    if (n >= 1) {
        while (cur2 * 2 <= n) {
            cur2 *= 2;
            log_n++;
        }
    }
    else {
        while (cur2 > n) {
            cur2 /= 2;
            log_n--;
        }
    }
    std::cout << log_n << "\n";
}