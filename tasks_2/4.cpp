#include <iostream>
#include <vector>
#include <string>

std::string GetRomanEmpire(int n) {
    std::string res = "";
    std::vector<int> val = {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1};
    std::vector<std::string> symbol = {"M",  "CM", "D",  "CD", "C",  "XC", "L",
                                       "XL", "X",  "IX", "V",  "IV", "I"};
    for (size_t i = 0; i < val.size(); i++) {
        while (n >= val[i]) {
            res += symbol[i];
            n -= val[i];
        }
    }
    return res;
}
int main() {
    for (int n = 1; n <= 1000; n++) {
        std::cout << GetRomanEmpire(n) << "\n";
    }
}