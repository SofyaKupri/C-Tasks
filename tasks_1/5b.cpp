#include <iostream>

int GCD(int a, int b) {
    if (a == 0) {
        return b;
    }
    return GCD(b % a, a);
}

int main() {
    int a, b;
    std::cin >> a >> b;
    std::cout << GCD(a, b) << "\n";
}