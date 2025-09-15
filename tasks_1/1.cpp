#include <iostream>
#include <cmath>

int main() {
    int n;
    std::cin >> n;
    long long f1 = 0, f2 = 1;
    for (int i = 2; i <= n; i++) {
        long long tmp = f2;
        f2 = f1 + f2;
        f1 = tmp;
    }
    std::cout << "a) " << f2 << "\n";
    double ans = (pow(1 + sqrt(5), n) - pow(1 - sqrt(5), n)) / (pow(2, n) * sqrt(5));
    long long ll_ans = ans;
    std::cout << "b) " << ll_ans << "\n";
}