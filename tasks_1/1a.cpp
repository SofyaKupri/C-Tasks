#include <cmath>
#include <iostream>

int main() {
  int n;
  std::cin >> n;
  long long f1 = 0, f2 = 1;
  for (int i = 2; i <= n; i++) {
    long long tmp = f2;
    f2 = f1 + f2;
    f1 = tmp;
  }
  std::cout << f2 << "\n";
}