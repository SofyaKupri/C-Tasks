#include <iostream>
#include <cmath>
#include <iomanip>

const double eps = 1e-9;

int main() {
    double a, b, c;
    std::cin >> a >> b >> c;
    if (abs(a) < eps) {
        std::cout << "Incorrect equation\n";
        return 0;
    }
    double d = b * b - 4 * a * c;
    if (d - eps < 0) {
        std::cout << "No solutions\n";
    }
    else if (abs(d) < eps) {
        double x = -b / (2 * a);
        std::cout << std::setprecision(8) << std::fixed << x << "\n";
    }
    else {
        double x1 = -(b - sqrt(d)) / (2 * a), x2 = -(b + sqrt(d)) / (2 * a);
        std::cout << std::setprecision(8) << std::fixed << x1 << " " << x2 << "\n";
    }
}