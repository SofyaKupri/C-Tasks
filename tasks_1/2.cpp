#include <iostream>

int ToCube(int x) {
    return x * x * x;
}

int main() {
    for (int c1 = 1; c1 < 10; c1++) {
        for (int c2 = 0; c2 < 10; c2++) {
            for (int c3 = 0; c3 < 10; c3++) {
                int number = c1 * 100 + c2 * 10 + c3;
                int sum = ToCube(c1) + ToCube(c2) + ToCube(c3);
                if (number == sum) {
                    std::cout << number << "\n";
                }
            }
        }
    }
}