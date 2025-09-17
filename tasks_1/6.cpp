#include <iostream>

const int str_size = 101;

int main() {
    char string1[str_size], string2[str_size];
    for (int i = 0; i < str_size; i++) {
        string1[i] = '\0';
        string2[i] = '\0';
    }
    std::cin >> string1 >> string2;
    int ans = 0;
    char* best = new char[str_size];
    for (int i = 0; i < str_size; i++) {
        for (int j = 0; j < str_size; j++) {
            int cur_ans = 0;
            char *s1 = new char[str_size], *s2 = new char[str_size];
            for (int sub_size = 1; std::max(i + sub_size, j + sub_size) < str_size; sub_size++) {
                if (string1[i + sub_size - 1] != string2[j + sub_size - 1] ||
                    string1[i + sub_size - 1] == '\0') {
                    break;
                }
                s1[sub_size - 1] = string1[i + sub_size - 1];
                s2[sub_size - 1] = string2[j + sub_size - 1];
                cur_ans++;
            }
            if (cur_ans > ans) {
                ans = cur_ans;
                best = s1;
            }
        }
    }
    std::cout << best << "\n";
}