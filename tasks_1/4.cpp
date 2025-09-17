#include <iostream>

int main() {
    int max_len = 0;
    for (int n = 1; n <= 100; n++) {
        int cur = n, cur_len = 1;
        while (cur > 1) {
            if (cur & 1) {
                cur = cur * 3 + 1;
            }
            else {
                cur /= 2;
            }
            ++cur_len;
        }
        max_len = std::max(max_len, cur_len);
    }
    std::cout << max_len << "\n";
}