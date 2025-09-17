#include <iostream>

int main() {
    int a, b;
    std::cin >> a >> b;
    while (a != 0) {
        int a_new = b % a, b_new = a;
        a = a_new;
        b = b_new;
    }
    std::cout << b << "\n";
}