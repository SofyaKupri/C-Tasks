#include <cmath>
#include <iostream>

int main() {
    int n;
    std::cin >> n;
    double ans = (pow(1 + sqrt(5), n) - pow(1 - sqrt(5), n)) / (pow(2, n) * sqrt(5));
    int64_t int_ans = ans;
    std::cout << int_ans << "\n";
}