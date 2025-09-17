#include <iostream>
#include <cmath>
#include <iomanip>

int main() {
    double a, b, c;
    std::cin >> a >> b >> c;
    if (a == 0) {
        std::cout << "Incorrect equation\n";
        return 0;
    }
    double d = b * b - 4 * a * c;
    if (d < 0) {
        std::cout << "No solutions\n";
    }
    else if (d == 0) {
        double x = -b / (2 * a);
        std::cout << std::setprecision(8) << std::fixed << x << "\n";
    }
    else {
        double x1 = -(b - sqrt(d)) / (2 * a), x2 = -(b + sqrt(d)) / (2 * a);
        std::cout << std::setprecision(8) << std::fixed << x1 << " " << x2 << "\n";
    }
}