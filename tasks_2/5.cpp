#include <iostream>
#include <vector>
#include <algorithm>

struct Road {
    Road() {
        from_num = to_num = distance = 0;
    }
    int from_num;
    int to_num;
    double distance;
};

std::istream& operator>>(std::istream& in, Road& r) {
    in >> r.from_num >> r.to_num >> r.distance;
    return in;
}

void FindPath(double (&c)[11][11], std::vector<int>& path, double& cost) {
    std::vector<int> cur(9);
    for (int i = 0; i < 9; i++) {
        cur[i] = i + 1;
    }
    do {
        if (c[0][cur[0]] == -1 || c[cur[8]][0] == -1) {
            continue;
        }
        bool ok = true;
        double cur_sum = c[0][cur[0]] + c[cur[8]][0];
        for (int i = 0; i < 8; i++) {
            if (c[cur[i]][cur[i + 1]] == -1) {
                ok = false;
                break;
            }
            cur_sum += c[cur[i]][cur[i + 1]];
        }
        if (ok && (cost == -1 || cost > cur_sum)) {
            cost = cur_sum;
            for (int i = 0; i < 9; i++) {
                path[i] = cur[i];
            }
        }
    } while (std::next_permutation(cur.begin(), cur.end()));
}
int main() {
    int m;
    std::cin >> m;
    double c[11][11];
    for (int i = 0; i < 11; i++) {
        for (int j = 0; j < 11; j++) {
            c[i][j] = -1;
        }
    }
    for (int i = 0; i < m; i++) {
        Road cur;
        std::cin >> cur;
        c[cur.from_num][cur.to_num] = cur.distance;
        c[cur.to_num][cur.from_num] = cur.distance;
    }
    std::vector<int> path(9);
    double cost = -1;
    FindPath(c, path, cost);
    if (cost == -1) {
        std::cout << "NO SOLUTION\n";
        return 0;
    }
    std::cout << cost << "\n0 ";
    for (int i = 0; i < 9; i++) {
        std::cout << path[i] << " ";
    }
    std::cout << "0\n";
}