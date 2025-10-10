#include <iostream>

int GetGrayCode(int n) {
    return n ^ (n >> 1);
}
int GetGrayDecode(int n) {
    int res = 0;
    while (n > 0) {
        res ^= n;
        n >>= 1;
    }
    return res;
}
void Print2(int n) {
    for (int i = 4; i >= 0; i--) {
        std::cout << ((n & (1 << i)) > 0);
    }
}

int main() {
    int n;
    std::cin >> n;
    Print2(n);
    std::cout << " ";
    Print2(GetGrayCode(n));
    std::cout << " ";
    Print2(GetGrayDecode(GetGrayCode(n)));
    std::cout << "\n";
}