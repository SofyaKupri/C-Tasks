#include <iostream>

const int str_size = 101;

int main() {
    char string[str_size];
    std::cin >> string;
    char *ans = new char[str_size];
    int ans_size = 0;
    for (int i = 0; i < str_size; i++) {
        for (int j = i; j < str_size; j++) {
            if (string[j] == '\0') {
                break;
            }
            bool isPalindrome = true;
            for (int k = 0; k < (j - i + 1) / 2; k++) {
                if (string[i + k] != string[j - k]) {
                    isPalindrome = false;
                    break;
                }
            }
            if (isPalindrome && ans_size < j - i + 1) {
                ans_size = j - i + 1;
                for (int k = i; k <= j; k++) {
                    ans[k - i] = string[k];
                }
                for (int k = j - i + 1; k < str_size; k++) {
                    ans[k] = '\0';
                }
            }
        }
    }
    std::cout << ans << "\n";
}