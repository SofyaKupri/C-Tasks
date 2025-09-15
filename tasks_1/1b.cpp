#include <iostream>
#include <cmath>

int main() {
    int n;
    std::cin >> n;
    double ans = (pow(1 + sqrt(5), n) - pow(1 - sqrt(5), n)) / (pow(2, n) * sqrt(5));
    long long ll_ans = ans;
    std::cout << ll_ans << "\n";
}